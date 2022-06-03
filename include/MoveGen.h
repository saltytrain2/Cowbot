#pragma once

#include "types.h"
#include "Utils.h"
#include "ChessBoard.h"
#include "Attack.h"
#include "Move.h"
#include <vector>
#include <memory>

class MoveGen
{
public:
    MoveGen(std::shared_ptr<ChessBoard> src, std::shared_ptr<Attack> attack);
    ~MoveGen() = default;
    MoveGen(const MoveGen& rhs) = default;
    MoveGen& operator=(const MoveGen& rhs) = default;

    void generatePseudoLegalMoves();
    void generateLegalMoves();

    std::vector<Move> getMoves() const noexcept;
private:
    std::shared_ptr<Attack> mAttack;
    std::vector<Move> mMoves;
    std::shared_ptr<ChessBoard> mBoard;

    // Legal Move Generation, not escaping check
    void generateLegalNonEvasivePawnMoves(Color color);
    void generateLegalNonEvasiveWhitePawnMoves();
    void generateLegalNonEvasiveBlackPawnMoves();
    void generateLegalNonEvasiveKnightMoves(Color color);
    void generateLegalNonEvasiveBishopMoves(Color color);
    void generateLegalNonEvasiveRookMoves(Color color);
    void generateLegalNonEvasiveQueenMoves(Color color);
    void generateLegalKingMoves(Color color);

    // Legal Move Generation, escaping check
    void generateLegalEvasivePawnMoves(Color color, Bitboard checkers);
    void generateLegalEvasiveWhitePawnMoves(Bitboard checkers);
    void generateLegalEvasiveBlackPawnMoves(Bitboard checkers);
    void generateLegalEvasiveKnightMoves(Color color, Bitboard checkers);
    void generateLegalEvasiveBishopMoves(Color color, Bitboard checkers);
    void generateLegalEvasiveRookMoves(Color color, Bitboard checkers);
    void generateLegalEvasiveQueenMoves(Color color, Bitboard checkers);
    void generateLegalEvasiveKingMoves(Color color, Bitboard checkers);

    bool isLegal(const Move& move, Color color);
    bool legalPinnedMove(Bitboard startLoc, Bitboard endLoc, Bitboard blockers);
    bool isSquareUnderAttack(Square sq, Color color, Bitboard blockers);
    Bitboard getWhiteAttacks(Bitboard blockers);
    Bitboard getBlackAttacks(Bitboard blockers);
};