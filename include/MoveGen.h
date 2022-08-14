#pragma once

#include "types.h"
#include "Utils.h"
#include "ChessBoard.h"
#include "Attack.h"
#include "Move.h"
#include <vector>
#include <memory>

namespace Cowbot {
class MoveGen
{
public:
    MoveGen(ChessBoard* src, Attack* attack);
    ~MoveGen() = default;
    MoveGen(const MoveGen& rhs) = default;
    MoveGen& operator=(const MoveGen& rhs) = default;

    void generatePseudoLegalMoves();
    std::vector<Move> generateLegalMoves(Color side);
    std::vector<Move> generateLegalCaptures(Color side);

private:
    Attack* mAttack;
    ChessBoard* mBoard;

    // Legal Move Generation, not escaping check
    void generateLegalNonEvasivePawnMoves(Color color, std::vector<Move>& moveList);
    void generateLegalNonEvasiveWhitePawnMoves(std::vector<Move>& moveList);
    void generateLegalNonEvasiveBlackPawnMoves(std::vector<Move>& moveList);
    void generateLegalNonEvasiveKnightMoves(Color color, std::vector<Move>& moveList);
    void generateLegalNonEvasiveBishopMoves(Color color, std::vector<Move>& moveList);
    void generateLegalNonEvasiveRookMoves(Color color, std::vector<Move>& moveList);
    void generateLegalNonEvasiveQueenMoves(Color color, std::vector<Move>& moveList);
    void generateLegalKingMoves(Color color, std::vector<Move>& moveList);

    // Legal Move Generation, escaping check
    void generateLegalEvasivePawnMoves(Color color, Bitboard checkers, std::vector<Move>& moveList);
    void generateLegalEvasiveWhitePawnMoves(Bitboard checkers, std::vector<Move>& moveList);
    void generateLegalEvasiveBlackPawnMoves(Bitboard checkers, std::vector<Move>& moveList);
    void generateLegalEvasiveKnightMoves(Color color, Bitboard checkers, std::vector<Move>& moveList);
    void generateLegalEvasiveBishopMoves(Color color, Bitboard checkers, std::vector<Move>& moveList);
    void generateLegalEvasiveRookMoves(Color color, Bitboard checkers, std::vector<Move>& moveList);
    void generateLegalEvasiveQueenMoves(Color color, Bitboard checkers, std::vector<Move>& moveList);
    void generateLegalEvasiveKingMoves(Color color, Bitboard checkers, std::vector<Move>& moveList);
};
} // namespace Cowbot