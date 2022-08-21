#pragma once

#include <wx/wx.h>
#include <wx/dataview.h>

#include "Move.h"
#include "ChessBoardPanel.hpp"
#include "ChessBoardEvent.hpp"
#include "MoveHistory.hpp"

enum 
{
    ID_WHITE = 1,
    ID_BLACK = 2,
    ID_FREE_PLAY = 3
};

enum GameState
{
    FREE_PLAY, PLAY_WHITE, PLAY_BLACK
};

class CowbotGuiFrame : public wxFrame
{
public:
    CowbotGuiFrame();
private:
    wxBoxSizer* mSizer;
    ChessBoardPanel* mChessBoard;
    MoveHistory* mMoveHistory;
    wxTextCtrl* mCommandBox;
    Color mTurn;
    uint16_t mFullMoveCounter;
    GameState mState;

    void OnEnter(wxCommandEvent& evt);
    void OnPieceDrag(ChessBoardEvent& evt);
    void OnComputerTurn(ChessBoardEvent&);
    void OnPlayAsWhite(wxCommandEvent&);
    void OnPlayAsBlack(wxCommandEvent&);
    void OnFreePlay(wxCommandEvent&);
};

wxDECLARE_EVENT(EVT_PIECE_MOVED, ChessBoardEvent);
wxDECLARE_EVENT(EVT_COMPUTER_TURN, ChessBoardEvent);

CowbotGuiFrame::CowbotGuiFrame()
    : wxFrame(NULL, wxID_ANY, "CowbotGUI", wxDefaultPosition, wxSize(1366, 768)),
      mSizer(new wxBoxSizer(wxHORIZONTAL)),
      mChessBoard(new ChessBoardPanel(this)),
      mMoveHistory(new MoveHistory(this)),
      mCommandBox(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER)),
      mTurn(Color::White),
      mFullMoveCounter(0),
      mState(GameState::FREE_PLAY)
{   
    // mMoveHistory->AppendTextColumn("White");
    // mMoveHistory->AppendTextColumn("Black");
    wxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    rightSizer->Add(mMoveHistory, wxSizerFlags(3).Expand().Border(wxALL, 5));
    rightSizer->Add(mCommandBox, wxSizerFlags(1).Expand().Border(wxALL, 5));


    mSizer->Add(mChessBoard, wxSizerFlags(3).Expand().Border(wxALL, 5));
    mSizer->Add(rightSizer, wxSizerFlags(2).Expand().Border(wxALL, 5));
    SetSizer(mSizer);
    // mSizer->Show(mChessBoard);
    // mSizer->Show(mMoveHistory);

    wxMenu *menuNewGame = new wxMenu;
    menuNewGame->Append(ID_WHITE, "&White", "Play Computer as White");
    menuNewGame->AppendSeparator();
    menuNewGame->Append(ID_BLACK, "&Black", "Play Computer as Black");
    menuNewGame->AppendSeparator();
    menuNewGame->Append(ID_FREE_PLAY, "&Free Play", "No Computer Opponent");
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuNewGame, "&New Game");
    menuBar->Append(menuHelp, "&Help");
 
    wxFrameBase::SetMenuBar( menuBar );
 
    CreateStatusBar();
 
    Bind(wxEVT_MENU, &CowbotGuiFrame::OnPlayAsWhite, this, ID_WHITE);
    Bind(wxEVT_MENU, &CowbotGuiFrame::OnPlayAsBlack, this, ID_BLACK);
    Bind(wxEVT_MENU, &CowbotGuiFrame::OnFreePlay, this, ID_FREE_PLAY);
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxMessageBox("This is a custom GUI for the Cowbot Chess Engine",
                 "About CowbotGUI", wxOK | wxICON_INFORMATION);}, wxID_ABOUT);
    Bind(wxEVT_MENU, [this](wxCommandEvent&) { Close(true); }, wxID_EXIT);
    Bind(wxEVT_TEXT_ENTER, &CowbotGuiFrame::OnEnter, this, wxID_ANY);
    Bind(EVT_PIECE_MOVED, &CowbotGuiFrame::OnPieceDrag, this);
    Bind(EVT_COMPUTER_TURN, &CowbotGuiFrame::OnComputerTurn, this);
}

void CowbotGuiFrame::OnEnter(wxCommandEvent& evt) 
{
    //mMoveHistory->Append(mCommandBox->GetValue());
    mCommandBox->Clear();
    mCommandBox->SetValue(wxGetCwd());
    evt.Skip();
}

void CowbotGuiFrame::OnPieceDrag(ChessBoardEvent& evt)
{
    if (mTurn == Color::White) {
        wxString num;
        mMoveHistory->InsertItem(mFullMoveCounter, num << mFullMoveCounter + 1);
    }
    mMoveHistory->SetItem(mFullMoveCounter, to_int(mTurn) + 1, evt.getString());
    mMoveHistory->Update();
    mTurn = !mTurn;
    if (mTurn == Color::White) {
        ++mFullMoveCounter;
    }

    if ((mState == GameState::PLAY_WHITE && mTurn == Color::Black) || (mState == GameState::PLAY_BLACK && mTurn == Color::White)) {
        ChessBoardEvent chessEvt(EVT_COMPUTER_TURN, GetId());
        chessEvt.SetEventObject(this);
        ProcessWindowEvent(chessEvt);
    }
}

void CowbotGuiFrame::OnComputerTurn(ChessBoardEvent&)
{
    mChessBoard->OnComputerTurn();
}

void CowbotGuiFrame::OnFreePlay(wxCommandEvent&)
{
    mState = GameState::FREE_PLAY;
    mChessBoard->reset();
    mChessBoard->setPerspective(Color::White);
    mMoveHistory->clear();
    mTurn = Color::White;
    mFullMoveCounter = 0;
}

void CowbotGuiFrame::OnPlayAsWhite(wxCommandEvent&)
{
    mState = GameState::PLAY_WHITE;
    mChessBoard->reset();
    mChessBoard->setPerspective(Color::White);
    mMoveHistory->clear();
    mTurn = Color::White;
    mFullMoveCounter = 0;
}

void CowbotGuiFrame::OnPlayAsBlack(wxCommandEvent&)
{
    mState = GameState::PLAY_BLACK;
    mChessBoard->reset();
    mChessBoard->setPerspective(Color::Black);
    mMoveHistory->clear();
    mTurn = Color::White;
    mFullMoveCounter = 0;

    // have computer play first move
    ChessBoardEvent chessEvt(EVT_COMPUTER_TURN, GetId());
    chessEvt.SetEventObject(this);
    ProcessWindowEvent(chessEvt);
}