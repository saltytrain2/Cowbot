#pragma once

#include "types.h"

namespace Utils {
constexpr Bitboard FIRST_RANK = 0xFF;
constexpr Bitboard SECOND_RANK = FIRST_RANK << 8;
constexpr Bitboard THIRD_RANK = SECOND_RANK << 8;
constexpr Bitboard FOURTH_RANK = THIRD_RANK << 8;
constexpr Bitboard FIFTH_RANK = FOURTH_RANK << 8;
constexpr Bitboard SIXTH_RANK = FIFTH_RANK << 8;
constexpr Bitboard SEVENTH_RANK = SIXTH_RANK << 8;
constexpr Bitboard EIGHTH_RANK = SEVENTH_RANK << 8;
constexpr Bitboard A_FILE = 0x101010101010101;
constexpr Bitboard B_FILE = A_FILE << 1;
constexpr Bitboard C_FILE = B_FILE << 1;
constexpr Bitboard D_FILE = C_FILE << 1;
constexpr Bitboard E_FILE = D_FILE << 1;
constexpr Bitboard F_FILE = E_FILE << 1;
constexpr Bitboard G_FILE = F_FILE << 1;
constexpr Bitboard H_FILE = G_FILE << 1;
constexpr Bitboard NOT_A_FILE = ~A_FILE;
constexpr Bitboard NOT_H_FILE = ~H_FILE;
constexpr Bitboard EDGES = A_FILE | H_FILE | FIRST_RANK | EIGHTH_RANK;
constexpr Bitboard NOT_EDGES = ~EDGES;
constexpr Bitboard WHITE_DIAGONAL = 0x102040810204080;
constexpr Bitboard BLACK_DIAGONAL = 0x8040201008040201;

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
bool isOneHot(Bitboard loc);


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
