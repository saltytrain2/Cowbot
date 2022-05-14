#include "types.h"

namespace BitboardUtils {
constexpr Bitboard NOT_A_FILE = 0xFEFEFEFEFEFEFEFE;
constexpr Bitboard NOT_H_FILE = 0x7F7F7F7F7F7F7F7F;
constexpr Bitboard EDGES = 0xFF818181818181FF;

Bitboard eastOne(Bitboard pieceLoc);
Bitboard westOne(Bitboard pieceLoc);
Bitboard northOne(Bitboard pieceLoc);
Bitboard southOne(Bitboard pieceLoc);
Bitboard northeastOne(Bitboard pieceLoc);
Bitboard northwestOne(Bitboard pieceLoc);
Bitboard southeastOne(Bitboard pieceLoc);
Bitboard southwestOne(Bitboard pieceLoc);
Bitboard squareToBitboard(Square sq);

// should only be called for initialization of masks
// assumes an empty occupancy grid
Bitboard slidingAttack(PieceSets piece, Square sq);
}