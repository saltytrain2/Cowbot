#include "MoveGen.h"

MoveGen::MoveGen(std::shared_ptr<ChessBoard> src, std::shared_ptr<Attack> attack) 
    : mAttack(attack), mMoves{}, mBoard(src)
{
}

void MoveGen::generateLegalMoves()
{
    mMoves.clear();
    Color toMove = mBoard->getTurn();
    Bitboard checkers = mBoard->getKingAttackers(toMove, mBoard->getAllPieces());

    if (checkers) {
        generateLegalEvasivePawnMoves(toMove, checkers);
        generateLegalEvasiveKnightMoves(toMove, checkers);
        generateLegalEvasiveBishopMoves(toMove, checkers);
        generateLegalEvasiveRookMoves(toMove, checkers);
        generateLegalEvasiveQueenMoves(toMove, checkers);
    } else {
        generateLegalNonEvasivePawnMoves(toMove);
        generateLegalNonEvasiveKnightMoves(toMove);
        generateLegalNonEvasiveBishopMoves(toMove);
        generateLegalNonEvasiveRookMoves(toMove);
        generateLegalNonEvasiveQueenMoves(toMove);
    }
    generateLegalKingMoves(toMove);
}

std::vector<Move> MoveGen::getMoves() const noexcept
{
    return mMoves;
}

void MoveGen::generateLegalNonEvasivePawnMoves(Color color)
{
    color == Color::White ? generateLegalNonEvasiveWhitePawnMoves() : generateLegalNonEvasiveBlackPawnMoves();
}

void MoveGen::generateLegalNonEvasiveWhitePawnMoves()
{
    Bitboard pawns = mBoard->getWhitePawns();
    Bitboard enemyPieces = mBoard->getBlackPieces();

    // pawn pushes
    Bitboard onePawnPush = Utils::northOne(pawns) & mBoard->getEmptySquares();
    Bitboard twoPawnPush = Utils::northOne(onePawnPush & Utils::THIRD_RANK) & mBoard->getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
            if (mBoard->isLegal(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen));
                mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook));
                mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight));
            }
        } else {
            Move move(Utils::southOne(endSquare), endSquare, MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                mMoves.push_back(move);
            }
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        Move move(Utils::southOne(Utils::southOne(endSquare)), endSquare, MoveType::Quiet);
        if (mBoard->isLegal(move)) {
            mMoves.push_back(move);
        }
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::White);
        attacks &= enemyPieces;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
                if (mBoard->isLegal(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight));
                }
            } else {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalNonEvasiveBlackPawnMoves()
{
    Bitboard pawns = mBoard->getBlackPawns();
    Bitboard enemyPieces = mBoard->getWhitePieces();

    // pawn pushes
    Bitboard onePawnPush = Utils::southOne(pawns) & mBoard->getEmptySquares();
    Bitboard twoPawnPush = Utils::southOne(onePawnPush & Utils::SIXTH_RANK) & mBoard->getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        if (Utils::getBitboard(endSquare) & Utils::FIRST_RANK) {
            if (mBoard->isLegal(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen));
                mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook));
                mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight));
            }
        } else {
            Move move(Utils::northOne(endSquare), endSquare, MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                mMoves.push_back(move);
            }
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        Move move(Utils::northOne(Utils::northOne(endSquare)), endSquare, MoveType::Quiet);
        if (mBoard->isLegal(move)) {
            mMoves.push_back(move);
        }
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::Black);
        attacks &= enemyPieces;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::FIRST_RANK) {
                if (mBoard->isLegal(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight));
                }
            } else {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalNonEvasiveKnightMoves(Color color)
{
    Bitboard knights = mBoard->getKnights(color);
    Bitboard blockers = mBoard->getPieces(color);

    while (knights) {
        Square startSquare = Utils::popLSB(knights);
        Bitboard attacks = mAttack->getKnightAttacks(startSquare);
        attacks &= ~blockers;
        while (attacks) {
            Move move(startSquare, Utils::popLSB(attacks), MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                mMoves.push_back(move);
            }
        }
    }
}

void MoveGen::generateLegalNonEvasiveBishopMoves(Color color)
{
    Bitboard bishops = mBoard->getBishops(color);
    Bitboard blockers = mBoard->getAllPieces();
    Bitboard ownPieces = mBoard->getPieces(color);

    while (bishops) {
        Square startSquare = Utils::popLSB(bishops);
        Bitboard attacks = mAttack->getBishopAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            Move move(startSquare, Utils::popLSB(attacks), MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                mMoves.push_back(move);
            }
        }
    }
}

