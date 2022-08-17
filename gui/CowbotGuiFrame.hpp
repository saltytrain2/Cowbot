#pragma once

#include <wx/wx.h>
#include <wx/dataview.h>

#include "Move.h"
#include "ChessBoardPanel.hpp"
#include "ChessBoardEvent.hpp"
#include "MoveHistory.hpp"

enum 
{
    ID_Hello = 1
};

class CowbotGuiFrame : public wxFrame
{
public:
    CowbotGuiFrame();
private:
    wxBoxSizer* mSizer;
    ChessBoardPanel* mChessBoard;
    wxListView* mMoveHistory;
    wxTextCtrl* mCommandBox;
    Color mTurn;
    uint16_t mFullMoveCounter;

    void OnEnter(wxCommandEvent& evt);
    void OnPieceDrag(ChessBoardEvent& evt);
    void OnComputerTurn(ChessBoardEvent&);
};

wxDECLARE_EVENT(EVT_PIECE_MOVED, ChessBoardEvent);
wxDECLARE_EVENT(EVT_COMPUTER_TURN, ChessBoardEvent);

CowbotGuiFrame::CowbotGuiFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World", wxDefaultPosition, wxSize(1366, 768)),
      mSizer(new wxBoxSizer(wxHORIZONTAL)),
      mChessBoard(new ChessBoardPanel(this)),
      mMoveHistory(new MoveHistory(this)),
      mCommandBox(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER)),
      mTurn(Color::White),
      mFullMoveCounter(0)
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

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    wxFrameBase::SetMenuBar( menuBar );
 
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
 
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLogMessage("Hello world from wxWidgets!");}, ID_Hello);
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);}, wxID_ABOUT);
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

    if (mTurn == Color::Black) {
        ChessBoardEvent chessEvt(EVT_COMPUTER_TURN, GetId());
        chessEvt.SetEventObject(this);
        ProcessWindowEvent(chessEvt);
    }
}

void CowbotGuiFrame::OnComputerTurn(ChessBoardEvent&)
{
    mChessBoard->OnComputerTurn();
}
