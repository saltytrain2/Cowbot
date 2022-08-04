#include "MoveGen.h"

MoveGen::MoveGen(ChessBoard* src, Attack* attack) 
    : mAttack(attack), mBoard(src)
{
}

std::vector<Move> MoveGen::generateLegalMoves(Color side)
{
    std::vector<Move> moveList;
    moveList.reserve(240);

    Bitboard checkers = mBoard->getKingAttackers(side, mBoard->getAllPieces());

    if (checkers) {
        generateLegalEvasivePawnMoves(side, checkers, moveList);
        generateLegalEvasiveKnightMoves(side, checkers, moveList);
        generateLegalEvasiveBishopMoves(side, checkers, moveList);
        generateLegalEvasiveRookMoves(side, checkers, moveList);
        generateLegalEvasiveQueenMoves(side, checkers, moveList);
    } else {
        generateLegalNonEvasivePawnMoves(side, moveList);
        generateLegalNonEvasiveKnightMoves(side, moveList);
        generateLegalNonEvasiveBishopMoves(side, moveList);
        generateLegalNonEvasiveRookMoves(side, moveList);
        generateLegalNonEvasiveQueenMoves(side, moveList);
    }
    generateLegalKingMoves(side, moveList);

    return moveList;
}

std::vector<Move> MoveGen::generateLegalCaptures(Color side)
{
    std::vector<Move> legalMoves = generateLegalMoves(side);
    Bitboard enemyPieces = mBoard->getPieces(!side);
    std::vector<Move> legalCaptures;

    for (auto move: legalMoves) {
        if (Utils::getBitboard(move.getEndingSquare()) & enemyPieces) {
            legalCaptures.push_back(move);
        }
    }
    return legalCaptures;
}

void MoveGen::generateLegalNonEvasivePawnMoves(Color color, std::vector<Move>& moveList)
{
    color == Color::White ? generateLegalNonEvasiveWhitePawnMoves(moveList) : generateLegalNonEvasiveBlackPawnMoves(moveList);
}

void MoveGen::generateLegalNonEvasiveWhitePawnMoves(std::vector<Move>& moveList)
{
    Bitboard pawns = mBoard->getWhitePawns();
    Bitboard enemyPieces = mBoard->getBlackPieces();
    Bitboard enpassantTarget = mBoard->getEnpassantTarget();

    // pawn pushes
    Bitboard onePawnPush = Utils::northOne(pawns) & mBoard->getEmptySquares();
    Bitboard twoPawnPush = Utils::northOne(onePawnPush & Utils::THIRD_RANK) & mBoard->getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
            if (mBoard->isLegal(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                moveList.emplace_back(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen);
                moveList.emplace_back(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook);
                moveList.emplace_back(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop);
                moveList.emplace_back(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight);
            }
        } else {
            Move move(Utils::southOne(endSquare), endSquare, MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                moveList.push_back(move);
            }
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        Move move(Utils::southOne(Utils::southOne(endSquare)), endSquare, MoveType::Quiet);
        if (mBoard->isLegal(move)) {
            moveList.push_back(move);
        }
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::White);
        if (attacks & enpassantTarget) {
            Move move(startSquare, Utils::getSquare(enpassantTarget), MoveType::Enpassant);
            if (mBoard->isLegal(move)) {
                moveList.push_back(move);
            }
        }

        attacks &= enemyPieces;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
                if (mBoard->isLegal(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                moveList.emplace_back(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen);
                moveList.emplace_back(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook);
                moveList.emplace_back(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop);
                moveList.emplace_back(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight);
                }
            } else {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalNonEvasiveBlackPawnMoves(std::vector<Move>& moveList)
{
    Bitboard pawns = mBoard->getBlackPawns();
    Bitboard enemyPieces = mBoard->getWhitePieces();
    Bitboard enpassantTarget = mBoard->getEnpassantTarget();

    // pawn pushes
    Bitboard onePawnPush = Utils::southOne(pawns) & mBoard->getEmptySquares();
    Bitboard twoPawnPush = Utils::southOne(onePawnPush & Utils::SIXTH_RANK) & mBoard->getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        if (Utils::getBitboard(endSquare) & Utils::FIRST_RANK) {
            if (mBoard->isLegal(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                moveList.emplace_back(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen);
                moveList.emplace_back(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook);
                moveList.emplace_back(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop);
                moveList.emplace_back(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight);
            }
        } else {
            Move move(Utils::northOne(endSquare), endSquare, MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                moveList.push_back(move);
            }
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        Move move(Utils::northOne(Utils::northOne(endSquare)), endSquare, MoveType::Quiet);
        if (mBoard->isLegal(move)) {
            moveList.push_back(move);
        }
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::Black);
        if (attacks & enpassantTarget) {
            Move move(startSquare, Utils::getSquare(enpassantTarget), MoveType::Enpassant);
            if (mBoard->isLegal(move)) {
                moveList.push_back(move);
            }
        }
        attacks &= enemyPieces;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::FIRST_RANK) {
                if (mBoard->isLegal(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                moveList.emplace_back(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen);
                moveList.emplace_back(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook);
                moveList.emplace_back(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop);
                moveList.emplace_back(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight);
                }
            } else {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalNonEvasiveKnightMoves(Color color, std::vector<Move>& moveList)
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
                moveList.push_back(move);
            }
        }
    }
}

void MoveGen::generateLegalNonEvasiveBishopMoves(Color color, std::vector<Move>& moveList)
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
                moveList.push_back(move);
            }
        }
    }
}