void MoveGen::generateLegalNonEvasiveRookMoves(Color color)
{
    Bitboard rooks = mBoard->getRooks(color);
    Bitboard blockers = mBoard->getAllPieces();
    Bitboard ownPieces = mBoard->getPieces(color);

    while (rooks) {
        Square startSquare = Utils::popLSB(rooks);
        Bitboard attacks = mAttack->getRookAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            Move move(startSquare, Utils::popLSB(attacks), MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                mMoves.push_back(move);
            }
        }
    }
}

void MoveGen::generateLegalNonEvasiveQueenMoves(Color color)
{
    Bitboard queens = mBoard->getQueens(color);
    Bitboard blockers = mBoard->getAllPieces();
    Bitboard ownPieces = mBoard->getPieces(color);

    while (queens) {
        Square startSquare = Utils::popLSB(queens);
        Bitboard attacks = mAttack->getQueenAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            Move move(startSquare, Utils::popLSB(attacks), MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                mMoves.push_back(move);
            }
        }
    }
}

void MoveGen::generateLegalKingMoves(Color color)
{
    Bitboard king = mBoard->getKing(color);
    bool kingsideRights = mBoard->getCastleRights(Castling::Kingside, color);
    bool queensideRights = mBoard->getCastleRights(Castling::Queenside, color);
    Bitboard blockers = mBoard->getPieces(color);

    Square startSquare = Utils::popLSB(king);
    Bitboard attacks = mAttack->getKingAttacks(startSquare);
    attacks &= ~blockers;
    while (attacks) {
        Move move(startSquare, Utils::popLSB(attacks), MoveType::Quiet);
        if (mBoard->isLegal(move)) {
            mMoves.push_back(move);
        }
    }

    if (!mBoard->isKingUnderAttack(color, blockers)) {
        // Castling
        Bitboard startBitboard = Utils::getBitboard(startSquare);
        if (kingsideRights) {
            Bitboard kingsideOne = Utils::eastOne(startBitboard);
            Bitboard kingsideTwo = Utils::eastOne(kingsideOne);
            if (kingsideOne & ~blockers && kingsideTwo & ~blockers) {
                Move move(startSquare, Utils::getSquare(kingsideTwo), MoveType::Castle);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
        if (queensideRights) {
            Bitboard queensideOne = Utils::westOne(startBitboard);
            Bitboard queensideTwo = Utils::westOne(queensideOne);
            if (queensideOne & ~blockers && queensideTwo & ~blockers) {
                Move move(startSquare, Utils::getSquare(queensideTwo), MoveType::Castle);
                if (mBoard->isLegal(move)) {
                     mMoves.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasivePawnMoves(Color color, Bitboard checkers)
{
    color == Color::White ? generateLegalEvasiveWhitePawnMoves(checkers) : generateLegalEvasiveBlackPawnMoves(checkers);
}

void MoveGen::generateLegalEvasiveWhitePawnMoves(Bitboard checkers)
{
    Bitboard pawns = mBoard->getWhitePawns();
    Square kingSquare = Utils::getSquare(mBoard->getKing(Color::White));

    // pawn pushes
    Bitboard onePawnPush = Utils::northOne(pawns) & mBoard->getEmptySquares();
    Bitboard twoPawnPush = Utils::northOne(onePawnPush & Utils::THIRD_RANK) & mBoard->getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare)) {
            if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
                if (mBoard->isLegal(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                    mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen));
                    mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook));
                    mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                    mMoves.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight));
                }
            } else {
                Move move(Utils::southOne(endSquare), endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare)) {
            Move move(Utils::southOne(Utils::southOne(endSquare)), endSquare, MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                mMoves.push_back(move);
            }
        }
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::White);
        attacks &= checkers;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
                if (mBoard->isLegal(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight));
                }
            } else {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasiveBlackPawnMoves(Bitboard checkers)
{
    Bitboard pawns = mBoard->getBlackPawns();
    Square kingSquare = Utils::getSquare(mBoard->getKing(Color::Black));

    // pawn pushes
    Bitboard onePawnPush = Utils::southOne(pawns) & mBoard->getEmptySquares();
    Bitboard twoPawnPush = Utils::southOne(onePawnPush & Utils::SIXTH_RANK) & mBoard->getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare)) {
            if (Utils::getBitboard(endSquare) & Utils::FIRST_RANK) {
                if (mBoard->isLegal(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                    mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen));
                    mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook));
                    mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                    mMoves.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight));
                }
            } else {
                Move move(Utils::northOne(endSquare), endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare)) {
            Move move(Utils::northOne(Utils::northOne(endSquare)), endSquare, MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                mMoves.push_back(move);
            }
        }
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::Black);
        attacks &= checkers;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::FIRST_RANK) {
                if (mBoard->isLegal(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                mMoves.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight));
                }
            } else {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
    }
}
void MoveGen::generateLegalEvasiveKnightMoves(Color color, Bitboard checkers)
{
    if (!Utils::isOneHot(checkers)) {
        return;
    }

    Bitboard knights = mBoard->getKnights(color);
    Bitboard blockers = mBoard->getPieces(color);
    Square kingSquare = Utils::getSquare(mBoard->getKing(color));

    while (knights) {
        Square startSquare = Utils::popLSB(knights);
        Bitboard attacks = mAttack->getKnightAttacks(startSquare);
        attacks &= ~blockers;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare) || attacks & checkers) {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasiveBishopMoves(Color color, Bitboard checkers)
{
    if (!Utils::isOneHot(checkers)) {
        return;
    }

    Bitboard bishops = mBoard->getBishops(color);
    Bitboard blockers = mBoard->getAllPieces();
    Bitboard ownPieces = mBoard->getPieces(color);
    Square kingSquare = Utils::getSquare(mBoard->getKing(color));

    while (bishops) {
        Square startSquare = Utils::popLSB(bishops);
        Bitboard attacks = mAttack->getBishopAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare) || attacks & checkers) {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasiveRookMoves(Color color, Bitboard checkers) 
{
    if (!Utils::isOneHot(checkers)) {
        return;
    }

    Bitboard rooks = mBoard->getRooks(color);
    Bitboard blockers = mBoard->getAllPieces();
    Bitboard ownPieces = mBoard->getPieces(color);
    Square kingSquare = Utils::getSquare(mBoard->getKing(color));

    while (rooks) {
        Square startSquare = Utils::popLSB(rooks);
        Bitboard attacks = mAttack->getRookAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare) || attacks & checkers) {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasiveQueenMoves(Color color, Bitboard checkers)
{
    if (!Utils::isOneHot(checkers)) {
        return;
    }

    Bitboard queens = mBoard->getQueens(color);
    Bitboard blockers = mBoard->getAllPieces();
    Bitboard ownPieces = mBoard->getPieces(color);
    Square kingSquare = Utils::getSquare(mBoard->getKing(color));

    while (queens) {
        Square startSquare = Utils::popLSB(queens);
        Bitboard attacks = mAttack->getQueenAttacks(startSquare, blockers);
        attacks &= ~ownPieces;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare) || attacks & checkers) {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    mMoves.push_back(move);
                }
            }
        }
    }
}