#pragma once

#include <wx/wx.h>

#include "types.h"
#include "CowbotGuiFrame.hpp"

class ChessBoardEvent : public wxCommandEvent
{
public:
    ChessBoardEvent(wxEventType type, int id, std::string move = std::string(), Result res = Result::ONGOING);
    std::string getString() const;
    Result getResult() const;

private:
    std::string mMove;
    Result mResult;
};

wxDEFINE_EVENT(EVT_PIECE_MOVED, ChessBoardEvent);
wxDEFINE_EVENT(EVT_COMPUTER_TURN, ChessBoardEvent);

ChessBoardEvent::ChessBoardEvent(wxEventType type, int id, std::string move, Result res)
    : wxCommandEvent(type, id),
      mMove(move),
      mResult(res)
{}

std::string ChessBoardEvent::getString() const
{
    return mMove;
}

Result ChessBoardEvent::getResult() const
{
    return mResult;
}