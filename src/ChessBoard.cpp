#include "ChessBoard.h"


ChessBoard::ChessBoard(std::shared_ptr<Attack> ptr, const std::string& layout)
    : mAttack(ptr), mPieceBB{}, mTurn(Color::White), mCastle{}, mSquareBoard{}, mMoveList{}
{
    updateChessBoard(layout);
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
    return mCastle[to_int(Color::White)][to_int(side)];
}

bool ChessBoard::getBlackCastleRights(Castling side) const noexcept
{
    return mCastle[to_int(Color::Black)][to_int(side)];
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

int ChessBoard::makeMove(Move& nextMove) noexcept
{
    mMoveList.push_back(nextMove);
    mPinnedList.push_back(mPinnedPieces);
    mCastlingList.push_back(mCastle);
    Square startingSquare = nextMove.getStartingSquare();
    Square endingSquare = nextMove.getEndingSquare();
    MoveType type = nextMove.getMoveType();
    PieceSets movedPiece = mSquareBoard[to_int(startingSquare)];
    PieceSets capturedPiece = mSquareBoard[to_int(endingSquare)];

    mPieceBB[to_int(movedPiece)] |= Utils::getBitboard(endingSquare);
    mPieceBB[to_int(movedPiece)] &= ~Utils::getBitboard(startingSquare);

    if (capturedPiece != PieceSets::EmptySquares) {
        nextMove.setCapturedPiece(capturedPiece);
        mPieceBB[to_int(capturedPiece)] &= ~Utils::getBitboard(endingSquare);
    }

    if (type == MoveType::Castle) {
        if (to_int(startingSquare) < to_int(endingSquare)) {
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] |= Utils::eastOne(Utils::getBitboard(startingSquare));
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] &= ~Utils::eastOne(Utils::getBitboard(endingSquare));
        } else {
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] |= Utils::westOne(Utils::getBitboard(startingSquare));
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] &= ~Utils::westOne(Utils::westOne(Utils::getBitboard(endingSquare)));
        }
    } else if (type == MoveType::Promotion) {
        PromotionPiece promotion = nextMove.getPromotionPiece();
        mPieceBB[to_int(movedPiece)] &= ~Utils::getBitboard(endingSquare);
        mPieceBB[to_int(promotion) + 2 + to_int(mTurn)] |= Utils::getBitboard(endingSquare);
    } else if (type == MoveType::Enpassant) {
        if (movedPiece == PieceSets::WhitePawns) {
            mPieceBB[to_int(PieceSets::BlackPawns)] &= Utils::southOne(Utils::getBitboard(endingSquare));
            nextMove.setCapturedPiece(PieceSets::BlackPawns);
        } else {
            mPieceBB[to_int(PieceSets::WhitePawns)] &= Utils::northOne(Utils::getBitboard(endingSquare));
            nextMove.setCapturedPiece(PieceSets::WhitePawns);
        }
    }

    updateRedundantBitboards();
    updateSquareBoard();
    updateCastlingRights(movedPiece);
    updatePinnedPieces(mTurn);
    
    return 0;
}


