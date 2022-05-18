#pragma once

#include "types.h"
#include <string>


class Move
{
public:
    explicit Move(std::string lan);
    explicit Move(Bitmove src);
    Move(Square startSquare, Square endSquare, MoveType type);
    ~Move() = default;
    Move(const Move& rhs) = default;
    Move& operator=(const Move& rhs) = default;

    void updateMove(std::string lan);
    Bitmove getMove() const noexcept;
    void setCapturedPiece(PieceSets piece) noexcept;
    PieceSets getCapturedPiece() const;
    // converting a lan string into its 16-bit representation
    //Bitmove convertStringToMove(const std::string& lan);
private:
    // Based off of Stockfish's move representation
    // one move contains 16 bits of information
    // 2 bits for move type: 00 standard, 01 castle, 10 promotion, 11 enpassant
    // 2 bits for promotion type if promotion bit set: 00 knight, 01 bishop, 10 rook, 11 queen
    // 6 bits for destination square
    // 6 bits for current square
    Bitmove mMove;
    PieceSets mCapturedPiece;
};