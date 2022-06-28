#include "ChessBoard.h"


ChessBoard::ChessBoard(Attack* ptr, const std::string& layout)
    : mAttack(ptr), mPieceBB{}, mTurn(Color::White), mCastle{}, mSquareBoard{}, mMoveList{}
{
    updateChessBoard(layout);
    mMoveList.reserve(200);
    mCastlingList.reserve(200);
    mPinnedList.reserve(200);
    mEnpassantList.reserve(200);
    //mCapturedList.reserve(200);
}

Bitboard ChessBoard::getWhitePawns() const noexcept
{
    return mPieceBB[to_int(PieceSets::WhitePawns)];
}

Bitboard ChessBoard::getWhiteKnights() const noexcept
{
    return mPieceBB[to_int(PieceSets::WhiteKnights)];
}

Bitboard ChessBoard::getWhiteBishops() const noexcept
{
    return mPieceBB[to_int(PieceSets::WhiteBishops)];
}

Bitboard ChessBoard::getWhiteRooks() const noexcept
{
    return mPieceBB[to_int(PieceSets::WhiteRooks)];
}

Bitboard ChessBoard::getWhiteQueens() const noexcept
{
    return mPieceBB[to_int(PieceSets::WhiteQueens)];
}

Bitboard ChessBoard::getWhiteKing() const noexcept
{
    return mPieceBB[to_int(PieceSets::WhiteKing)];
}

Bitboard ChessBoard::getBlackPawns() const noexcept
{
    return mPieceBB[to_int(PieceSets::BlackPawns)];
}

Bitboard ChessBoard::getBlackKnights() const noexcept
{
    return mPieceBB[to_int(PieceSets::BlackKnights)];
}

Bitboard ChessBoard::getBlackBishops() const noexcept
{
    return mPieceBB[to_int(PieceSets::BlackBishops)];
}

Bitboard ChessBoard::getBlackRooks() const noexcept
{
    return mPieceBB[to_int(PieceSets::BlackRooks)];
}

Bitboard ChessBoard::getBlackQueens() const noexcept
{
    return mPieceBB[to_int(PieceSets::BlackQueens)];
}

Bitboard ChessBoard::getBlackKing() const noexcept
{
    return mPieceBB[to_int(PieceSets::BlackKing)];
}

Bitboard ChessBoard::getWhitePieces() const noexcept
{
    return mPieceBB[to_int(PieceSets::WhitePieces)];
}

Bitboard ChessBoard::getBlackPieces() const noexcept
{
    return mPieceBB[to_int(PieceSets::BlackPieces)];
}

Bitboard ChessBoard::getAllPieces() const noexcept
{
    return mPieceBB[to_int(PieceSets::AllPieces)];
}

Bitboard ChessBoard::getEmptySquares() const noexcept
{
    return mPieceBB[to_int(PieceSets::EmptySquares)];
}

Bitboard ChessBoard::getPawns(Color color) const noexcept
{
    return mPieceBB[to_int(PieceSets::WhitePawns) + to_int(color)];
}

Bitboard ChessBoard::getKnights(Color color) const noexcept
{
    return mPieceBB[to_int(PieceSets::WhiteKnights) + to_int(color)];
}

Bitboard ChessBoard::getBishops(Color color) const noexcept
{
    return mPieceBB[to_int(PieceSets::WhiteBishops) + to_int(color)];
}

Bitboard ChessBoard::getRooks(Color color) const noexcept
{
    return mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(color)];
}

Bitboard ChessBoard::getQueens(Color color) const noexcept
{
    return mPieceBB[to_int(PieceSets::WhiteQueens) + to_int(color)];
}

Bitboard ChessBoard::getKing(Color color) const noexcept
{
    return mPieceBB[to_int(PieceSets::WhiteKing) + to_int(color)];
}

Bitboard ChessBoard::getPieces(Color color) const noexcept
{
    return mPieceBB[to_int(PieceSets::WhitePieces) + to_int(color)];
}

Color ChessBoard::getTurn() const noexcept
{
    return mTurn;
}

bool ChessBoard::getWhiteCastleRights(Castling side) const noexcept
{
    return mCastle[to_int(side)];
}

bool ChessBoard::getBlackCastleRights(Castling side) const noexcept
{
    return mCastle[2 + to_int(side)];
}

