#include "MoveGen.h"

MoveGen::MoveGen(const ChessBoard& src, Attack* attack) : mAttack(attack), mMoves{}, mBoard(src)
{
}

void MoveGen::generatePseudoLegalMoves()
{
    generatePseudoLegalPawnMoves();
    generatePseudoLegalKnightMoves();
    generatePseudoLegalBishopMoves();
    generatePseudoLegalRookMoves();
    generatePseudoLegalQueenMoves();
    generatePseudoLegalKingMoves();
}

void MoveGen::generatePseudoLegalPawnMoves()
{
    mBoard.getTurn() == white ? generatePseudoLegalWhitePawnMoves() : generatePseudoLegalBlackPawnMoves();
}

void MoveGen::generatePseudoLegalWhitePawnMoves()
{
    Bitboard pawns = mBoard.getWhitePawns();
    Bitboard enemyPieces = mBoard.getBlackPieces();

    // pawn pushes
    Bitboard onePawnPush = Utils::northOne(pawns) & mBoard.getEmptySquares();
    Bitboard twoPawnPush = Utils::northOne(onePawnPush & Utils::THIRD_RANK) & mBoard.getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        mMoves.push_back(Move(Utils::southOne(endSquare), endSquare));
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        mMoves.push_back(Move(Utils::southOne(Utils::southOne(endSquare)), endSquare));
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, white);
        attacks &= enemyPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks)));
        }
    }
}

void MoveGen::generatePseudoLegalBlackPawnMoves()
{
    Bitboard pawns = mBoard.getBlackPawns();
    Bitboard enemyPieces = mBoard.getWhitePieces();

    // pawn pushes
    Bitboard onePawnPush = Utils::southOne(pawns) & mBoard.getEmptySquares();
    Bitboard twoPawnPush = Utils::southOne(onePawnPush & Utils::SIXTH_RANK) & mBoard.getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        mMoves.push_back(Move(Utils::northOne(endSquare), endSquare));
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        mMoves.push_back(Move(Utils::northOne(Utils::northOne(endSquare)), endSquare));
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, black);
        attacks &= enemyPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks)));
        }
    }
}

void MoveGen::generatePseudoLegalKnightMoves()
{
    Bitboard knights = mBoard.getTurn() == white ? mBoard.getWhiteKnights() : mBoard.getBlackKnights();
    Bitboard blockers = mBoard.getTurn() == white ? mBoard.getWhitePieces() : mBoard.getBlackPieces();

    while (knights) {
        Square startSquare = Utils::popLSB(knights);
        Bitboard attacks = mAttack->getKnightAttacks(startSquare);
        attacks &= ~blockers;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks)));
        }
    }
}

void MoveGen::generatePseudoLegalBishopMoves()
{
    Bitboard bishops = mBoard.getTurn() == white ? mBoard.getWhiteBishops() : mBoard.getBlackBishops();
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getTurn() == white ? mBoard.getWhitePieces() : mBoard.getBlackPieces();

    while (bishops) {
        Square startSquare = Utils::popLSB(bishops);
        Bitboard attacks = mAttack->getBishopAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks)));
        }
    }
}

void MoveGen::generatePseudoLegalRookMoves()
{
    Bitboard rooks = mBoard.getTurn() == white ? mBoard.getWhiteRooks() : mBoard.getBlackRooks();
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getTurn() == white ? mBoard.getWhitePieces() : mBoard.getBlackPieces();

    while (rooks) {
        Square startSquare = Utils::popLSB(rooks);
        Bitboard attacks = mAttack->getRookAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks)));
        }
    }
}

void MoveGen::generatePseudoLegalQueenMoves()
{
    Bitboard queens = mBoard.getTurn() == white ? mBoard.getWhiteQueens() : mBoard.getBlackQueens();
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getTurn() == white ? mBoard.getWhitePieces() : mBoard.getBlackPieces();

    while (queens) {
        Square startSquare = Utils::popLSB(queens);
        Bitboard attacks = mAttack->getQueenAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks)));
        }
    }
}

void MoveGen::generatePseudoLegalKingMoves()
{
    Bitboard king = mBoard.getTurn() == white ? mBoard.getWhiteKing() : mBoard.getBlackKing();
    Bitboard blockers = mBoard.getTurn() == white ? mBoard.getWhitePieces() : mBoard.getBlackPieces();

    Square startSquare = Utils::popLSB(king);
    Bitboard attacks = mAttack->getKingAttacks(startSquare);
    attacks &= ~blockers;
    while (attacks) {
        mMoves.push_back(Move(startSquare, Utils::popLSB(attacks)));
    }
}

std::vector<Move> MoveGen::getMoves() const noexcept
{
    return mMoves;
}
