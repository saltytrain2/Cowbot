#pragma once

#include <wx/wx.h>

#include "types.h"

class ChessBoardPiece : public wxObject
{
public:
    ChessBoardPiece(PieceSets piece = PieceSets::EmptySquares, bool show = true);
    void setShow(bool show);
    bool getShow() const;
    void setPiece(PieceSets piece);
    PieceSets getPiece() const;

private:
    PieceSets mPiece;
    bool mShow;
};


ChessBoardPiece::ChessBoardPiece(PieceSets piece, bool show)
    : mPiece(piece),
      mShow(show)
{}

void ChessBoardPiece::setShow(bool show)
{
    mShow = show;
}

bool ChessBoardPiece::getShow() const
{
    return mShow;
}

void ChessBoardPiece::setPiece(PieceSets piece)
{
    mPiece = piece;
}

PieceSets ChessBoardPiece::getPiece() const
{
    return mPiece;
}