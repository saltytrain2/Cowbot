#include "Move.h"

using namespace Cowbot;

Move::Move(std::string lan) : mMove(0)
{
    updateMove(lan);
}

Move::Move(Bitmove src) : mMove(src)
{
}

Move::Move(Square startSquare, Square endSquare, MoveType type, PromotionPiece piece) 
    : mMove(to_int(startSquare) | (to_int(endSquare) << 6) | (to_int(type) << 14) | ((to_int(piece) << 12)))
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
    Square startingSquare = Square(8 * (lan[1] - '1') + (lan[0] - 'a'));
    Square endingSquare = Square(8 * (lan[3] - '1') + (lan[2] - 'a'));

    newMove |= to_int(startingSquare);
    newMove |= to_int(endingSquare) << 6;
    
    mMove = newMove;
}

Square Move::getStartingSquare() const noexcept
{
    return Square(mMove & 0x3F);
}

Square Move::getEndingSquare() const noexcept
{
    return Square((mMove & 0xFC0) >> 6);
}

MoveType Move::getMoveType() const noexcept
{
    return MoveType((mMove & 0xC000) >> 14);
}

PromotionPiece Move::getPromotionPiece() const noexcept
{
    if (getMoveType() != MoveType::Promotion) {
        return PromotionPiece::Null;
    }

    return PromotionPiece((mMove & 0x3000) >> 12);
}   

std::string Move::toString() const
{
    std::string res;
    auto startNumber = to_int(getStartingSquare()) / 8;
    auto startLetter = to_int(getStartingSquare()) % 8;
    auto endNumber = to_int(getEndingSquare()) / 8;
    auto endLetter = to_int(getEndingSquare()) % 8;

    res += char(startLetter + 'a');
    res += char(startNumber + '1');
    res += char(endLetter + 'a');
    res += char(endNumber + '1');

    if (getMoveType() == MoveType::Promotion) {
        auto piece = to_int(getPromotionPiece());
        if (piece == 0) {
            res += 'n';
        } else if (piece == 1) {
            res += 'b';
        } else if (piece == 2) {
            res += 'r';
        } else {
            res += 'q';
        }
    }
    return res;
}

bool Move::operator==(const Move& rhs) const
{
    return mMove == rhs.mMove;
}

Move::operator bool() const
{
    return mMove;
}

bool Move::operator<(const Move& rhs) const
{
    return mMove < rhs.mMove;
}

Bitmove Move::getMove() const noexcept
{
    return mMove;
}