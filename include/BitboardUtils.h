#include "types.h"

namespace SquareBitboardUtils {
constexpr Bitboard NOT_A_FILE = 0xFEFEFEFEFEFEFEFE;
constexpr Bitboard NOT_H_FILE = 0x7F7F7F7F7F7F7F7F;
constexpr Bitboard EDGES = 0xFF818181818181FF;
constexpr Bitboard NOT_EDGES = ~EDGES;

Bitboard eastOne(Bitboard pieceLoc);
Bitboard westOne(Bitboard pieceLoc);
Bitboard northOne(Bitboard pieceLoc);
Bitboard southOne(Bitboard pieceLoc);
Bitboard northeastOne(Bitboard pieceLoc);
Bitboard northwestOne(Bitboard pieceLoc);
Bitboard southeastOne(Bitboard pieceLoc);
Bitboard southwestOne(Bitboard pieceLoc);
Bitboard getBitboard(Square sq);
uint8_t getTrailingZeros(Bitboard src);

// should only be called for initialization of masks
// assumes an empty occupancy grid
Bitboard slidingAttack(PieceSets piece, Square sq);

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
