#pragma once

#include "types.h"
#include <string>

namespace Cowbot {
class Move
{
public:
    Move();
    explicit Move(std::string lan);
    explicit Move(Bitmove src);
    Move(Square startSquare, Square endSquare, MoveType type, PromotionPiece piece = PromotionPiece::Null);
    ~Move() = default;
    Move(const Move& rhs) = default;
    Move& operator=(const Move& rhs) = default;
    bool operator==(const Move& rhs) const;
    operator bool() const;
    bool operator<(const Move& rhs) const;

    void updateMove(std::string lan);
    Square getStartingSquare() const noexcept;
    Square getEndingSquare() const noexcept;
    MoveType getMoveType() const noexcept;
    PromotionPiece getPromotionPiece() const noexcept;
    Bitmove getMove() const noexcept;

    // converting a lan string into its 16-bit representation
    
    std::string toString() const;

private:
    // Based off of Stockfish's move representation
    // one move contains 16 bits of information
    // 2 bits for move type: 00 standard, 01 castle, 10 promotion, 11 enpassant
    // 2 bits for promotion type if promotion bit set: 00 knight, 01 bishop, 10 rook, 11 queen
    // 6 bits for destination square
    // 6 bits for current square
    Bitmove mMove;
};
} // namespace Cowbot