void MoveGen::generateLegalNonEvasiveRookMoves(Color color, std::vector<Move>& moveList)
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
                moveList.push_back(move);
            }
        }
    }
}

void MoveGen::generateLegalNonEvasiveQueenMoves(Color color, std::vector<Move>& moveList)
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
                moveList.push_back(move);
            }
        }
    }
}

void MoveGen::generateLegalKingMoves(Color color, std::vector<Move>& moveList)
{
    Bitboard king = mBoard->getKing(color);
    bool kingsideRights = mBoard->getCastleRights(Castling::Kingside, color);
    bool queensideRights = mBoard->getCastleRights(Castling::Queenside, color);
    Bitboard blockers = mBoard->getPieces(color);
    Bitboard allPieces = mBoard->getAllPieces();

    Square startSquare = Utils::popLSB(king);
    Bitboard attacks = mAttack->getKingAttacks(startSquare);
    attacks &= ~blockers;
    while (attacks) {
        Move move(startSquare, Utils::popLSB(attacks), MoveType::Quiet);
        if (mBoard->isLegal(move)) {
            moveList.push_back(move);
        }
    }

    if (!mBoard->isKingUnderAttack(color, allPieces)) {
        // Castling
        Bitboard startBitboard = Utils::getBitboard(startSquare);
        if (kingsideRights) {
            Bitboard kingsideOne = Utils::eastOne(startBitboard);
            Bitboard kingsideTwo = Utils::eastOne(kingsideOne);
            if (!((kingsideOne | kingsideTwo) & allPieces)) {
                Move move(startSquare, Utils::getSquare(kingsideTwo), MoveType::Castle);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
        if (queensideRights) {
            Bitboard queensideOne = Utils::westOne(startBitboard);
            Bitboard queensideTwo = Utils::westOne(queensideOne);
            Bitboard queensideThree = Utils::westOne(queensideTwo);
            if (!((queensideOne | queensideTwo | queensideThree) & allPieces)) {
                Move move(startSquare, Utils::getSquare(queensideTwo), MoveType::Castle);
                if (mBoard->isLegal(move)) {
                     moveList.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasivePawnMoves(Color color, Bitboard checkers, std::vector<Move>& moveList)
{
    color == Color::White ? generateLegalEvasiveWhitePawnMoves(checkers, moveList) : generateLegalEvasiveBlackPawnMoves(checkers, moveList);
}

void MoveGen::generateLegalEvasiveWhitePawnMoves(Bitboard checkers, std::vector<Move>& moveList)
{
    if (!Utils::isOneHot(checkers)) {
        return;
    }

    Bitboard pawns = mBoard->getWhitePawns();
    Square kingSquare = Utils::getSquare(mBoard->getKing(Color::White));
    Bitboard enpassantTarget = mBoard->getEnpassantTarget();

    // pawn pushes
    Bitboard onePawnPush = Utils::northOne(pawns) & mBoard->getEmptySquares();
    Bitboard twoPawnPush = Utils::northOne(onePawnPush & Utils::THIRD_RANK) & mBoard->getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare)) {
            if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
                if (mBoard->isLegal(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                    moveList.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen));
                    moveList.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook));
                    moveList.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                    moveList.push_back(Move(Utils::southOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight));
                }
            } else {
                Move move(Utils::southOne(endSquare), endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare)) {
            Move move(Utils::southOne(Utils::southOne(endSquare)), endSquare, MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                moveList.push_back(move);
            }
        }
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::White);
        if (attacks & enpassantTarget) {
            Move move(startSquare, Utils::getSquare(enpassantTarget), MoveType::Enpassant);
            if (mBoard->isLegal(move)) {
                moveList.push_back(move);
            }
        }
        attacks &= checkers;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::EIGHTH_RANK) {
                if (mBoard->isLegal(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                moveList.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen));
                moveList.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook));
                moveList.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                moveList.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight));
                }
            } else {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasiveBlackPawnMoves(Bitboard checkers, std::vector<Move>& moveList)
{
    if (!Utils::isOneHot(checkers)) {
        return;
    }

    Bitboard pawns = mBoard->getBlackPawns();
    Square kingSquare = Utils::getSquare(mBoard->getKing(Color::Black));
    Bitboard enpassantTarget = mBoard->getEnpassantTarget();

    // pawn pushes
    Bitboard onePawnPush = Utils::southOne(pawns) & mBoard->getEmptySquares();
    Bitboard twoPawnPush = Utils::southOne(onePawnPush & Utils::SIXTH_RANK) & mBoard->getEmptySquares();
    while (onePawnPush) {
        Square endSquare = Utils::popLSB(onePawnPush);
        if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare)) {
            if (Utils::getBitboard(endSquare) & Utils::FIRST_RANK) {
                if (mBoard->isLegal(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                    moveList.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Queen));
                    moveList.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Rook));
                    moveList.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                    moveList.push_back(Move(Utils::northOne(endSquare), endSquare, MoveType::Promotion, PromotionPiece::Knight));
                }
            } else {
                Move move(Utils::northOne(endSquare), endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
    }
    while (twoPawnPush) {
        Square endSquare = Utils::popLSB(twoPawnPush);
        if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare)) {
            Move move(Utils::northOne(Utils::northOne(endSquare)), endSquare, MoveType::Quiet);
            if (mBoard->isLegal(move)) {
                moveList.push_back(move);
            }
        }
    }
    while (pawns) {
        Square startSquare = Utils::popLSB(pawns);
        Bitboard attacks = mAttack->getPawnAttacks(startSquare, Color::Black);
        if (attacks & enpassantTarget) {
            Move move(startSquare, Utils::getSquare(enpassantTarget), MoveType::Enpassant);
            if (mBoard->isLegal(move)) {
                moveList.push_back(move);
            }
        }
        attacks &= checkers;
        while (attacks) {
            Square endSquare = Utils::popLSB(attacks);
            if (Utils::getBitboard(endSquare) & Utils::FIRST_RANK) {
                if (mBoard->isLegal(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen))) {
                moveList.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Queen));
                moveList.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Rook));
                moveList.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Bishop));
                moveList.push_back(Move(startSquare, endSquare, MoveType::Promotion, PromotionPiece::Knight));
                }
            } else {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasiveKnightMoves(Color color, Bitboard checkers, std::vector<Move>& moveList)
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
            if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare) || Utils::getBitboard(endSquare) & checkers) {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasiveBishopMoves(Color color, Bitboard checkers, std::vector<Move>& moveList)
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
            if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare) || Utils::getBitboard(endSquare) & checkers) {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasiveRookMoves(Color color, Bitboard checkers, std::vector<Move>& moveList) 
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
            if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare) || Utils::getBitboard(endSquare) & checkers) {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
    }
}

void MoveGen::generateLegalEvasiveQueenMoves(Color color, Bitboard checkers, std::vector<Move>& moveList)
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
            if (mAttack->inBetween(kingSquare, Utils::getSquare(checkers)) & Utils::getBitboard(endSquare) || Utils::getBitboard(endSquare) & checkers) {
                Move move(startSquare, endSquare, MoveType::Quiet);
                if (mBoard->isLegal(move)) {
                    moveList.push_back(move);
                }
            }
        }
    }
}