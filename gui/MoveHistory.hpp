#pragma once

#include <wx/wx.h>

class MoveHistory : public wxListView
{
public:
    MoveHistory(wxWindow* parent);

};

MoveHistory::MoveHistory(wxWindow* parent)
    : wxListView(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL)
{
    AppendColumn("Move No");
    AppendColumn("White");
    AppendColumn("Black");
}