#include "Attack.h"
#include <cassert>
#include <iostream>

using namespace Cowbot;
// magics taken from shallow blue https://github.com/GunshipPenguin/shallow-blue
// TODO create custom magics
static constexpr uint64_t ROOK_MAGICS[64] = {
    0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL,
    0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
    0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL,
    0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL,
    0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL,
    0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL,
    0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL,
    0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
    0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
    0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL,
    0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL,
    0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL,
    0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL
};

static constexpr uint64_t BISHOP_MAGICS[64] = {
    0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
    0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL,
    0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
    0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL,
    0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL,
    0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL,
    0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL,
    0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
    0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL,
    0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL,
    0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL,
    0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL,
    0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
};

static constexpr uint8_t BISHOP_SHIFTS[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

static constexpr uint8_t ROOK_SHIFTS[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};


Attack::Attack()
{
    initPawnAttacks();
    initKingAttacks();
    initKnightAttacks();
    initBishopAttacks();
    initRookAttacks();
    initBetweenTable();
    initLinedTable();
}

Bitboard Attack::getMaskedBlockers(Bitboard mask, uint16_t index) const
{
    Bitboard blockers = 0;

    while (index) {
        uint8_t shift = Utils::getTrailingZeros(mask);
        if (index & 1) {
            blockers |= Bitboard(1) << shift;
        }
        mask &= (mask - 1);
        index >>= 1;
    }
    return blockers;
}

void Attack::initPawnAttacks()
{
    for (Square i = Square::A1; i < Square::Null; ++i) {
        mPawnAttacks[to_int(Color::White)][to_int(i)] = (Utils::northeastOne(Utils::getBitboard(i))) | (Utils::northwestOne(Utils::getBitboard(i)));
        mPawnAttacks[to_int(Color::Black)][to_int(i)] = (Utils::southeastOne(Utils::getBitboard(i))) | (Utils::southwestOne(Utils::getBitboard(i)));
    }
}

void Attack::initKingAttacks()
{
    Bitboard board = 1;

    for (Square i = Square::A1; i < Square::Null; ++i, board <<= 1) {
        mKingAttacks[to_int(i)] = calcKingAttacks(board);
    }
}

void Attack::initKnightAttacks()
{
    Bitboard board = 1;

    for (Square i = Square::A1; i < Square::Null; ++i, board <<= 1) {
        mKnightAttacks[to_int(i)] = calcKnightAttacks(board);
    }
}

void Attack::initBishopAttacks()
{
    for (Square i = Square::A1; i < Square::Null; ++i) {
        Bitboard mask = getSlidingMasks(PieceSets::WhiteBishops, i);
        uint16_t permutations = uint16_t(1) << BISHOP_SHIFTS[to_int(i)];

        for (uint16_t j = 0; j < permutations; ++j) {
            Bitboard blockers = getMaskedBlockers(mask, j);
            Bitboard attacks = calcBishopAttacks(i, blockers);
            uint16_t magicIndex = (blockers * BISHOP_MAGICS[to_int(i)]) >> (64 - BISHOP_SHIFTS[to_int(i)]);
            mBishopAttacks[to_int(i)][magicIndex] = attacks;
        }
        mBishopMasks[to_int(i)] = mask;
    }
}

void Attack::initRookAttacks()
{
    for (Square i = Square::A1; i < Square::Null; ++i) {
        Bitboard mask = getSlidingMasks(PieceSets::WhiteRooks, i);
        uint16_t permutations = 1 << ROOK_SHIFTS[to_int(i)];

        for (uint16_t j = 0; j < permutations; ++j) {
            Bitboard blockers = getMaskedBlockers(mask, j);
            Bitboard attacks = calcRookAttacks(i, blockers);
            uint16_t magicIndex = (blockers * ROOK_MAGICS[to_int(i)]) >> (64 - ROOK_SHIFTS[to_int(i)]);
            mRookAttacks[to_int(i)][magicIndex] = attacks;
        }
        mRookMasks[to_int(i)] = mask;
    }
}

Bitboard Attack::getPawnAttacks(Square sq, Color color) const
{
    return mPawnAttacks[to_int(color)][to_int(sq)];
}

Bitboard Attack::getKingAttacks(Square sq) const
{
    return mKingAttacks[to_int(sq)];
}

Bitboard Attack::getKnightAttacks(Square sq) const
{
    return mKnightAttacks[to_int(sq)];
}

Bitboard Attack::getBishopAttacks(Square sq, Bitboard blockers) const
{
    blockers &= mBishopMasks[to_int(sq)];
    blockers *= BISHOP_MAGICS[to_int(sq)];
    blockers >>= 64 - BISHOP_SHIFTS[to_int(sq)];
    return mBishopAttacks[to_int(sq)][blockers];
}

Bitboard Attack::getRookAttacks(Square sq, Bitboard blockers) const
{
    blockers &= mRookMasks[to_int(sq)];
    blockers *= ROOK_MAGICS[to_int(sq)];
    blockers >>= 64 - ROOK_SHIFTS[to_int(sq)];
    return mRookAttacks[to_int(sq)][blockers];
}

Bitboard Attack::getQueenAttacks(Square sq, Bitboard blockers) const
{
    return getBishopAttacks(sq, blockers) | getRookAttacks(sq, blockers);
}

Bitboard Attack::calcKingAttacks(Bitboard kingLoc) const
{
    Bitboard attacks = Utils::eastOne(kingLoc) | Utils::westOne(kingLoc);
    kingLoc |= attacks;
    attacks |= Utils::northOne(kingLoc) | Utils::southOne(kingLoc);
    return attacks;
}

Bitboard Attack::calcKnightAttacks(Bitboard knightLoc) const
{
    Bitboard east = Utils::eastOne(knightLoc);
    Bitboard west = Utils::westOne(knightLoc);
    Bitboard attacks = (east|west) << 16 | (east|west) >> 16;
    east = Utils::eastOne(east);
    west = Utils::westOne(west);
    attacks |= (east|west) << 8 | (east|west) >> 8;
    return attacks;
}   

Bitboard Attack::getSlidingMasks(PieceSets piece, Square sq) const
{
    Bitboard attacks = 0;

    if (piece == PieceSets::WhiteBishops) {
        for (Square i = Utils::northeastOne(sq); i != Square::Null; i = Utils::northeastOne(i)) {
            attacks |= Utils::getBitboard(i);
        }
        for (Square i = Utils::northwestOne(sq); i != Square::Null; i = Utils::northwestOne(i)) {
            attacks |= Utils::getBitboard(i);
        }
        for (Square i = Utils::southeastOne(sq); i != Square::Null; i = Utils::southeastOne(i)) {
            attacks |= Utils::getBitboard(i);
        }
        for (Square i = Utils::southwestOne(sq); i != Square::Null; i = Utils::southwestOne(i)) {
            attacks |= Utils::getBitboard(i);
        }
        attacks &= Utils::NOT_EDGES;
    } else if (piece == PieceSets::WhiteRooks) {
        Square i = Utils::northOne(sq);
        while (i != Square::Null && Utils::northOne(i) != Square::Null) {
            attacks |= Utils::getBitboard(i);
            i = Utils::northOne(i);
        }
        i = Utils::southOne(sq);
        while (i != Square::Null && Utils::southOne(i) != Square::Null) {
            attacks |= Utils::getBitboard(i);
            i = Utils::southOne(i);
        }
        i = Utils::westOne(sq);
        while (i != Square::Null && Utils::westOne(i) != Square::Null) {
            attacks |= Utils::getBitboard(i);
            i = Utils::westOne(i);
        }
        i = Utils::eastOne(sq);
        while (i != Square::Null && Utils::eastOne(i) != Square::Null) {
            attacks |= Utils::getBitboard(i);
            i = Utils::eastOne(i);
        }
    }
    return attacks;
}

Bitboard Attack::calcBishopAttacks(Square sq, Bitboard blockers) const
{
    Bitboard attacks = 0;

    for (Square i = Utils::northeastOne(sq); i != Square::Null; i = Utils::northeastOne(i)) {
        attacks |= Utils::getBitboard(i);
        if (blockers & Utils::getBitboard(i)) {
            break;
        }
    }
    for (Square i = Utils::northwestOne(sq); i != Square::Null; i = Utils::northwestOne(i)) {
        attacks |= Utils::getBitboard(i);
        if (blockers & Utils::getBitboard(i)) {
            break;
        }
    }
    for (Square i = Utils::southeastOne(sq); i != Square::Null; i = Utils::southeastOne(i)) {
        attacks |= Utils::getBitboard(i);
        if (blockers & Utils::getBitboard(i)) {
            break;
        }
    }
    for (Square i = Utils::southwestOne(sq); i != Square::Null; i = Utils::southwestOne(i)) {
        attacks |= Utils::getBitboard(i);
        if (blockers & Utils::getBitboard(i)) {
            break;
        }
    }
    return attacks;
}

Bitboard Attack::calcRookAttacks(Square sq, Bitboard blockers) const
{
    Bitboard attacks = 0;

    for (Square i = Utils::northOne(sq); i != Square::Null; i = Utils::northOne(i)) {
        attacks |= Utils::getBitboard(i);
        if (blockers & Utils::getBitboard(i)) {
            break;
        }
    }
    for (Square i = Utils::westOne(sq); i != Square::Null; i = Utils::westOne(i)) {
        attacks |= Utils::getBitboard(i);
        if (blockers & Utils::getBitboard(i)) {
            break;
        }
    }
    for (Square i = Utils::eastOne(sq); i != Square::Null; i = Utils::eastOne(i)) {
        attacks |= Utils::getBitboard(i);
        if (blockers & Utils::getBitboard(i)) {
            break;
        }
    }
    for (Square i = Utils::southOne(sq); i != Square::Null; i = Utils::southOne(i)) {
        attacks |= Utils::getBitboard(i);
        if (blockers & Utils::getBitboard(i)) {
            break;
        }
    }
    return attacks;
}

Bitboard Attack::inBetween(Square from, Square to) const
{
    return mBetweenRectangular[to_int(from)][to_int(to)];
}

void Attack::initBetweenTable()
{
    constexpr Bitboard allSet = Utils::EDGES | Utils::NOT_EDGES;
    constexpr Bitboard a2a7 = Utils::A_FILE & ~(Utils::EIGHTH_RANK | Utils::FIRST_RANK);
    constexpr Bitboard b2g7 = Utils::BLACK_DIAGONAL & ~(Utils::EIGHTH_RANK | Utils::FIRST_RANK);
    constexpr Bitboard h1b7 = Utils::WHITE_DIAGONAL & ~Utils::EIGHTH_RANK;

    for (Square i = Square::A1; i < Square::Null; ++i) {
        for (Square j = Square::A1; j < Square::Null; ++j) {
            Bitboard between = (allSet << to_int(i)) ^ (allSet << to_int(j));
            Bitboard file = (to_int(j) & 7) - (to_int(i) & 7);
            Bitboard rank = ((to_int(j) | 7) - to_int(i)) >> 3;
            Bitboard line = ((file & 7) - 1) & a2a7;
            line += 2 * (((rank & 7) - 1) >> 58);
            line += (((rank - file) & 15) - 1) & b2g7;
            line += (((rank + file) & 15) - 1) & h1b7;
            line *= between & -between;
            mBetweenRectangular[to_int(i)][to_int(j)] = line & between;
        }
    }
}

Bitboard Attack::inLine(Square from, Square to) const
{
    return mLined[to_int(from)][to_int(to)];
}

void Attack::initLinedTable()
{
    for (Square i = Square::A1; i < Square::Null; ++i) {
        for (Square j = Square::A1; j < Square::Null; ++j) {
            if (getBishopAttacks(i, 0) & Utils::getBitboard(j)) {
                mLined[to_int(i)][to_int(j)] = (getBishopAttacks(i, 0) & getBishopAttacks(j, 0)) | Utils::getBitboard(i) | Utils::getBitboard(j);
            } else if (getRookAttacks(i, 0) & Utils::getBitboard(j)) {
                mLined[to_int(i)][to_int(j)] = (getRookAttacks(i, 0) & getRookAttacks(j, 0)) | Utils::getBitboard(i) | Utils::getBitboard(j);
            }
        }
    }
}

