#include "Attack.h"

Attack::Attack()
{
    Bitboard board = 1;
    for (Square i = a1; i < null; ++i, board <<= 1) {
        mKingAttacks[i] = kingAttacks(board);
        mKnightAttacks[i] = knightAttacks(board);
        mBishopMagics[i].mask = BitboardUtils::slidingAttack(whiteBishops, i);
        mBishopMagics[i].magic = BISHOP_MAGICS[i];
        mRookMagics[i].mask = BitboardUtils::slidingAttack(whiteRooks, i);
        mRookMagics[i].magic = ROOK_MAGICS[i];
    }
}

Bitboard Attack::kingAttacks(Bitboard kingLoc)
{
    Bitboard attacks = BitboardUtils::eastOne(kingLoc) | BitboardUtils::westOne(kingLoc);
    kingLoc |= attacks;
    attacks |= BitboardUtils::northOne(kingLoc) | BitboardUtils::southOne(kingLoc);
    return attacks;
}

Bitboard Attack::knightAttacks(Bitboard knightLoc)
{
    Bitboard east = BitboardUtils::eastOne(knightLoc);
    Bitboard west = BitboardUtils::westOne(knightLoc);
    Bitboard attacks = (east|west) << 16 | (east|west) >> 16;
    east = BitboardUtils::eastOne(east);
    west = BitboardUtils::westOne(west);
    attacks |= (east|west) << 8 | (east|west) >> 8;
    return attacks;
}

Bitboard Attack::getKingAttacks(Square sq)
{
    return mKingAttacks[sq];
}

Bitboard Attack::getKnightAttacks(Square sq)
{
    return mKnightAttacks[sq];
}

Bitboard Attack::getBishopAttacks(Bitboard blockers, Square sq)
{
    blockers &= mBishopMagics[sq].mask;
    blockers *= mBishopMagics[sq].magic;
    blockers >>= 64 - 9;
    return mBishopAttacks[sq][blockers];
}

Bitboard Attack::getRookAttacks(Bitboard blockers, Square sq)
{
    blockers &= mRookMagics[sq].mask;
    blockers *= mRookMagics[sq].magic;
    blockers >>= 64 - 12;
    return mRookAttacks[sq][blockers];
}

Bitboard Attack::getQueenAttacks(Bitboard blockers, Square sq)
{
    return getBishopAttacks(blockers, sq) | getRookAttacks(blockers, sq);
}