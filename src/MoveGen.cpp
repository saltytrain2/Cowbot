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

void MoveGen::generateLegalMoves()
{
    generateLegalPawnMoves();
    generateLegalKnightMoves();
    generateLegalBishopMoves();
    generateLegalRookMoves();
    generateLegalQueenMoves();
    generateLegalKingMoves();
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
        mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, QUIET));
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        mMoves.push_back(Move(Utils::southOne(Utils::southOne(endSquare)), endSquare, QUIET));
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, white);
        attacks &= enemyPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
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
        mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, QUIET));
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        mMoves.push_back(Move(Utils::northOne(Utils::northOne(endSquare)), endSquare, QUIET));
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, black);
        attacks &= enemyPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
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
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
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
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
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
            Move move(startSquare, Utils::popLSB(attacks), QUIET);
            if (board.isLegal(move) {
                mMoves.push_back(move);
            }
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
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
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
        mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
    }
}

std::vector<Move> MoveGen::getMoves() const noexcept
{
    return mMoves;
}

void MoveGen::generateLegalPawnMoves()
{
    mBoard.getTurn() == white ? generateLegalWhitePawnMoves() : generateLegalBlackPawnMoves();
}

void MoveGen::generateLegalWhitePawnMoves()
{
    Bitboard pawns = mBoard.getWhitePawns();
    Bitboard enemyPieces = mBoard.getBlackPieces();

    // pawn pushes
    Bitboard onePawnPush = Utils::northOne(pawns) & mBoard.getEmptySquares();
    Bitboard twoPawnPush = Utils::northOne(onePawnPush & Utils::THIRD_RANK) & mBoard.getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, QUIET));
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        mMoves.push_back(Move(Utils::southOne(Utils::southOne(endSquare)), endSquare, QUIET));
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, white);
        attacks &= enemyPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
        }
    }
}

void MoveGen::generateLegalBlackPawnMoves()
{
    Bitboard pawns = mBoard.getBlackPawns();
    Bitboard enemyPieces = mBoard.getWhitePieces();

    // pawn pushes
    Bitboard onePawnPush = Utils::southOne(pawns) & mBoard.getEmptySquares();
    Bitboard twoPawnPush = Utils::southOne(onePawnPush & Utils::SIXTH_RANK) & mBoard.getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        if ((Bitboard(1) << endSquare) & Utils::FIRST_RANK) {
        }
        mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, QUIET));
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        mMoves.push_back(Move(Utils::northOne(Utils::northOne(endSquare)), endSquare, QUIET));
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, black);
        attacks &= enemyPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
        }
    }
}

void MoveGen::generateLegalKnightMoves()
{
    Bitboard knights = mBoard.getTurn() == white ? mBoard.getWhiteKnights() : mBoard.getBlackKnights();
    Bitboard blockers = mBoard.getTurn() == white ? mBoard.getWhitePieces() : mBoard.getBlackPieces();
    knights &= ~mPinnedPieces;

    while (knights) {
        Square startSquare = Utils::popLSB(knights);
        Bitboard attacks = mAttack->getKnightAttacks(startSquare);
        attacks &= ~blockers;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
        }
    }
}

void MoveGen::generateLegalBishopMoves()
{
    Bitboard bishops = mBoard.getTurn() == white ? mBoard.getWhiteBishops() : mBoard.getBlackBishops();
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getTurn() == white ? mBoard.getWhitePieces() : mBoard.getBlackPieces();
    bishops &= ~mPinnedPieces;

    while (bishops) {
        Square startSquare = Utils::popLSB(bishops);
        Bitboard attacks = mAttack->getBishopAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
        }
    }
}

void MoveGen::generateLegalRookMoves()
{
    Bitboard rooks = mBoard.getTurn() == white ? mBoard.getWhiteRooks() : mBoard.getBlackRooks();
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getTurn() == white ? mBoard.getWhitePieces() : mBoard.getBlackPieces();
    rooks &= ~mPinnedPieces;

    while (rooks) {
        Square startSquare = Utils::popLSB(rooks);
        Bitboard attacks = mAttack->getRookAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
        }
    }
}

void MoveGen::generateLegalQueenMoves()
{
    Bitboard queens = mBoard.getTurn() == white ? mBoard.getWhiteQueens() : mBoard.getBlackQueens();
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getTurn() == white ? mBoard.getWhitePieces() : mBoard.getBlackPieces();
    queens &= ~mPinnedPieces;

    while (queens) {
        Square startSquare = Utils::popLSB(queens);
        Bitboard attacks = mAttack->getQueenAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
        }
    }
}

void MoveGen::generateLegalKingMoves()
{
    Bitboard king = mBoard.getTurn() == white ? mBoard.getWhiteKing() : mBoard.getBlackKing();
    Bitboard blockers = mBoard.getTurn() == white ? mBoard.getWhitePieces() : mBoard.getBlackPieces();

    Square startSquare = Utils::popLSB(king);
    Bitboard attacks = mAttack->getKingAttacks(startSquare);
    attacks &= ~blockers;
    while (attacks) {
        mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), QUIET));
    }
}
