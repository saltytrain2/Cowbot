#pragma once

#include "types.h"
#include "Utils.h"
#include "ChessBoard.h"
#include "Attack.h"
#include "Move.h"
#include <vector>

class MoveGen
{
public:
    MoveGen(const ChessBoard& src, Attack* attack);
    ~MoveGen() = default;
    MoveGen(const MoveGen& rhs) = default;
    MoveGen& operator=(const MoveGen& rhs) = default;

    void generatePseudoLegalMoves();
    void generateLegalMoves();

    std::vector<Move> getMoves() const noexcept;
private:
    Attack* mAttack;
    std::vector<Move> mMoves;
    ChessBoard mBoard;

    void generatePseudoLegalPawnMoves();
    void generatePseudoLegalWhitePawnMoves();
    void generatePseudoLegalBlackPawnMoves();
    void generatePseudoLegalKnightMoves();
    void generatePseudoLegalBishopMoves();
    void generatePseudoLegalRookMoves();
    void generatePseudoLegalQueenMoves();
    void generatePseudoLegalKingMoves();
};