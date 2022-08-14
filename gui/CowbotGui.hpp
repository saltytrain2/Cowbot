#pragma once

#include <wx/wx.h>

#include "CowbotGuiFrame.hpp"

class CowbotGui : public wxApp
{
public:
    virtual bool OnInit();
};

bool CowbotGui::OnInit()
{
    CowbotGuiFrame* frame = new CowbotGuiFrame();
    frame->Show();
    return true;
}