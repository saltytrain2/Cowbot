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
    Bitboard mPinnedPieces;
    Bitboard mKingAttackers;

    // Psuedo Legal Move Generation
    void generatePseudoLegalPawnMoves(Color color);
    void generatePseudoLegalWhitePawnMoves();
    void generatePseudoLegalBlackPawnMoves();
    void generatePseudoLegalKnightMoves(Color color);
    void generatePseudoLegalBishopMoves(Color color);
    void generatePseudoLegalRookMoves(Color color);
    void generatePseudoLegalQueenMoves(Color color);
    void generatePseudoLegalKingMoves(Color color);

    // Legal Move Generation
    void generateLegalPawnMoves(Color color);
    void generateLegalWhitePawnMoves();
    void generateLegalBlackPawnMoves();
    void generateLegalKnightMoves(Color color);
    void generateLegalBishopMoves(Color color);
    void generateLegalRookMoves(Color color);
    void generateLegalQueenMoves(Color color);
    void generateLegalKingMoves(Color color);

    bool isLegal(const Move& move, Color color);
    bool legalPinnedMove(Bitboard startLoc, Bitboard endLoc, Bitboard blockers);
    bool isSquareUnderAttack(Square sq, Color color, Bitboard blockers);
    Bitboard getWhiteAttacks(Bitboard blockers);
    Bitboard getBlackAttacks(Bitboard blockers);
};