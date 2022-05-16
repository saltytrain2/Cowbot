#include "BitboardUtils.h"

Bitboard SquareBitboardUtils::eastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) << 1;
}

Bitboard SquareBitboardUtils::westOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) >> 1;
}

Bitboard SquareBitboardUtils::northOne(Bitboard pieceLoc)
{
    return pieceLoc >> 8;
}

Bitboard SquareBitboardUtils::southOne(Bitboard pieceLoc)
{
    return pieceLoc << 8;
}

Bitboard SquareBitboardUtils::northeastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) << 9;
}

Bitboard SquareBitboardUtils::northwestOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) << 7;
}

Bitboard SquareBitboardUtils::southwestOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) >> 9;
}

Bitboard SquareBitboardUtils::southeastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) >> 7;
}

Bitboard SquareBitboardUtils::getBitboard(Square sq)
{
    return Bitboard(1) << sq;
}

Bitboard SquareBitboardUtils::slidingMasks(PieceSets piece, Square sq)
{
    Bitboard attacks = 0;

    if (piece == whiteBishops) {
        for (Square i = northeastOne(sq); i != null; i = northeastOne(i)) {
            attacks |= Bitboard(1) << i;
        }
        for (Square i = northwestOne(sq); i != null; i = northwestOne(i)) {
            attacks |= Bitboard(1) << i;
        }
        for (Square i = southeastOne(sq); i != null; i = southeastOne(i)) {
            attacks |= Bitboard(1) << i;
        }
        for (Square i = southwestOne(sq); i != null; i = southwestOne(i)) {
            attacks |= Bitboard(1) << i;
        }
        attacks &= NOT_EDGES;
    } else if (piece == whiteRooks) {
        Square i = northOne(sq);
        while (i != null && northOne(i) != null) {
            attacks |= Bitboard(1) << i;
            i = northOne(i);
        }
        i = southOne(sq);
        while (i != null && southOne(i) != null) {
            attacks |= Bitboard(1) << i;
            i = southOne(i);
        }
        i = westOne(sq);
        while (i != null && westOne(i) != null) {
            attacks |= Bitboard(1) << i;
            i = westOne(i);
        }
        i = eastOne(sq);
        while (i != null && eastOne(i) != null) {
            attacks |= Bitboard(1) << i;
            i = eastOne(i);
        }
    }
    return attacks;
}

uint8_t SquareBitboardUtils::getTrailingZeros(Bitboard src)
{
    uint8_t count = 0;
    while (!(src & 1)) {
        ++count;
        src >>= 1;
    }
    return count;
}

Square SquareBitboardUtils::getSquare(Bitboard loc)
{
    if (loc == 0) {
        return null;
    }

    uint8_t count = 0;
    while (loc) {
        if (loc & 1) {
            break;
        }
        ++count;
        loc >>= 1;
    }
    return Square(count);
}

Square SquareBitboardUtils::eastOne(Square sq)
{
    return getSquare(eastOne(getBitboard(sq)));
}

Square SquareBitboardUtils::westOne(Square sq)
{
    return getSquare(westOne(getBitboard(sq)));
}

Square SquareBitboardUtils::northOne(Square sq)
{
    return getSquare(northOne(getBitboard(sq)));
}

Square SquareBitboardUtils::southOne(Square sq)
{
    return getSquare(southOne(getBitboard(sq)));
}

Square SquareBitboardUtils::northeastOne(Square sq)
{
    return getSquare(northeastOne(getBitboard(sq)));
}

Square SquareBitboardUtils::northwestOne(Square sq)
{
    return getSquare(northwestOne(getBitboard(sq)));
}

Square SquareBitboardUtils::southeastOne(Square sq)
{
    return getSquare(southeastOne(getBitboard(sq)));
}

Square SquareBitboardUtils::southwestOne(Square sq)
{
    return getSquare(southwestOne(getBitboard(sq)));
}