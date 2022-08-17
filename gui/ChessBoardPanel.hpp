#pragma once

#include <array>
#include <memory>
#include <chrono>
#include <thread>

#include <wx/wx.h>
#include <wx/dragimag.h>
#include <wx/mstream.h>

#include "ChessBoardPiece.hpp"
#include "ChessBoardEngine.hpp"
#include "ChessBoardEvent.hpp"
#include "ChessPieces.h"
#include "Move.h"
#include "types.h"
#include "Utils.h"

class CowbotGuiFrame;

class ChessBoardPanel : public wxPanel
{
public:
    ChessBoardPanel(wxWindow* parent);

private:
    friend class CowbotGuiFrame;

    void OnPaint(wxPaintEvent&);
    void OnLeftDown(wxMouseEvent& evt);
    void OnLeftUp(wxMouseEvent& evt);
    void OnMotion(wxMouseEvent& evt);
    void OnComputerTurn();

    void initPieces();
    void resizePieces(std::array<ChessBoardPiece, 64>& board, int width, int height);
    void setBoard();
    void makeMove(Cowbot::Move move);

    Square getSquare(const wxPoint& point) const;
    Square mCurrentSq;
    int mWidth;
    int mHeight;
    std::array<wxImage, 13> mPieces;
    std::array<ChessBoardPiece, 64> mBoard;
    std::unique_ptr<ChessBoardEngine> mEngine;
    wxImage mToDrag;
    wxDragImage* mDragImage;
    wxPoint mStartPos;
};

ChessBoardPanel::ChessBoardPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED),
      mCurrentSq(Square::Null),
      mWidth(),
      mHeight(),
      mPieces(),
      mEngine(std::make_unique<ChessBoardEngine>()),
      mDragImage(nullptr)

{
    SetCursor(wxCursor(wxCURSOR_ARROW));
    SetBackgroundColour(*wxWHITE);
    wxImage::AddHandler(new wxPNGHandler());
    initPieces();
    setBoard();

    Show();
    std::cout << std::hex << mEngine->getHash() << std::endl;

    Bind(wxEVT_PAINT, &ChessBoardPanel::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &ChessBoardPanel::OnLeftDown, this);
    Bind(wxEVT_LEFT_UP, &ChessBoardPanel::OnLeftUp, this);
    Bind(wxEVT_MOTION, &ChessBoardPanel::OnMotion, this);
}

void ChessBoardPanel::OnPaint(wxPaintEvent&)
{
    wxPoint drawCords;
    GetClientSize(&mWidth, &mHeight);
    int sqWidth = mWidth / 8;
    int sqHeight = mHeight / 8;
    drawCords.y = 7 * sqHeight;

    wxPaintDC painter(this);
    wxDCPenChanger painterPen(painter, *wxTRANSPARENT_PEN);
    wxBrush painterBrush;
    wxColour squareColor;

    auto board = mBoard;
    resizePieces(board, sqWidth, sqHeight);

    wxSize sqSize(sqWidth, sqHeight);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            (i + j) % 2 ? squareColor.Set(233, 217, 185) : squareColor.Set(170, 138, 104);
            painterBrush.SetColour(squareColor);
            painter.SetBrush(painterBrush);
            painter.DrawRectangle(drawCords, sqSize);
            if (board[8 * i + j].getShow()) {
                painter.DrawBitmap(wxBitmap(board[8 * i + j].getImage()), drawCords);
            }
            drawCords.x += sqWidth;
        }
        drawCords.y -= sqHeight;
        drawCords.x -= 8 * sqWidth;
    }
}

Square ChessBoardPanel::getSquare(const wxPoint& point) const
{
    int row = 8 * point.y / mHeight, file = 8 * point.x / mWidth;

    // wxLogMessage("x:%d y:%d width:%d height:%d row:%d file:%d", point.x, point.y, mWidth, mHeight, row, file);

    if (row < 0 || file < 0 || row > 7 || file > 7) {
        return Square::Null;
    }

    return Square(8 * (7 - row) + file);
}

void ChessBoardPanel::OnLeftDown(wxMouseEvent& evt)
{
    evt.Skip();

    int sqWidth = mWidth / 8;
    int sqHeight = mHeight / 8;

    mCurrentSq = getSquare(evt.GetPosition());
    mToDrag = mBoard[to_int(mCurrentSq)].getImage();
    if (!mToDrag.IsOk()) {
        return;
    }

    mBoard[to_int(mCurrentSq)].setShow(false);
    Refresh(true);
    Update();

    mToDrag.Rescale(sqWidth, sqHeight, wxIMAGE_QUALITY_HIGH);
    mStartPos = evt.GetPosition();
    mDragImage = new wxDragImage(wxBitmap(mToDrag));
    mDragImage->BeginDrag(wxPoint(0, 0), this); 
    mDragImage->Move(evt.GetPosition() - wxPoint(mWidth / 16, mHeight / 16));
    mDragImage->Show();
}

void ChessBoardPanel::OnMotion(wxMouseEvent& evt)
{
    evt.Skip();

    if (!mDragImage) {
        return;
    }

    mDragImage->Move(evt.GetPosition() - wxPoint(mWidth / 16, mHeight / 16));
    mDragImage->Show();
}

