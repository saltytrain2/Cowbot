#include "Attack.h"
#include <cassert>
#include <iostream>

Attack::Attack()
{
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 512; ++j) {
            mBishopAttacks[i][j] = 69;
        }
    }
    initKingAttacks();
    initKnightAttacks();
    initBishopAttacks();
    initRookAttacks();
}

Bitboard Attack::getMaskedBlockers(Bitboard mask, uint16_t index)
{
    Bitboard blockers = 0;

    while (index) {
        uint8_t shift = SquareBitboardUtils::getTrailingZeros(mask);
        if (index & 1) {
            blockers |= Bitboard(1) << shift;
        }
        mask &= (mask - 1);
        index >>= 1;
    }
    return blockers;
}

void Attack::initKingAttacks()
{
    Bitboard board = 1;

    for (Square i = a1; i < null; ++i, board <<= 1) {
        mKingAttacks[i] = calcKingAttacks(board);
    }
}

void Attack::initKnightAttacks()
{
    Bitboard board = 1;

    for (Square i = a1; i < null; ++i, board <<= 1) {
        mKnightAttacks[i] = calcKnightAttacks(board);
    }
}

void Attack::initBishopAttacks()
{
    for (Square i = a1; i < null; ++i) {
        Bitboard mask = SquareBitboardUtils::slidingMasks(whiteBishops, i);
        uint16_t permutations = uint16_t(1) << BISHOP_SHIFTS[i];

        for (uint16_t j = 0; j < permutations; ++j) {
            Bitboard blockers = getMaskedBlockers(mask, j);
            Bitboard attacks = 0;
            
            for (Square k = SquareBitboardUtils::northeastOne(i); k != null; k = SquareBitboardUtils::northeastOne(k)) {
                attacks |= Bitboard(1) << k;
                if (blockers & Bitboard(1) << k) {
                    break;
                }
            }
            for (Square k = SquareBitboardUtils::northwestOne(i); k != null; k = SquareBitboardUtils::northwestOne(k)) {
                attacks |= Bitboard(1) << k;
                if (blockers & Bitboard(1) << k) {
                    break;
                }
            }
            for (Square k = SquareBitboardUtils::southeastOne(i); k != null; k = SquareBitboardUtils::southeastOne(k)) {
                attacks |= Bitboard(1) << k;
                if (blockers & Bitboard(1) << k) {
                    break;
                }
            }
            for (Square k = SquareBitboardUtils::southwestOne(i); k != null; k = SquareBitboardUtils::southwestOne(k)) {
                attacks |= Bitboard(1) << k;
                if (blockers & Bitboard(1) << k) {
                    break;
                }
            }
            uint16_t magicIndex = (blockers * BISHOP_MAGICS[i]) >> (64 - BISHOP_SHIFTS[i]);
            mBishopAttacks[i][magicIndex] = attacks;
        }
        mBishopMasks[i] = mask;
    }
}

void Attack::initRookAttacks()
{
    for (Square i = a1; i < null; ++i) {
        Bitboard mask = SquareBitboardUtils::slidingMasks(whiteRooks, i);
        uint16_t permutations = 1 << ROOK_SHIFTS[i];

        for (uint16_t j = 0; j < permutations; ++j) {
            Bitboard blockers = getMaskedBlockers(mask, j);
            Bitboard attacks = 0;
            
            for (Square k = SquareBitboardUtils::northOne(i); k != null; k = SquareBitboardUtils::northOne(k)) {
                attacks |= Bitboard(1) << k;
                if (blockers & Bitboard(1) << k) {
                    break;
                }
            }
            for (Square k = SquareBitboardUtils::westOne(i); k != null; k = SquareBitboardUtils::westOne(k)) {
                attacks |= Bitboard(1) << k;
                if (blockers & Bitboard(1) << k) {
                    break;
                }
            }
            for (Square k = SquareBitboardUtils::eastOne(i); k != null; k = SquareBitboardUtils::eastOne(k)) {
                attacks |= Bitboard(1) << k;
                if (blockers & Bitboard(1) << k) {
                    break;
                }
            }
            for (Square k = SquareBitboardUtils::southOne(i); k != null; k = SquareBitboardUtils::southOne(k)) {
                attacks |= Bitboard(1) << k;
                if (blockers & Bitboard(1) << k) {
                    break;
                }
            }
            uint16_t magicIndex = (blockers * ROOK_MAGICS[i]) >> (64 - ROOK_SHIFTS[i]);
            mRookAttacks[i][magicIndex] = attacks;
        }
        mRookMasks[i] = mask;
    }
}

Bitboard Attack::getKingAttacks(Square sq)
{
    return mKingAttacks[sq];
}

Bitboard Attack::getKnightAttacks(Square sq)
{
    return mKnightAttacks[sq];
}

Bitboard Attack::getBishopAttacks(Square sq, Bitboard blockers)
{
    blockers &= mBishopMasks[sq];
    blockers *= BISHOP_MAGICS[sq];
    blockers >>= 64 - BISHOP_SHIFTS[sq];
    return mBishopAttacks[sq][blockers];
}

Bitboard Attack::getRookAttacks(Square sq, Bitboard blockers)
{
    blockers &= mRookMasks[sq];
    blockers *= ROOK_MAGICS[sq];
    blockers >>= 64 - ROOK_SHIFTS[sq];
    return mRookAttacks[sq][blockers];
}

Bitboard Attack::getQueenAttacks(Square sq, Bitboard blockers)
{
    return getBishopAttacks(sq, blockers) | getRookAttacks(sq, blockers);
}

Bitboard Attack::calcKingAttacks(Bitboard kingLoc)
{
    Bitboard attacks = SquareBitboardUtils::eastOne(kingLoc) | SquareBitboardUtils::westOne(kingLoc);
    kingLoc |= attacks;
    attacks |= SquareBitboardUtils::northOne(kingLoc) | SquareBitboardUtils::southOne(kingLoc);
    return attacks;
}

Bitboard Attack::calcKnightAttacks(Bitboard knightLoc)
{
    Bitboard east = SquareBitboardUtils::eastOne(knightLoc);
    Bitboard west = SquareBitboardUtils::westOne(knightLoc);
    Bitboard attacks = (east|west) << 16 | (east|west) >> 16;
    east = SquareBitboardUtils::eastOne(east);
    west = SquareBitboardUtils::westOne(west);
    attacks |= (east|west) << 8 | (east|west) >> 8;
    return attacks;
}   

Bitboard Attack::getBishopMasks(Square sq)
{
    return mBishopMasks[sq];
}

Bitboard Attack::getRookMasks(Square sq)
{
    return mRookMasks[sq];
}