#pragma once

#include "types.h"

namespace Utils {
constexpr Bitboard FIRST_RANK = 0xFF;
constexpr Bitboard SECOND_RANK = 0xFF00;
constexpr Bitboard THIRD_RANK = 0xFF0000;
constexpr Bitboard SIXTH_RANK = 0xFF0000000000;
constexpr Bitboard EIGHTH_RANK = 0xFF00000000000000;
constexpr Bitboard NOT_A_FILE = 0xFEFEFEFEFEFEFEFE;
constexpr Bitboard NOT_H_FILE = 0x7F7F7F7F7F7F7F7F;
constexpr Bitboard EDGES = 0xFF818181818181FF;
constexpr Bitboard NOT_EDGES = ~EDGES;

// Bitboard operations
Bitboard eastOne(Bitboard pieceLoc);
Bitboard westOne(Bitboard pieceLoc);
Bitboard northOne(Bitboard pieceLoc);
Bitboard southOne(Bitboard pieceLoc);
Bitboard northeastOne(Bitboard pieceLoc);
Bitboard northwestOne(Bitboard pieceLoc);
Bitboard southeastOne(Bitboard pieceLoc);
Bitboard southwestOne(Bitboard pieceLoc);
Bitboard getBitboard(Square sq);
Square popLSB(Bitboard& pieceLocs);
uint8_t getTrailingZeros(Bitboard src);


// Square operations
// function assumes only one hot location
Square getSquare(Bitboard loc);
Square eastOne(Square sq);
Square westOne(Square sq);
Square northOne(Square sq);
Square southOne(Square sq);
Square northeastOne(Square sq);
Square northwestOne(Square sq);
Square southeastOne(Square sq);
Square southwestOne(Square sq);
}
