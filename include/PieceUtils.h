#pragma once

#include "types.h"
const Bitboard NOT_A_FILE = 0xFEFEFEFEFEFEFEFE;
const Bitboard NOT_H_FILE = 0x7F7F7F7F7F7F7F7F;

namespace PieceUtils {
    AttackBoard arrKingAttacks[64];
    AttackBoard arrKnightAttacks[64];

    // surrounding squares, also used for King attacks
    AttackBoard eastOne(Bitboard pieceLoc);
    AttackBoard westOne(Bitboard pieceLoc);
    AttackBoard northOne(Bitboard pieceLoc);
    AttackBoard southOne(Bitboard pieceLoc);
    AttackBoard northeastOne(Bitboard pieceLoc);
    AttackBoard northwestOne(Bitboard pieceLoc);
    AttackBoard southeastOne(Bitboard pieceLoc);
    AttackBoard southwestOne(Bitboard pieceLoc);

    AttackBoard kingAttacks(Bitboard kingLoc);
    AttackBoard knightAttacks(Bitboard knightLoc);
}