#include "MoveGen.h"

MoveGen::MoveGen(const ChessBoard& src, Attack* attack) : mAttack(attack), mMoves{}, mBoard(src)
{
}

void MoveGen::generatePseudoLegalMoves()
{
    Color toMove = mBoard.getTurn();

    generatePseudoLegalPawnMoves(toMove);
    generatePseudoLegalKnightMoves(toMove);
    generatePseudoLegalBishopMoves(toMove);
    generatePseudoLegalRookMoves(toMove);
    generatePseudoLegalQueenMoves(toMove);
    generatePseudoLegalKingMoves(toMove);
}

void MoveGen::generateLegalMoves()
{
    Color toMove = mBoard.getTurn();

    generateLegalPawnMoves(toMove);
    generateLegalKnightMoves(toMove);
    generateLegalBishopMoves(toMove);
    generateLegalRookMoves(toMove);
    generateLegalQueenMoves(toMove);
    generateLegalKingMoves(toMove);
}

void MoveGen::generatePseudoLegalPawnMoves(Color color)
{
    color == Color::White ? generatePseudoLegalWhitePawnMoves() : generatePseudoLegalBlackPawnMoves();
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
        if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
            mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen));
            mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook));
            mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop));
            mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight));
        } else {
            mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Quiet));
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        mMoves.push_back(Move(Utils::southOne(Utils::southOne(endSquare)), endSquare, MoveType::Quiet));
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::White);
        attacks &= enemyPieces;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight));
            } else {
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Quiet));
            }
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
        mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Quiet));
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        mMoves.push_back(Move(Utils::northOne(Utils::northOne(endSquare)), endSquare, MoveType::Quiet));
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::Black);
        attacks &= enemyPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), MoveType::Quiet));
        }
    }
}

void MoveGen::generatePseudoLegalKnightMoves(Color color)
{
    Bitboard knights = mBoard.getKnights(color);
    Bitboard blockers = mBoard.getPieces(color);

    while (knights) {
        Square startSquare = Utils::popLSB(knights);
        Bitboard attacks = mAttack->getKnightAttacks(startSquare);
        attacks &= ~blockers;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), MoveType::Quiet));
        }
    }
}

void MoveGen::generatePseudoLegalBishopMoves(Color color)
{
    Bitboard bishops = mBoard.getBishops(color);
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getPieces(color);

    while (bishops) {
        Square startSquare = Utils::popLSB(bishops);
        Bitboard attacks = mAttack->getBishopAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), MoveType::Quiet));
        }
    }
}

void MoveGen::generatePseudoLegalRookMoves(Color color)
{
    Bitboard rooks = mBoard.getRooks(color);
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getPieces(color);

    while (rooks) {
        Square startSquare = Utils::popLSB(rooks);
        Bitboard attacks = mAttack->getRookAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            Move move(startSquare, Utils::popLSB(attacks), MoveType::Quiet);
            mMoves.push_back(move);
        }
    }
}

void MoveGen::generatePseudoLegalQueenMoves(Color color)
{
    Bitboard queens = mBoard.getQueens(color);
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getPieces(color);

    while (queens) {
        Square startSquare = Utils::popLSB(queens);
        Bitboard attacks = mAttack->getQueenAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), MoveType::Quiet));
        }
    }
}

void MoveGen::generatePseudoLegalKingMoves(Color color)
{
    Bitboard king = mBoard.getKing(color);
    Bitboard blockers = mBoard.getPieces(color);

    Square startSquare = Utils::popLSB(king);
    Bitboard attacks = mAttack->getKingAttacks(startSquare);
    attacks &= ~blockers;
    while (attacks) {
        mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), MoveType::Quiet));
    }
}

std::vector<Move> MoveGen::getMoves() const noexcept
{
    return mMoves;
}

