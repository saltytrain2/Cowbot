#include "PieceUtils.h"

AttackBoard PieceUtils::eastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) << 1;
}

AttackBoard PieceUtils::westOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) >> 1;
}

AttackBoard PieceUtils::northOne(Bitboard pieceLoc)
{
    return pieceLoc >> 8;
}

AttackBoard PieceUtils::southOne(Bitboard pieceLoc)
{
    return pieceLoc << 8;
}

AttackBoard PieceUtils::northeastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) << 9;
}

AttackBoard PieceUtils::northwestOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) << 7;
}

AttackBoard PieceUtils::southwestOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_A_FILE) >> 9;
}

AttackBoard PieceUtils::southeastOne(Bitboard pieceLoc)
{
    return (pieceLoc & NOT_H_FILE) >> 7;
}

AttackBoard PieceUtils::kingAttacks(Bitboard kingLoc)
{
    AttackBoard attacks = eastOne(kingLoc) | westOne(kingLoc);
    kingLoc |= attacks;
    attacks |= northOne(kingLoc) | southOne(kingLoc);
    return attacks;
}

AttackBoard PieceUtils::knightAttacks(Bitboard knightLoc)
{
    AttackBoard east = eastOne(knightLoc);
    AttackBoard west = westOne(knightLoc);
    AttackBoard attacks = (east|west) << 16 | (east|west) >> 16;
    east = eastOne(east);
    west = westOne(west);
    attacks |= (east|west) << 8 | (east|west) >> 8;
    return attacks;
}