void ChessBoard::updateSquareBoard() noexcept
{
    for (Square i = Square::A1; i < Square::Null; ++i) {
        bool changed = false;
        for (PieceSets j = PieceSets::WhitePawns; j < PieceSets::WhitePieces; ++j) {
            if (mPieceBB[to_int(j)] & Utils::getBitboard(i)) {
                mSquareBoard[to_int(i)] = j;
                changed = true;
            }
        }
        if (!changed) {
            mSquareBoard[to_int(i)] = PieceSets::EmptySquares;
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
    Move prevMove = mMoveList.back();
    mCastle = mCastlingList.back();
    mPinnedPieces = mPinnedList.back();
    mTurn = !mTurn;

    Square startingSquare = prevMove.getStartingSquare();
    Square endingSquare = prevMove.getEndingSquare();
    MoveType type = prevMove.getMoveType();

    PieceSets movedPiece = mSquareBoard[to_int(endingSquare)];
    PieceSets capturedPiece = mMoveList[mMoveList.size() - 1].getCapturedPiece();
    mPieceBB[to_int(movedPiece)] |= Utils::getBitboard(startingSquare);
    mPieceBB[to_int(movedPiece)] &= ~Utils::getBitboard(endingSquare);
    
    if (capturedPiece != PieceSets::EmptySquares) {
        mPieceBB[to_int(capturedPiece)] |= Utils::getBitboard(endingSquare);
    }

    if (type == MoveType::Castle) {
        if (to_int(startingSquare) < to_int(endingSquare)) {
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] &= ~Utils::eastOne(Utils::getBitboard(startingSquare));
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] |= Utils::eastOne(Utils::getBitboard(endingSquare));
        } else {
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] &= ~Utils::westOne(Utils::getBitboard(startingSquare));
            mPieceBB[to_int(PieceSets::WhiteRooks) + to_int(mTurn)] |= Utils::westOne(Utils::westOne(Utils::getBitboard(endingSquare)));
        }
    } else if (type == MoveType::Promotion) {
        PromotionPiece promotion = prevMove.getPromotionPiece();
        mPieceBB[to_int(promotion) + 2 + to_int(mTurn)] &= ~Utils::getBitboard(endingSquare);
    } else if (type == MoveType::Enpassant) {
        if (movedPiece == PieceSets::WhitePawns) {
            mPieceBB[to_int(PieceSets::BlackPawns)] |= Utils::southOne(Utils::getBitboard(endingSquare));
            mPieceBB[to_int(PieceSets::BlackPawns)] &= ~Utils::getBitboard(endingSquare);
        } else {
            mPieceBB[to_int(PieceSets::WhitePawns)] |= Utils::northOne(Utils::getBitboard(endingSquare));
            mPieceBB[to_int(PieceSets::BlackPawns)] &= ~Utils::getBitboard(endingSquare);
        }
    }
    updateRedundantBitboards();
    updateSquareBoard();

    mMoveList.pop_back();
    mPinnedList.pop_back();
    mCastlingList.pop_back();
}

Bitboard ChessBoard::getPinnedPieces() const noexcept
{
    return mPinnedPieces;
}

void ChessBoard::updateCastlingRights(PieceSets movedPiece) noexcept
{
    mCastlingList.push_back(mCastle);

    if (movedPiece == PieceSets::WhiteRooks) {
        Bitboard whiteRooks = getWhiteRooks();
        if (whiteRooks & Utils::getBitboard(Square::A1)) {
            mCastle[to_int(Color::White)][to_int(Castling::Queenside)] = false;
            return;
        }
        if (whiteRooks & Utils::getBitboard(Square::H1)) {
            mCastle[to_int(Color::White)][to_int(Castling::Kingside)] = false;
            return;
        }
    } else if (movedPiece == PieceSets::BlackRooks) {
        Bitboard blackRooks = getBlackRooks();
        if (blackRooks & Utils::getBitboard(Square::A8)) {
            mCastle[to_int(Color::Black)][to_int(Castling::Queenside)] = false;
            return;
        }
        if (blackRooks & Utils::getBitboard(Square::H8)) {
            mCastle[to_int(Color::Black)][to_int(Castling::Kingside)] = false;
            return;
        }
    } else if (movedPiece == PieceSets::WhiteKing) {
        mCastle[to_int(Color::White)][to_int(Castling::Queenside)] = false;
        mCastle[to_int(Color::White)][to_int(Castling::Kingside)] = false;
        return;
    } else if (movedPiece == PieceSets::BlackKing) {
        mCastle[to_int(Color::Black)][to_int(Castling::Queenside)] = false;
        mCastle[to_int(Color::Black)][to_int(Castling::Kingside)] = false;
        return;
    }
}

void ChessBoard::updatePinnedPieces(Color turn) noexcept
{
    mPinnedPieces = squareBlockers(Utils::getSquare(getKing(turn)), turn);
}

