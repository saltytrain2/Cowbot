#pragma once

#include <wx/wx.h>

#include "types.h"
#include "CowbotGuiFrame.hpp"

class ChessBoardEvent : public wxCommandEvent
{
public:
    ChessBoardEvent(wxEventType type, int id, std::string move = std::string());
    std::string getString() const;

private:
    std::string mMove;
};

wxDEFINE_EVENT(EVT_PIECE_MOVED, ChessBoardEvent);
wxDEFINE_EVENT(EVT_COMPUTER_TURN, ChessBoardEvent);

ChessBoardEvent::ChessBoardEvent(wxEventType type, int id, std::string move)
    : wxCommandEvent(type, id),
      mMove(move)
{}

std::string ChessBoardEvent::getString() const
{
    return mMove;
}