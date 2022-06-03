#pragma once

#include <cstdint>
#include <type_traits>
#include <cassert>
#include <string>

typedef uint64_t Bitboard;
typedef std::string UCImove;
typedef uint16_t Bitmove;

enum class Color : uint8_t {White, Black};

inline constexpr uint8_t to_int(Color color) noexcept
{
    return static_cast<uint8_t>(color);
}

inline Color operator!(Color rhs)
{
    return Color(to_int(rhs) ^ 1);
}

// Little Endian Rank-File Mapping 
enum class Square : uint8_t {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, Null
};

inline Square operator++(Square& rhs) 
{
    uint8_t squareAsInt = static_cast<uint8_t>(rhs);
    rhs = static_cast<Square>(++squareAsInt);
    return rhs;
}

inline Square operator--(Square& rhs) 
{
    uint8_t squareAsInt = static_cast<uint8_t>(rhs);
    rhs = static_cast<Square>(--squareAsInt);
    return rhs;
}

inline Square operator+=(Square& rhs, uint8_t offset)
{
    uint8_t squareAsInt = static_cast<uint8_t>(rhs);
    rhs = static_cast<Square>(squareAsInt + offset);
    return rhs;
}

inline Square operator-=(Square& rhs, uint8_t offset)
{
    uint8_t squareAsInt = static_cast<uint8_t>(rhs);
    rhs = static_cast<Square>(squareAsInt - offset);
    return rhs;
}

inline constexpr uint8_t to_int(Square sq)
{
    return static_cast<uint8_t>(sq);
}

enum class PieceSets : uint8_t {
    WhitePawns, BlackPawns, WhiteKnights, BlackKnights, WhiteBishops, BlackBishops,
    WhiteRooks, BlackRooks, WhiteQueens, BlackQueens, WhiteKing, BlackKing,
    WhitePieces, BlackPieces, AllPieces, EmptySquares
};

inline PieceSets operator++(PieceSets& rhs)
{
    uint8_t pieceAsInt = static_cast<uint8_t>(rhs);
    rhs = static_cast<PieceSets>(++pieceAsInt);
    return rhs;
}

inline PieceSets operator--(PieceSets& rhs)
{
    uint8_t pieceAsInt = static_cast<uint8_t>(rhs);
    rhs = static_cast<PieceSets>(--pieceAsInt);
    return rhs;
}

inline constexpr uint8_t to_int(PieceSets piece)
{
    return static_cast<uint8_t>(piece);
}

enum class Castling : uint8_t {Kingside, Queenside};

inline constexpr uint8_t to_int(Castling side)
{
    return static_cast<uint8_t>(side);
}

enum class PieceValues {
    pawn = 100,
    knight = 300,
    bishop = 300,
    rook = 500,
    queen = 900,
    king = 100000
};

enum class MoveType : uint8_t {
    Quiet, Castle, Promotion, Enpassant
};

inline constexpr uint8_t to_int(MoveType type) 
{
    return static_cast<uint8_t>(type);
}

enum class PromotionPiece : uint8_t {
    Knight, Bishop, Rook, Queen, Null = 0
};

inline constexpr uint8_t to_int(PromotionPiece piece)
{
    return static_cast<uint8_t>(piece);
}