#include "Utils.h"

// debruijn lookup for determining index of LSB
constexpr Square SQUARE_LOOKUP[64] = {
    a1, h6, b1, a8, a7, d4, c1, e8,
    b8, b7, b6, f5, e4, a3, d1, f8,
    g7, c8, d5, e7, c7, c6, f3, e6,
    g5, a5, f4, h3, b3, d2, e1, g8,
    g6, h7, c4, d8, a6, e5, h2, f7,
    c5, d7, e3, d6, h4, g3, c2, f6,
    b4, h5, g2, b5, d3, g4, b2, a4,
    f2, c3, a2, e2, h1, g1, f1, h8
};
constexpr Bitboard DEBRUIJN64 = 0x03F79D71B4CB0A89;

Bitboard Utils::eastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) << 1;
}

Bitboard Utils::westOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) >> 1;
}

Bitboard Utils::northOne(Bitboard pieceLoc)
{
    return pieceLoc << 8;
}

Bitboard Utils::southOne(Bitboard pieceLoc)
{
    return pieceLoc >> 8;
}

Bitboard Utils::northeastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) << 9;
}

Bitboard Utils::northwestOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) << 7;
}

Bitboard Utils::southwestOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) >> 9;
}

Bitboard Utils::southeastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) >> 7;
}

Bitboard Utils::getBitboard(Square sq)
{
    return Bitboard(1) << sq;
}

uint8_t Utils::getTrailingZeros(Bitboard src)
{
    return popLSB(src);
}

Square Utils::getSquare(Bitboard loc)
{
    return loc == 0 ? null : SQUARE_LOOKUP[((loc ^ (loc - 1)) * DEBRUIJN64) >> 58];
}

Square Utils::eastOne(Square sq)
{
    return getSquare(eastOne(getBitboard(sq)));
}

Square Utils::westOne(Square sq)
{
    return getSquare(westOne(getBitboard(sq)));
}

Square Utils::northOne(Square sq)
{
    return getSquare(northOne(getBitboard(sq)));
}

Square Utils::southOne(Square sq)
{
    return getSquare(southOne(getBitboard(sq)));
}

Square Utils::northeastOne(Square sq)
{
    return getSquare(northeastOne(getBitboard(sq)));
}

Square Utils::northwestOne(Square sq)
{
    return getSquare(northwestOne(getBitboard(sq)));
}

Square Utils::southeastOne(Square sq)
{
    return getSquare(southeastOne(getBitboard(sq)));
}

Square Utils::southwestOne(Square sq)
{
    return getSquare(southwestOne(getBitboard(sq)));
}

Square Utils::popLSB(Bitboard& pieceLocs)
{
    uint8_t index = ((pieceLocs ^ (pieceLocs - 1)) * DEBRUIJN64) >> 58;
    pieceLocs &= pieceLocs - 1;
    return SQUARE_LOOKUP[index];
}