bool ChessBoard::getCastleRights(Castling side, Color turn) const noexcept
{
    return mCastle[to_int(turn)][to_int(side)];
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
            return !(isSquareUnderAttack(startSquare, mTurn, blockers) && 
                   isSquareUnderAttack(Utils::eastOne(startSquare), mTurn, blockers) &&
                   isSquareUnderAttack(endSquare, mTurn, blockers));
        } else {
            return !(isSquareUnderAttack(startSquare, mTurn, blockers) && 
                   isSquareUnderAttack(Utils::westOne(startSquare), mTurn, blockers) &&
                   isSquareUnderAttack(endSquare, mTurn, blockers));
        }
    } else if (type == MoveType::Enpassant) {
        // make the move
        if (mTurn == Color::White) {
            blockers &= ~(startLoc | Utils::southOne(endLoc));
            mPieceBB[to_int(PieceSets::BlackPawns)] &= ~Utils::southOne(endLoc);
            mPieceBB[to_int(PieceSets::WhitePawns)] |= endLoc;
            mPieceBB[to_int(PieceSets::WhitePawns)] &= ~startLoc;
        } else {
            blockers &= ~(startLoc | Utils::northOne(endLoc));
            mPieceBB[to_int(PieceSets::WhitePawns)] &= ~Utils::northOne(endLoc);
            mPieceBB[to_int(PieceSets::BlackPawns)] |= endLoc;
            mPieceBB[to_int(PieceSets::BlackPawns)] &= ~startLoc;
        }
        blockers |= endLoc;
        bool legal = isKingUnderAttack(mTurn, blockers);
        
        // undo the move
        if (mTurn == Color::White) {
            mPieceBB[to_int(PieceSets::BlackPawns)] |= Utils::southOne(endLoc);
            mPieceBB[to_int(PieceSets::WhitePawns)] &= ~endLoc;
            mPieceBB[to_int(PieceSets::WhitePawns)] |= startLoc;
        } else {
            mPieceBB[to_int(PieceSets::BlackPawns)] |= Utils::southOne(endLoc);
            mPieceBB[to_int(PieceSets::WhitePawns)] &= ~endLoc;
            mPieceBB[to_int(PieceSets::WhitePawns)] |= startLoc;
        }

        return legal;
    } else if (mSquareBoard[to_int(startSquare)] == PieceSets::WhiteKing || mSquareBoard[to_int(startSquare)] == PieceSets::BlackKing) {
        return !isSquareUnderAttack(endSquare, mTurn, blockers);
    }

    // All other move types
    return isLegalPinnedMove(startLoc, endLoc, mTurn);
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
    Bitboard blockers = 0;
    Bitboard enemyBishopQueens = getBishops(!turn) | getQueens(!turn);
    Bitboard enemyRookQueens = getRooks(!turn) | getQueens(!turn);
    Bitboard sliderAttackers = (mAttack->getBishopAttacks(sq, 0) & enemyBishopQueens) |
                               (mAttack->getRookAttacks(sq, 0) & enemyRookQueens);
    Bitboard ownPieces = getPieces(turn);

    while (sliderAttackers) {
        Square sliderSquare = Utils::popLSB(sliderAttackers);
        Bitboard between = mAttack->inBetween(sq, sliderSquare);
        between &= ownPieces;
        if (Utils::isOneHot(between)) {
            blockers |= between;
        }
    }
    return blockers;
}

bool ChessBoard::isLegalPinnedMove(Bitboard startLoc, Bitboard endLoc, Color turn)
{
    Bitboard pinned = startLoc & getPinnedPieces();

    if (!pinned) {
        return true;
    }

    return mAttack->inLine(Utils::getSquare(startLoc), Utils::getSquare(endLoc)) & getKing(turn);
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
    mCastle[to_int(Color::White)][to_int(Castling::Kingside)] = false;
    mCastle[to_int(Color::White)][to_int(Castling::Queenside)] = false;
    mCastle[to_int(Color::Black)][to_int(Castling::Kingside)] = false;
    mCastle[to_int(Color::Black)][to_int(Castling::Queenside)] = false;
    for (auto i: fenFields[2]) {
        switch (i) {
            case '-':
                break;
            case 'K':
                mCastle[to_int(Color::White)][to_int(Castling::Kingside)] = true;
                break;
            case 'Q':
                mCastle[to_int(Color::White)][to_int(Castling::Queenside)] = true;
                break;
            case 'k':
                mCastle[to_int(Color::Black)][to_int(Castling::Kingside)] = true;
                break;
            case 'q':
                mCastle[to_int(Color::Black)][to_int(Castling::Queenside)] = true;
                break;
        }
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