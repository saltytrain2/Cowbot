#pragma once

#include <wx/wx.h>

#include <string>
#include <vector>
#include <memory>

#include "Attack.h"
#include "ChessBoard.h"
#include "MoveGen.h"
#include "Search.h"
#include "TT.h"
#include "MoveOrdering.h"
#include "FakeEval.h"

class ChessBoardEngine 
{
public:
    ChessBoardEngine();
    PieceSets getPiece(Square sq) const;
    Cowbot::Move isLegal(Square from, Square to);
    void makeMove(Cowbot::Move move);
    bool isOccupied(Square sq);
    std::string toAlgebraic(Cowbot::Move move);
    void appendCheckOrMate(std::string& move);
    Cowbot::Move search();
    uint64_t getHash() const;
    void reset();
    Color getTurn() const;
    

private:
    std::unique_ptr<Cowbot::Attack> mAttack;
    std::unique_ptr<Cowbot::ChessBoard> mBoard;
    std::unique_ptr<Cowbot::MoveGen> mMoveGen;
    std::unique_ptr<Cowbot::FakeEval> mFakeEval;
    std::unique_ptr<Cowbot::TT> mTT;
    std::unique_ptr<Cowbot::MoveOrdering> mMoveOrdering;
    std::unique_ptr<Cowbot::Search> mSearch;
    std::vector<Cowbot::Move> mLegalMoves;
};

ChessBoardEngine::ChessBoardEngine()
    : mAttack(std::make_unique<Cowbot::Attack>()),
      mBoard(std::make_unique<Cowbot::ChessBoard>(mAttack.get())),
      mMoveGen(std::make_unique<Cowbot::MoveGen>(mBoard.get(), mAttack.get())),
      mFakeEval(std::make_unique<Cowbot::FakeEval>()),
      mTT(std::make_unique<Cowbot::TT>()),
      mMoveOrdering(std::make_unique<Cowbot::MoveOrdering>(mBoard.get(), mFakeEval.get())),
      mSearch(std::make_unique<Cowbot::Search>(mBoard.get(), mMoveGen.get(), mFakeEval.get(), mTT.get(), mMoveOrdering.get()))
{
    mTT->setSize(1024);
    mLegalMoves = mMoveGen->generateLegalMoves(mBoard->getTurn());
}

PieceSets ChessBoardEngine::getPiece(Square sq) const
{
    return mBoard->getPiece(sq);
}

Cowbot::Move ChessBoardEngine::isLegal(Square from, Square to)
{
    for (auto move: mLegalMoves) {
        if (move.getStartingSquare() == from && move.getEndingSquare() == to) {
            return move;
        }
    }
    return Cowbot::Move();
}

void ChessBoardEngine::makeMove(Cowbot::Move move)
{
    mBoard->makeMove(move);
    mLegalMoves = mMoveGen->generateLegalMoves(mBoard->getTurn());
}

bool ChessBoardEngine::isOccupied(Square sq)
{
    return Cowbot::Utils::getBitboard(sq) & mBoard->getAllPieces();
}

std::string ChessBoardEngine::toAlgebraic(Cowbot::Move move)
{
    std::string algebraic;
    Square from = move.getStartingSquare();
    Square to = move.getEndingSquare();
    PieceSets movedPiece = mBoard->getPiece(from);
    bool isCapture = isOccupied(to) || move.getMoveType() == MoveType::Enpassant;

    if ((movedPiece == PieceSets::WhitePawns || movedPiece == PieceSets::BlackPawns) && isCapture) {
        algebraic += char('a' + Cowbot::Utils::getColumn(from));
    } else if (movedPiece == PieceSets::WhiteKnights || movedPiece == PieceSets::BlackKnights) {
        algebraic += 'N';
    } else if (movedPiece == PieceSets::WhiteBishops || movedPiece == PieceSets::BlackBishops) {
        algebraic += 'B';
    } else if (movedPiece == PieceSets::WhiteRooks || movedPiece == PieceSets::BlackRooks) {
        algebraic += 'R';
    } else if (movedPiece == PieceSets::WhiteQueens || movedPiece == PieceSets::BlackQueens) {
        algebraic += 'Q';
    } else if (move.getMoveType() == MoveType::Castle) {
        if (to == Square::G8 || to == Square::G1) {
            return "0-0";
        } else {
            return "0-0-0";
        }
    } else if (movedPiece == PieceSets::WhiteKing || movedPiece == PieceSets::BlackKing) {
        algebraic += 'K';
    }

    if (isCapture) {
        algebraic += 'x';
    }

    algebraic += char('a' + Cowbot::Utils::getColumn(to));
    algebraic += char('1' + Cowbot::Utils::getRank(to));

    return algebraic;
}

void ChessBoardEngine::appendCheckOrMate(std::string& move) 
{
    if (mBoard->isKingUnderAttack(mBoard->getTurn(), mBoard->getAllPieces())) {
        mLegalMoves.empty() ? move.append("#") : move.append("+");
    }
}

Cowbot::Move ChessBoardEngine::search()
{
    return mSearch->search(6).first;
}

uint64_t ChessBoardEngine::getHash() const
{
    return mBoard->getHash();
}

void ChessBoardEngine::reset()
{
    mBoard->reset();
    mLegalMoves = mMoveGen->generateLegalMoves(mBoard->getTurn());
}

Color ChessBoardEngine::getTurn() const
{
    return mBoard->getTurn();
}