void MoveGen::generateLegalPawnMoves(Color color)
{
    color == Color::White ? generateLegalWhitePawnMoves() : generateLegalBlackPawnMoves();
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
        if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
            mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen));
            mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook));
            mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop));
            mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight));
        } else {
            mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Quiet));
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        mMoves.push_back(Move(Utils::southOne(Utils::southOne(endSquare)), endSquare, MoveType::Quiet));
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::White);
        attacks &= enemyPieces;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight));
            } else {
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Quiet));
            }
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
        if (Utils::getBitboard(endSquare) & Utils::FIRST_RANK) {
            mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen));
            mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook));
            mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop));
            mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight));
        } else {
            mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Quiet));
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        mMoves.push_back(Move(Utils::northOne(Utils::northOne(endSquare)), endSquare, MoveType::Quiet));
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::Black);
        attacks &= enemyPieces;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight));
            } else {
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Quiet));
            }
        }
    }
}

void MoveGen::generateLegalKnightMoves(Color color)
{
    Bitboard knights = mBoard.getKnights(color);
    Bitboard blockers = mBoard.getPieces(color);
    knights &= ~mPinnedPieces;

    while (knights) {
        Square startSquare = Utils::popLSB(knights);
        Bitboard attacks = mAttack->getKnightAttacks(startSquare);
        attacks &= ~blockers;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), MoveType::Quiet));
        }
    }
}

void MoveGen::generateLegalBishopMoves(Color color)
{
    Bitboard bishops = mBoard.getBishops(color);
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getPieces(color);

    while (bishops) {
        Square startSquare = Utils::popLSB(bishops);
        Bitboard attacks = mAttack->getBishopAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), MoveType::Quiet));
        }
    }
}

void MoveGen::generateLegalRookMoves(Color color)
{
    Bitboard rooks = mBoard.getRooks(color);
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getPieces(color);

    while (rooks) {
        Square startSquare = Utils::popLSB(rooks);
        Bitboard attacks = mAttack->getRookAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), MoveType::Quiet));
        }
    }
}

void MoveGen::generateLegalQueenMoves(Color color)
{
    Bitboard queens = mBoard.getQueens(color);
    Bitboard blockers = mBoard.getAllPieces();
    Bitboard ownPieces = mBoard.getPieces(color);

    while (queens) {
        Square startSquare = Utils::popLSB(queens);
        Bitboard attacks = mAttack->getQueenAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), MoveType::Quiet));
        }
    }
}

void MoveGen::generateLegalKingMoves(Color color)
{
    Bitboard king = mBoard.getKing(color);
    bool kingsideRights = mBoard.getCastleRights(Castling::Kingside, color);
    bool queensideRights = mBoard.getCastleRights(Castling::Queenside, color);
    Bitboard blockers = mBoard.getPieces(color);

    Square startSquare = Utils::popLSB(king);
    Bitboard attacks = mAttack->getKingAttacks(startSquare);
    attacks &= ~blockers;
    while (attacks) {
        mMoves.push_back(Move(startSquare, Utils::popLSB(attacks), MoveType::Quiet));
    }

    if (!mBoard.isSquareUnderAttack(startSquare, !color, blockers)) {
        // Castling
        Bitboard startBitboard = Utils::getBitboard(startSquare);
        if (kingsideRights) {
            Bitboard kingsideOne = Utils::eastOne(startBitboard);
            Bitboard kingsideTwo = Utils::eastOne(kingsideOne);
            if (kingsideOne & ~blockers && kingsideTwo & ~blockers) {
                mMoves.push_back(Move(startSquare, Utils::getSquare(kingsideTwo), MoveType::Castle));
            }
        }
        if (queensideRights) {
            Bitboard queensideOne = Utils::westOne(startBitboard);
            Bitboard queensideTwo = Utils::westOne(queensideOne);
            if (queensideOne & ~blockers && queensideTwo & ~blockers) {
             mMoves.push_back(Move(startSquare, Utils::getSquare(queensideTwo), MoveType::Castle));
            }
        }
    }
}