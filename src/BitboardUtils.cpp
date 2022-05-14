#include "BitboardUtils.h"

Bitboard BitboardUtils::eastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) << 1;
}

Bitboard BitboardUtils::westOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) >> 1;
}

Bitboard BitboardUtils::northOne(Bitboard pieceLoc)
{
    return pieceLoc >> 8;
}

Bitboard BitboardUtils::southOne(Bitboard pieceLoc)
{
    return pieceLoc << 8;
}

Bitboard BitboardUtils::northeastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) << 9;
}

Bitboard BitboardUtils::northwestOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) << 7;
}

Bitboard BitboardUtils::southwestOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) >> 9;
}

Bitboard BitboardUtils::southeastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) >> 7;
}

Bitboard BitboardUtils::squareToBitboard(Square sq)
{
    return Bitboard(1) << sq;
}

Bitboard BitboardUtils::slidingAttack(PieceSets piece, Square sq)
{
    Bitboard attacks = 0;

    if (piece == whiteBishops) {
        Square curSquare = sq;
        while (northeastOne(squareToBitboard(curSquare))) {
            attacks |= curSquare += 9;
        }
        curSquare = sq;
        while (southeastOne(squareToBitboard(curSquare))) {
            attacks |= curSquare -= 7;
        }
        curSquare = sq;
        while (northwestOne(squareToBitboard(curSquare))) {
            attacks |= curSquare += 7;
        }
        curSquare = sq;
        while (southwestOne(squareToBitboard(curSquare))) {
            attacks |= curSquare -= 9;
        }
    } else if (piece == whiteRooks) {
        Square curSquare = sq;
        while (northOne(squareToBitboard(curSquare))) {
            attacks |= curSquare += 8;
        }
        curSquare = sq;
        while (southOne(squareToBitboard(curSquare))) {
            attacks |= curSquare -= 8;
        }
        curSquare = sq;
        while (westOne(squareToBitboard(curSquare))) {
            attacks |= curSquare -= 1;
        }
        curSquare = sq;
        while (eastOne(squareToBitboard(curSquare))) {
            attacks |= curSquare += 1;
        }
    }

    return attacks;
}