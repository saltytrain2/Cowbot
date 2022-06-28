#pragma once

#include "types.h"
#include "Utils.h"

class Attack
{
public:
    Attack() = default;
    ~Attack() = default;
    Attack(const Attack& rhs) = default;
    Attack& operator=(const Attack& rhs) = default;

    void initTables();

    Bitboard getPawnAttacks(Square sq, Color color) const;
    Bitboard getKingAttacks(Square sq) const;
    Bitboard getKnightAttacks(Square sq) const;
    Bitboard getBishopAttacks(Square sq, Bitboard blockers) const;
    Bitboard getRookAttacks(Square sq, Bitboard blockers) const;
    Bitboard getQueenAttacks(Square sq, Bitboard blockers) const;

    Bitboard inBetween(Square from, Square to) const;
    Bitboard inLine(Square from, Square to) const;

private:
    // masks for each square on the board
    static Bitboard mBishopMasks[64];
    static Bitboard mRookMasks[64];

    // Lookup tables of all attacks possible by each piece
    // Bishop and Rook Lookup Tables are done using Basic Magic Bitboards
    // Queen Lookup Table is the xor of Bishop and Rook
    static Bitboard mPawnAttacks[2][64];
    static Bitboard mKingAttacks[64];
    static Bitboard mKnightAttacks[64];
    static Bitboard mBishopAttacks[64][512];
    static Bitboard mRookAttacks[64][4096];

    static Bitboard mBetweenRectangular[64][64];
    static Bitboard mLined[64][64];

    Bitboard getMaskedBlockers(Bitboard mask, uint16_t index) const;
    Bitboard getSlidingMasks(PieceSets piece, Square sq) const;

    Bitboard calcKingAttacks(Bitboard kingLoc) const;
    Bitboard calcKnightAttacks(Bitboard knightLoc) const;
    Bitboard calcBishopAttacks(Square sq, Bitboard blockers) const;
    Bitboard calcRookAttacks(Square sq, Bitboard blockers) const;

    void initPawnAttacks();
    void initKingAttacks();
    void initKnightAttacks();
    void initBishopAttacks();
    void initRookAttacks();

    void initBetweenTable();
    void initLinedTable();
};