#include "Move.h"

Move::Move(std::string lan) : mMove(0), mCapturedPiece(emptySquares)
{
    updateMove(lan);
}

Move::Move(Bitmove src) : mMove(src)
{
}

Move::Move(Square startSquare, Square endSquare) : mMove(startSquare | (endSquare << 6)), mCapturedPiece(emptySquares)
{
}

void Move::updateMove(std::string lan)
{
    Bitmove newMove = 0;
    // setting special move flags
    if (lan.length() == 5) {
        switch (lan[4]) {
            case 'n':
                break;
            case 'b':
                newMove |= 1 << 12;
                break;
            case 'r':
                newMove |= 1 << 13;
                break;
            case 'q':
                newMove |= (1 << 12) | (1 << 13);
        }
        newMove |= 1 << 15;
    }   
    //uint16_t startingSquare = 8 * (lan[1]-'1') + (lan[0] - 'a');
    //uint16_t endingSquare = 8 * (lan[3]-'1') + (lan[2] - 'a');
    Square startingSquare = Square(8 * (lan[1] - '1') + (lan[0] - 'a'));
    Square endingSquare = Square(8 * (lan[3] - '1') + (lan[2] - 'a'));

    newMove |= startingSquare;
    newMove |= endingSquare << 6;
    
    mMove = newMove;
}

Bitmove Move::getMove() const noexcept
{
    return mMove;
}

void Move::setCapturedPiece(PieceSets piece) noexcept
{
    mCapturedPiece = piece;
}

PieceSets Move::getCapturedPiece() const
{
    return mCapturedPiece;
}