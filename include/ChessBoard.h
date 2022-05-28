#pragma once

#include <cstdint>
#include <cctype>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <string>
#include <algorithm>

#include "types.h"
#include "Move.h"
#include "Attack.h"

class ChessBoard
{
public:
    // Alternate constructor w/ default parameter
    // layout follows Forsyth-Edwards Notation, no guarantee on 3-fold repetition
    explicit ChessBoard(Attack* ptr, std::string layout = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    ~ChessBoard() = default;
    ChessBoard(const ChessBoard& rhs) = default;
    ChessBoard& operator=(const ChessBoard& rhs) = default;

    // All of the get methods for each bitboard
    Bitboard getWhitePawns() const noexcept;
    Bitboard getWhiteKnights() const noexcept;
    Bitboard getWhiteBishops() const noexcept;
    Bitboard getWhiteRooks() const noexcept;
    Bitboard getWhiteQueens() const noexcept;
    Bitboard getWhiteKing() const noexcept;
    Bitboard getBlackPawns() const noexcept;
    Bitboard getBlackKnights() const noexcept;
    Bitboard getBlackBishops() const noexcept;
    Bitboard getBlackRooks() const noexcept;
    Bitboard getBlackQueens() const noexcept;
    Bitboard getBlackKing() const noexcept;
    Bitboard getWhitePieces() const noexcept;
    Bitboard getBlackPieces() const noexcept;
    Bitboard getAllPieces() const noexcept;
    Bitboard getEmptySquares() const noexcept;

    Bitboard getPawns(Color color) const noexcept;
    Bitboard getKnights(Color color) const noexcept;
    Bitboard getBishops(Color color) const noexcept;
    Bitboard getRooks(Color color) const noexcept;
    Bitboard getQueens(Color color) const noexcept;
    Bitboard getKing(Color color) const noexcept;
    Bitboard getPieces(Color color) const noexcept;

    // get method for the turn
    Color getTurn() const noexcept;

    // get methods for castling rights
    bool getWhiteCastleRights(Castling side) const noexcept;
    bool getBlackCastleRights(Castling side) const noexcept;
    bool getCastleRights(Castling side, Color color) const noexcept;

    std::vector<Move> getMoveList() const noexcept;
    Bitboard getPinnedPieces() const noexcept;
    void printSquareBoard() const noexcept;

    // makes the move
    int makeMove(Move& nextMove) noexcept;
    void undoMove();


    bool isLegal(const Move& move, Color turn);
    bool legalPinnedMove(Bitboard startLoc, Bitboard endLoc, Bitboard blockers);
    bool isSquareUnderAttack(Square sq, Color color, Bitboard blockers);
    Bitboard squareAttackers(Square sq, Color color, Bitboard blockers);

private:
    Attack* mAttack;
    // buffer to store all bitboards describing the board state
    // boards 0-5 describe individual white pieces ordered from pawn, knight ... queen, king
    // boards 6-11 describe individual black pieces ordered from pawn, knight ... queen, king
    // board 12 describes all white pieces, board 13 describes all black pieces
    // board 14 describes all occupied squares, board 16 describes all empty squares
    std::array<Bitboard, 16> mPieceBB;
    // which player's turn it is to make a move
    Color mTurn;
 
    // buffers to check whehter the player can castle or not
    // index 0 for kingside, index 1 for queenside
    std::array<std::array<bool, 2>, 2> mCastle;


    // A square-centric represenation of the board
    PieceSets mSquareBoard[64];
    // A list of moves that have been currently made
    std::vector<Move> mMoveList;
    std::vector<std::array<std::array<bool, 2>, 2>> mCastlingList;
    std::vector<Bitboard> mPinnedList;

    // keeping track of which pieces are currently pinned
    Bitboard mPinnedPieces;

    void updateRedundantBitboards() noexcept;
    void updateSquareBoard() noexcept;
    void updateCastlingRights(PieceSets movedPiece) noexcept;
    void updatePinnedPieces() noexcept;

    // TODO 
    // possibly have to overload this function with a string and rank version
    void updateBitboards(const std::vector<std::string>& piecesByRank) noexcept;
};