std::vector<Move> ChessBoard::getMoveList() const noexcept
{
    return mMoveList;
}

void ChessBoard::updateBitboards(const std::vector<std::string>& piecesByRank) noexcept
{
    // clear the bitboards
    for (PieceSets i = PieceSets::WhitePawns; i < PieceSets::WhitePieces; ++i) {
        mPieceBB[to_int(i)] = 0;
    }

    uint64_t shiftIndex = 56;
    for (auto i: piecesByRank) {
        for (auto j: i) {
            if (std::isdigit(j)) {
                shiftIndex += j - '0';
            } else {
                if (j == 'P') {
                    mPieceBB[to_int(PieceSets::WhitePawns)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'N') {
                    mPieceBB[to_int(PieceSets::WhiteKnights)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'B') {
                    mPieceBB[to_int(PieceSets::WhiteBishops)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'R') {
                    mPieceBB[to_int(PieceSets::WhiteRooks)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'Q') {
                    mPieceBB[to_int(PieceSets::WhiteQueens)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'K') {
                    mPieceBB[to_int(PieceSets::WhiteKing)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'p') {
                    mPieceBB[to_int(PieceSets::BlackPawns)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'n') {
                    mPieceBB[to_int(PieceSets::BlackKnights)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'b') {
                    mPieceBB[to_int(PieceSets::BlackBishops)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'r') {
                    mPieceBB[to_int(PieceSets::BlackRooks)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'q') {
                    mPieceBB[to_int(PieceSets::BlackQueens)] |= Bitboard(1) << shiftIndex;
                } else if (j == 'k') {
                    mPieceBB[to_int(PieceSets::BlackKing)] |= Bitboard(1) << shiftIndex;
                }
                ++shiftIndex;
            }
        }
        shiftIndex -= 16;
    }
    updateRedundantBitboards();
}

void ChessBoard::updateRedundantBitboards() noexcept
{
    mPieceBB[to_int(PieceSets::WhitePieces)] = mPieceBB[to_int(PieceSets::WhitePawns)] | mPieceBB[to_int(PieceSets::WhiteKnights)] |
                                               mPieceBB[to_int(PieceSets::WhiteBishops)] | mPieceBB[to_int(PieceSets::WhiteRooks)] | 
                                               mPieceBB[to_int(PieceSets::WhiteQueens)] | mPieceBB[to_int(PieceSets::WhiteKing)];
    mPieceBB[to_int(PieceSets::BlackPieces)] = mPieceBB[to_int(PieceSets::BlackPawns)] | mPieceBB[to_int(PieceSets::BlackKnights)] |
                                               mPieceBB[to_int(PieceSets::BlackBishops)] | mPieceBB[to_int(PieceSets::BlackRooks)] | 
                                               mPieceBB[to_int(PieceSets::BlackQueens)] | mPieceBB[to_int(PieceSets::BlackKing)];
    mPieceBB[to_int(PieceSets::AllPieces)] = mPieceBB[to_int(PieceSets::WhitePieces)] | mPieceBB[to_int(PieceSets::BlackPieces)];
    mPieceBB[to_int(PieceSets::EmptySquares)] = ~mPieceBB[to_int(PieceSets::AllPieces)];
}

void ChessBoard::makeMove(Move& nextMove)
{
    Square startingSquare = nextMove.getStartingSquare();
    Square endingSquare = nextMove.getEndingSquare();
    Bitboard startBitboard = Utils::getBitboard(startingSquare);
    Bitboard endBitboard = Utils::getBitboard(endingSquare);
    MoveType type = nextMove.getMoveType();
    PieceSets movedPiece = mSquareBoard[to_int(startingSquare)];
    PieceSets capturedPiece = mSquareBoard[to_int(endingSquare)];
    nextMove.setCapturedPiece(capturedPiece);

    mMoveList.push_back(nextMove);
    mPinnedList.push_back(mPinnedPieces);
    mCastlingList.push_back(mCastle);
    mEnpassantList.push_back(mEnpassantTarget);

    mPieceBB[to_int(movedPiece)] ^= startBitboard | endBitboard;
    mPieceBB[to_int(capturedPiece)] &= ~endBitboard;
    mSquareBoard[to_int(startingSquare)] = PieceSets::EmptySquares;
    mSquareBoard[to_int(endingSquare)] = movedPiece;
    mEnpassantTarget = 0;

    if (type == MoveType::Castle) {
        if (to_int(startingSquare) < to_int(endingSquare)) {
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] ^= Utils::eastOne(startBitboard) | Utils::eastOne(endBitboard);
            mSquareBoard[to_int(Utils::eastOne(endingSquare))] = PieceSets::EmptySquares;
            mSquareBoard[to_int(Utils::eastOne(startingSquare))] = PieceSets(to_int(PieceSets::WhiteRooks) + to_int(mTurn));
        } else {
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] ^= Utils::westOne(startBitboard) | Utils::westOne(Utils::westOne(endBitboard));
            mSquareBoard[to_int(Utils::westOne(Utils::westOne(endingSquare)))] = PieceSets::EmptySquares;
            mSquareBoard[to_int(Utils::westOne(startingSquare))] = PieceSets(to_int(PieceSets::WhiteRooks) + to_int(mTurn));
        }
    } else if (type == MoveType::Promotion) {
        PromotionPiece promotion = nextMove.getPromotionPiece();
        mPieceBB[to_int(movedPiece)] &= ~endBitboard;
        mPieceBB[(to_int(promotion) << 1) + 2 + to_int(mTurn)] |= endBitboard;
        mSquareBoard[to_int(endingSquare)] = PieceSets((to_int(promotion) << 1) + 2 + to_int(mTurn));
    } else if (type == MoveType::Enpassant) {
        if (movedPiece == PieceSets::WhitePawns) {
            mPieceBB[to_int(PieceSets::BlackPawns)] &= ~Utils::southOne(endBitboard);
            mSquareBoard[to_int(Utils::southOne(endingSquare))] = PieceSets::EmptySquares;
        } else {
            mPieceBB[to_int(PieceSets::WhitePawns)] &= ~Utils::northOne(endBitboard);
            mSquareBoard[to_int(Utils::northOne(endingSquare))] = PieceSets::EmptySquares;
        }
    } else if (movedPiece == PieceSets::WhitePawns && to_int(endingSquare) - to_int(startingSquare) == 16) {
        mEnpassantTarget = Utils::southOne(endBitboard);
    } else if (movedPiece == PieceSets::BlackPawns && to_int(startingSquare) - to_int(endingSquare) == 16) {
        mEnpassantTarget = Utils::northOne(endBitboard);
    }

    // mMoveList.push_back(nextMove);
    // mPinnedList.push_back(mPinnedPieces);
    // mCastlingList.push_back(mCastle);
    // mEnpassantList.push_back(mEnpassantTarget);

    updateRedundantBitboards();
    updateCastlingRights();
    mTurn = !mTurn;
    updatePinnedPieces(mTurn);
}


void ChessBoard::updateSquareBoard() noexcept
{
    for (Square i = Square::A1; i < Square::Null; ++i) {
        mSquareBoard[to_int(i)] = PieceSets::EmptySquares;
    }

    for (PieceSets i = PieceSets::WhitePawns; i < PieceSets::WhitePieces; ++i) {
        Bitboard pieces = mPieceBB[to_int(i)];
        while (pieces) {
            mSquareBoard[to_int(Utils::popLSB(pieces))] = i;
        }
    }
}

void ChessBoard::printSquareBoard() const noexcept
{
    std::cout << "-----------------" << std::endl;
    std::string row;
    for (Square i = Square::Null; i > Square::A1; --i) {
        row += "|";
        if (mSquareBoard[to_int(i) - 1] == PieceSets::WhitePawns) {
            row += "P";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::WhiteKnights) {
            row += "N";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::WhiteBishops) {
            row += "B";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::WhiteRooks) {
            row += "R";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::WhiteQueens) {
            row += "Q";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::WhiteKing) {
            row += "K";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::BlackPawns) {
            row += "p";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::BlackKnights) {
            row += "n";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::BlackBishops) {
            row += "b";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::BlackRooks) {
            row += "r";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::BlackQueens) {
            row += "q";
        } else if (mSquareBoard[to_int(i) - 1] == PieceSets::BlackKing) {
            row += "k";
        } else {
            row += " ";
        }
        //std::cout << "|";

        // if last square in row
        if (to_int(i) % 8 == 1) {
            row += "|";
            std::reverse(row.begin(), row.end());
            std::cout << row << std::endl;
            std::cout << "-----------------" << std::endl;
            row.clear();
        } 
    }
    std::cout << std::endl;
}

void ChessBoard::undoMove() 
{
    mTurn = !mTurn;
    Move prevMove = mMoveList.back();
    mCastle = mCastlingList.back();
    mPinnedPieces = mPinnedList.back();
    mEnpassantTarget = mEnpassantList.back();

    Square startingSquare = prevMove.getStartingSquare();
    Square endingSquare = prevMove.getEndingSquare();
    Bitboard startBitboard = Utils::getBitboard(startingSquare);
    Bitboard endBitboard = Utils::getBitboard(endingSquare);
    MoveType type = prevMove.getMoveType();
    PieceSets movedPiece = mSquareBoard[to_int(endingSquare)];
    PieceSets capturedPiece = prevMove.getCapturedPiece();

    mPieceBB[to_int(movedPiece)] ^= startBitboard | endBitboard;
    mPieceBB[to_int(capturedPiece)] |= endBitboard;
    mSquareBoard[to_int(startingSquare)] = movedPiece;
    mSquareBoard[to_int(endingSquare)] = capturedPiece;

    if (type == MoveType::Castle) {
        if (to_int(startingSquare) < to_int(endingSquare)) {
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] ^= Utils::eastOne(startBitboard) | Utils::eastOne(endBitboard);
            mSquareBoard[to_int(Utils::eastOne(endingSquare))] = PieceSets(to_int(PieceSets::WhiteRooks) + to_int(mTurn));
            mSquareBoard[to_int(Utils::eastOne(startingSquare))] = PieceSets::EmptySquares;
        } else {
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] ^= Utils::westOne(startBitboard) | Utils::westOne(Utils::westOne(endBitboard));
            mSquareBoard[to_int(Utils::westOne(Utils::westOne(endingSquare)))] = PieceSets(to_int(PieceSets::WhiteRooks) + to_int(mTurn));
            mSquareBoard[to_int(Utils::westOne(startingSquare))] = PieceSets::EmptySquares;
        }
    } else if (type == MoveType::Promotion) {
        mPieceBB[to_int(movedPiece)] &= ~startBitboard;
        mPieceBB[to_int(PieceSets::WhitePawns) + to_int(mTurn)] |= startBitboard;
        mSquareBoard[to_int(startingSquare)] = PieceSets(to_int(PieceSets::WhitePawns) + to_int(mTurn));
    } else if (type == MoveType::Enpassant) {
        if (movedPiece == PieceSets::WhitePawns) {
            mPieceBB[to_int(PieceSets::BlackPawns)] |= Utils::southOne(endBitboard);
            mSquareBoard[to_int(Utils::southOne(endingSquare))] = PieceSets::BlackPawns;
        } else {
            mPieceBB[to_int(PieceSets::WhitePawns)] |= Utils::northOne(endBitboard);
            mSquareBoard[to_int(Utils::northOne(endingSquare))] = PieceSets::WhitePawns;
        }
    }
    updateRedundantBitboards();

    mMoveList.pop_back();
    mPinnedList.pop_back();
    mCastlingList.pop_back();
    mEnpassantList.pop_back();
}

Bitboard ChessBoard::getPinnedPieces() const noexcept
{
    return mPinnedPieces;
}

void ChessBoard::updateCastlingRights() noexcept
{
    Bitboard whiteRooks = getWhiteRooks();
    Bitboard whiteKing = getWhiteKing();
    Bitboard blackRooks = getBlackRooks();
    Bitboard blackKing = getBlackKing();

    mCastle[(to_int(Color::White) << 1) + to_int(Castling::Kingside)] = mCastle[(to_int(Color::White) << 1) + to_int(Castling::Kingside)] && whiteRooks & Utils::getBitboard(Square::H1) && whiteKing & Utils::getBitboard(Square::E1);
    mCastle[(to_int(Color::White) << 1) + to_int(Castling::Queenside)] = mCastle[(to_int(Color::White) << 1) + to_int(Castling::Queenside)] && whiteRooks & Utils::getBitboard(Square::A1) && whiteKing & Utils::getBitboard(Square::E1);
    mCastle[(to_int(Color::Black) << 1) + to_int(Castling::Kingside)] = mCastle[(to_int(Color::Black) << 1) + to_int(Castling::Kingside)] && blackRooks & Utils::getBitboard(Square::H8) && blackKing & Utils::getBitboard(Square::E8);
    mCastle[(to_int(Color::Black) << 1) + to_int(Castling::Queenside)] = mCastle[(to_int(Color::Black) << 1) + to_int(Castling::Queenside)] && blackRooks & Utils::getBitboard(Square::A8) && blackKing & Utils::getBitboard(Square::E8);
}

void ChessBoard::updatePinnedPieces(Color turn) noexcept
{
    mPinnedPieces = squareBlockers(Utils::getSquare(getKing(turn)), turn);
}

bool ChessBoard::getCastleRights(Castling side, Color turn) const noexcept
{
    return mCastle[(to_int(turn) << 1) + to_int(side)];
}

bool ChessBoard::isLegal(const Move& move)
{
    MoveType type = move.getMoveType();
    Square startSquare = move.getStartingSquare();
    Square endSquare = move.getEndingSquare();
    Bitboard startLoc = Utils::getBitboard(startSquare);
    Bitboard endLoc = Utils::getBitboard(endSquare);
    Bitboard blockers = getAllPieces();
    //Bitboard enemyPieces = getPieces(!mTurn);

    if (type == MoveType::Castle) {
        if (endLoc > startLoc) {
            return !(isSquareUnderAttack(Utils::eastOne(startSquare), mTurn, blockers) ||
                   isSquareUnderAttack(endSquare, mTurn, blockers));
        } else {
            return !(isSquareUnderAttack(Utils::westOne(startSquare), mTurn, blockers) ||
                   isSquareUnderAttack(endSquare, mTurn, blockers));
        }
    } else if (type == MoveType::Enpassant) {
        // make the move
        if (mTurn == Color::White) {
            blockers ^= startLoc | Utils::southOne(endLoc) | endLoc;
            mPieceBB[to_int(PieceSets::BlackPawns)] &= ~Utils::southOne(endLoc);
            mPieceBB[to_int(PieceSets::WhitePawns)] ^= startLoc | endLoc;
        } else {
            blockers ^= startLoc | Utils::northOne(endLoc) | endLoc;
            mPieceBB[to_int(PieceSets::WhitePawns)] &= ~Utils::northOne(endLoc);
            mPieceBB[to_int(PieceSets::BlackPawns)] ^= startLoc | endLoc;
        }

        bool legal = !isKingUnderAttack(mTurn, blockers);
        
        // undo the move
        if (mTurn == Color::White) {
            mPieceBB[to_int(PieceSets::BlackPawns)] |= Utils::southOne(endLoc);
            mPieceBB[to_int(PieceSets::WhitePawns)] ^= startLoc | endLoc;
        } else {
            mPieceBB[to_int(PieceSets::BlackPawns)] ^= startLoc | endLoc;
            mPieceBB[to_int(PieceSets::WhitePawns)] |= Utils::northOne(endLoc);
        }

        return legal;
    } else if (mSquareBoard[to_int(startSquare)] == PieceSets::WhiteKing || mSquareBoard[to_int(startSquare)] == PieceSets::BlackKing) {
        blockers &= ~Utils::getBitboard(startSquare);
        return !isSquareUnderAttack(endSquare, mTurn, blockers);
    }

    // All other move types
    return isLegalPinnedMove(startSquare, endSquare, mTurn);
}

bool ChessBoard::isSquareUnderAttack(Square sq, Color turn, Bitboard blockers)
{
    return squareAttackers(sq, !turn, blockers) & getPieces(!turn);
}

Bitboard ChessBoard::squareAttackers(Square sq, Color color, Bitboard blockers) const
{
    Bitboard pawns = getPawns(color);
    Bitboard knights = getKnights(color);
    Bitboard bishopQueens = getBishops(color) | getQueens(color);
    Bitboard rookQueens = getRooks(color) | getQueens(color);
    Bitboard king = getKing(color);

    return (mAttack->getPawnAttacks(sq, !color) & pawns) |
           (mAttack->getKnightAttacks(sq) & knights) |
           (mAttack->getBishopAttacks(sq, blockers) & bishopQueens) |
           (mAttack->getRookAttacks(sq, blockers) & rookQueens) |
           (mAttack->getKingAttacks(sq) & king);
}

Bitboard ChessBoard::squareBlockers(Square sq, Color turn)
{
    Color enemyColor = !turn;
    Bitboard blockers = 0;
    Bitboard enemyBishopQueens = getBishops(enemyColor) | getQueens(enemyColor);
    Bitboard enemyRookQueens = getRooks(enemyColor) | getQueens(enemyColor);
    Bitboard sliderAttackers = (mAttack->getBishopAttacks(sq, 0) & enemyBishopQueens) |
                               (mAttack->getRookAttacks(sq, 0) & enemyRookQueens);
    Bitboard allPieces = getAllPieces();

    while (sliderAttackers) {
        Square sliderSquare = Utils::popLSB(sliderAttackers);
        Bitboard between = mAttack->inBetween(sq, sliderSquare);
        between &= allPieces;
        if (Utils::isOneHot(between)) {
            blockers |= between;
        }
    }
    return blockers;
}

bool ChessBoard::isLegalPinnedMove(Square startSquare, Square endSquare, Color turn)
{
    Bitboard pinned = Utils::getBitboard(startSquare) & getPinnedPieces();

    if (!pinned) {
        return true;
    }

    return mAttack->inLine(startSquare, endSquare) & getKing(turn);
}

void ChessBoard::updateChessBoard(const std::string& layout)
{
    // one vector to hold all the necessary fields from layout
    // one vector to hold the pieces by rank, 0 index starts at rank 8
    std::vector<std::string> fenFields;
    std::vector<std::string> pieceRank;
    std::stringstream s(layout);
    std::string item;
    while (s >> item) {
        fenFields.push_back(item);
    }
    s = std::stringstream(fenFields[0]);

    // initializing mPieceBB
    while(std::getline(s, item, '/')) {
        pieceRank.push_back(item);
    }
    updateBitboards(pieceRank);

    // initializing mTurn
    if (fenFields[1] == "w") {
        mTurn = Color::White;
    } else {
        mTurn = Color::Black;
    }

    // initializing mWhiteCastle and mBlackCastle
    mCastle[(to_int(Color::White) << 1) + to_int(Castling::Kingside)] = false;
    mCastle[(to_int(Color::White) << 1) + to_int(Castling::Queenside)] = false;
    mCastle[(to_int(Color::Black) << 1) + to_int(Castling::Kingside)] = false;
    mCastle[(to_int(Color::Black) << 1) + to_int(Castling::Queenside)] = false;
    for (auto i: fenFields[2]) {
        switch (i) {
            case '-':
                break;
            case 'K':
                mCastle[(to_int(Color::White) << 1) + to_int(Castling::Kingside)] = true;
                break;
            case 'Q':
                mCastle[(to_int(Color::White) << 1) + to_int(Castling::Queenside)] = true;
                break;
            case 'k':
                mCastle[(to_int(Color::Black) << 1) + to_int(Castling::Kingside)] = true;
                break;
            case 'q':
                mCastle[(to_int(Color::Black) << 1) + to_int(Castling::Queenside)] = true;
                break;
        }
    }

    // initializing enpassant targets
    if (fenFields[3] == "-") {
        mEnpassantTarget = 0;
    } else {
        mEnpassantTarget = Utils::getBitboard(Square(8 * (fenFields[3][1] - '1') + (fenFields[3][0] - 'a')));
    }
    updateSquareBoard();
    updatePinnedPieces(mTurn);
}

bool ChessBoard::isKingUnderAttack(Color turn, Bitboard blockers)
{
    return isSquareUnderAttack(Utils::getSquare(getKing(turn)), turn, blockers);
}

Bitboard ChessBoard::getKingAttackers(Color turn, Bitboard blockers)
{
    return squareAttackers(Utils::getSquare(getKing(turn)), !turn, blockers);
}

Bitboard ChessBoard::getEnpassantTarget() const noexcept
{
    return mEnpassantTarget;
}

bool ChessBoard::validateSquareBoard() const
{
    for (Square sq = Square::A1; sq != Square::Null; ++sq) {
        PieceSets piece = mSquareBoard[to_int(sq)];
        if (!(mPieceBB[to_int(piece)] & Utils::getBitboard(sq))){
            return false;
        }
    }
    return true;
}