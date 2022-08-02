#include "Utils.h"

// debruijn lookup for determining index of LSB
static constexpr Square SQUARE_LOOKUP[64] = {
    Square::A1, Square::H6, Square::B1, Square::A8, Square::A7, Square::D4, 
    Square::C1, Square::E8, Square::B8, Square::B7, Square::B6, Square::F5, 
    Square::E4, Square::A3, Square::D1, Square::F8, Square::G7, Square::C8, 
    Square::D5, Square::E7, Square::C7, Square::C6, Square::F3, Square::E6,
    Square::G5, Square::A5, Square::F4, Square::H3, Square::B3, Square::D2, 
    Square::E1, Square::G8, Square::G6, Square::H7, Square::C4, Square::D8, 
    Square::A6, Square::E5, Square::H2, Square::F7, Square::C5, Square::D7, 
    Square::E3, Square::D6, Square::H4, Square::G3, Square::C2, Square::F6,
    Square::B4, Square::H5, Square::G2, Square::B5, Square::D3, Square::G4, 
    Square::B2, Square::A4, Square::F2, Square::C3, Square::A2, Square::E2, 
    Square::H1, Square::G1, Square::F1, Square::H8
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
    return Bitboard(1) << to_int(sq);
}

uint8_t Utils::getTrailingZeros(Bitboard src)
{
    return to_int(popLSB(src));
}

Square Utils::getSquare(Bitboard loc)
{
    return loc == 0 ? Square::Null : SQUARE_LOOKUP[((loc ^ (loc - 1)) * DEBRUIJN64) >> 58];
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

bool Utils::isOneHot(Bitboard loc)
{
    return loc && !(loc & (loc - 1));
}

Square Utils::flipSquare(Square sq)
{
    return Square(to_int(sq) ^ 0b111000);
}

uint8_t Utils::getColumn(Square sq)
{
    return to_int(sq) & 0b111;
}