void ChessBoardPanel::OnLeftUp(wxMouseEvent& evt)
{
    evt.Skip();

    if (mCurrentSq == Square::Null || !HasCapture()) {
        return;
    }

    mDragImage->Hide();
    mDragImage->EndDrag();
    wxDELETE(mDragImage);
    Square endingSquare = getSquare(evt.GetPosition());
    mBoard[to_int(mCurrentSq)].setShow(true);
    Cowbot::Move move = mEngine->isLegal(mCurrentSq, endingSquare);

    if (endingSquare == Square::Null || mCurrentSq == endingSquare || !move) {
        Refresh(true);
        Update();
        return;
    }

    std::string algebraic = mEngine->toAlgebraic(move);
    makeMove(move);
    mEngine->appendCheckOrMate(algebraic);

    ChessBoardEvent chessEvt(EVT_PIECE_MOVED, GetId(), algebraic);
    chessEvt.SetEventObject(this);
    ProcessWindowEvent(chessEvt);
}

void ChessBoardPanel::initPieces()
{
    wxMemoryInputStream whitePawn(bin2c_wP_png, 9333);
    wxMemoryInputStream blackPawn(bin2c_bP_png, 4983);
    wxMemoryInputStream whiteKnight(bin2c_wN_png, 14849);
    wxMemoryInputStream blackKnight(bin2c_bN_png, 11998);
    wxMemoryInputStream whiteBishop(bin2c_wB_png, 13094);
    wxMemoryInputStream blackBishop(bin2c_bB_png, 7843);
    wxMemoryInputStream whiteRook(bin2c_wR_png, 3866);
    wxMemoryInputStream blackRook(bin2c_bR_png, 3900);
    wxMemoryInputStream whiteQueen(bin2c_wQ_png, 19948);
    wxMemoryInputStream blackQueen(bin2c_bQ_png, 16749);
    wxMemoryInputStream whiteKing(bin2c_wK_png, 16332);
    wxMemoryInputStream blackKing(bin2c_bK_png, 19747);

    mPieces[0] = wxImage(whitePawn);
    mPieces[1] = wxImage(blackPawn);
    mPieces[2] = wxImage(whiteKnight);
    mPieces[3] = wxImage(blackKnight);
    mPieces[4] = wxImage(whiteBishop);
    mPieces[5] = wxImage(blackBishop);
    mPieces[6] = wxImage(whiteRook);
    mPieces[7] = wxImage(blackRook);
    mPieces[8] = wxImage(whiteQueen);
    mPieces[9] = wxImage(blackQueen);
    mPieces[10] = wxImage(whiteKing);
    mPieces[11] = wxImage(blackKing);
}

void ChessBoardPanel::resizePieces(std::array<ChessBoardPiece, 64>& board, int width, int height)
{
    for (int i = 0; i < 64; ++i) {
        board[i].scale(width, height);
    }
}

void ChessBoardPanel::setBoard()
{
    for (Square sq = Square::A1; sq < Square::Null; ++sq) {
        PieceSets piece = mEngine->getPiece(sq);
        if (piece != PieceSets::EmptySquares) {
            mBoard[to_int(sq)].setImage(mPieces[to_int(piece)]);
        }
    }
}

void ChessBoardPanel::makeMove(Cowbot::Move move)
{
    Square from = move.getStartingSquare();
    Square to = move.getEndingSquare();
    std::swap(mBoard[to_int(from)], mBoard[to_int(to)]);

    if (mEngine->isOccupied(to)) {
        mBoard[to_int(from)].setImage(wxImage());
    }

    MoveType type = move.getMoveType();
    if (type == MoveType::Castle) {
        switch (to) {
            case Square::G1:
                std::swap(mBoard[to_int(Square::F1)], mBoard[to_int(Square::H1)]);
                break;
            case Square::C1:
                std::swap(mBoard[to_int(Square::A1)], mBoard[to_int(Square::D1)]);
                break;
            case Square::G8:
                std::swap(mBoard[to_int(Square::F8)], mBoard[to_int(Square::H8)]);
                break;
            case Square::C8:
                std::swap(mBoard[to_int(Square::A8)], mBoard[to_int(Square::D8)]);
                break;
            default:
                break;
        }
    } else if (type == MoveType::Enpassant) {
        if (Cowbot::Utils::getBitboard(to) & Cowbot::Utils::SIXTH_RANK) {
            mBoard[to_int(Cowbot::Utils::southOne(to))].setImage(wxImage());
        } else {
            mBoard[to_int(Cowbot::Utils::northOne(to))].setImage(wxImage());
        }
    }

    mEngine->makeMove(move);
    Refresh(true);
    Update();
}

void ChessBoardPanel::OnComputerTurn()
{
    Cowbot::Move bestMove = mEngine->search();
    std::string algebraic = mEngine->toAlgebraic(bestMove);
    makeMove(bestMove);
    mEngine->appendCheckOrMate(algebraic);

    ChessBoardEvent chessEvt(EVT_PIECE_MOVED, GetId(), algebraic);
    chessEvt.SetEventObject(this);
    ProcessWindowEvent(chessEvt);
}