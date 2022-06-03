#pragma once

#include "types.h"
#include "Utils.h"

class Attack
{
public:
    Attack();
    ~Attack() = default;
    Attack(const Attack& rhs) = default;
    Attack& operator=(const Attack& rhs) = default;

    Bitboard getPawnAttacks(Square sq, Color color);
    Bitboard getKingAttacks(Square sq);
    Bitboard getKnightAttacks(Square sq);
    Bitboard getBishopAttacks(Square sq, Bitboard blockers);
    Bitboard getRookAttacks(Square sq, Bitboard blockers);
    Bitboard getQueenAttacks(Square sq, Bitboard blockers);

    Bitboard inBetween(Square from, Square to);
    Bitboard inLine(Square from, Square to);

private:
    // masks for each square on the board
    Bitboard mBishopMasks[64];
    Bitboard mRookMasks[64];

    // Lookup tables of all attacks possible by each piece
    // Bishop and Rook Lookup Tables are done using Basic Magic Bitboards
    // Queen Lookup Table is the xor of Bishop and Rook
    Bitboard mPawnAttacks[2][64];
    Bitboard mKingAttacks[64];
    Bitboard mKnightAttacks[64];
    Bitboard mBishopAttacks[64][512];
    Bitboard mRookAttacks[64][4096];

    Bitboard mBetweenRectangular[64][64];
    Bitboard mLined[64][64];

    Bitboard getMaskedBlockers(Bitboard mask, uint16_t index);
    Bitboard getSlidingMasks(PieceSets piece, Square sq);

    Bitboard calcKingAttacks(Bitboard kingLoc);
    Bitboard calcKnightAttacks(Bitboard knightLoc);
    Bitboard calcBishopAttacks(Square sq, Bitboard blockers);
    Bitboard calcRookAttacks(Square sq, Bitboard blockers);

    void initPawnAttacks();
    void initKingAttacks();
    void initKnightAttacks();
    void initBishopAttacks();
    void initRookAttacks();

    void initBetweenTable();
    void initLinedTable();
};