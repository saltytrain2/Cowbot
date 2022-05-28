#include "ChessBoard.h"


ChessBoard::ChessBoard(Attack* ptr, std::string layout)
    : mAttack(ptr), mPieceBB{}, mTurn(Color::White), mCastle{}, mSquareBoard{}, mMoveList{}
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
    if (fenFields[1] == "b") {
        mTurn = Color::Black;
    }

    // initializing mWhiteCastle and mBlackCastle
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
    Bitmove move = nextMove.getMove();
    Square startingSquare = Square(move & 0x003F);
    Square endingSquare = Square((move & 0x0FC0) >> 6);

    // // TODO
    // // assume that the move given is valid for now

    PieceSets movedPiece = mSquareBoard[to_int(startingSquare)];
    PieceSets capturedPiece = mSquareBoard[to_int(endingSquare)];

    mPieceBB[to_int(movedPiece)] |= Utils::getBitboard(endingSquare);
    mPieceBB[to_int(movedPiece)] &= ~Utils::getBitboard(startingSquare);

    if (capturedPiece != PieceSets::EmptySquares) {
        nextMove.setCapturedPiece(capturedPiece);
        mPieceBB[to_int(capturedPiece)] &= ~Utils::getBitboard(endingSquare);
    }

    updateRedundantBitboards();
    updateSquareBoard();
    updateCastlingRights(movedPiece);
    updatePinnedPieces();

    mMoveList.push_back(nextMove);
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
    Bitmove move = mMoveList[mMoveList.size() - 1].getMove();
    Square startingSquare = Square(move & 0x3F);
    Square endingSquare = Square((move & 0xFC0) >> 6);

    PieceSets movedPiece = mSquareBoard[to_int(endingSquare)];
    PieceSets capturedPiece = mMoveList[mMoveList.size() - 1].getCapturedPiece();
    mPieceBB[to_int(movedPiece)] |= Utils::getBitboard(startingSquare);
    mPieceBB[to_int(movedPiece)] &= ~Utils::getBitboard(endingSquare);
    
    if (capturedPiece != PieceSets::EmptySquares) {
        mPieceBB[to_int(capturedPiece)] |= Utils::getBitboard(endingSquare);
    }

    updateRedundantBitboards();
    updateSquareBoard();
    mMoveList.pop_back();

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

void ChessBoard::updatePinnedPieces() noexcept
{}

bool ChessBoard::getCastleRights(Castling side, Color color) const noexcept
{
    return mCastle[to_int(color)][to_int(side)];
}

bool ChessBoard::isLegal(const Move& move, Color turn)
{
    MoveType type = MoveType(move.getMove() >> 14);
    Square startSquare = Square(move.getMove() & 0x3F);
    Square endSquare = Square((move.getMove() & 0xFC0) >> 6);
    Bitboard startLoc = Utils::getBitboard(startSquare);
    Bitboard endLoc = Utils::getBitboard(endSquare);
    Bitboard blockers = getAllPieces();
    Bitboard kingLoc = getKing(turn);
    Bitboard enemyPieces = getPieces(!turn);

    if (type == MoveType::Castle) {
        if (endLoc > startLoc) {
            return !(isSquareUnderAttack(startSquare, !turn, blockers) && 
                   isSquareUnderAttack(Utils::eastOne(startSquare), !turn, blockers) &&
                   isSquareUnderAttack(endSquare, !turn, blockers));
        } else {
            return !(isSquareUnderAttack(startSquare, !turn, blockers) && 
                   isSquareUnderAttack(Utils::westOne(startSquare), !turn, blockers) &&
                   isSquareUnderAttack(endSquare, !turn, blockers));
        }
    } else if (type == MoveType::Enpassant) {
        // make the move
        if (turn == Color::White) {
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
        bool legal = squareAttackers(Utils::getSquare(kingLoc), !turn, blockers) & enemyPieces;
        
        // undo the move
        if (turn == Color::White) {
            mPieceBB[to_int(PieceSets::BlackPawns)] |= Utils::southOne(endLoc);
            mPieceBB[to_int(PieceSets::WhitePawns)] &= ~endLoc;
            mPieceBB[to_int(PieceSets::WhitePawns)] |= startLoc;
        } else {
            mPieceBB[to_int(PieceSets::BlackPawns)] |= Utils::southOne(endLoc);
            mPieceBB[to_int(PieceSets::WhitePawns)] &= ~endLoc;
            mPieceBB[to_int(PieceSets::WhitePawns)] |= startLoc;
        }

        return legal;
    }

    // All other move types
    return true;
}

bool ChessBoard::isSquareUnderAttack(Square sq, Color color, Bitboard blockers)
{
    return squareAttackers(sq, color, blockers) & getPieces(color);
}

Bitboard ChessBoard::squareAttackers(Square sq, Color color, Bitboard blockers)
{
    Bitboard pawns = getPawns(color);
    Bitboard knights = getKnights(color);
    Bitboard bishopQueens = getBishops(color) | getQueens(color);
    Bitboard rookQueens = getRooks(color) | getQueens(color);
    Bitboard king = getKing(color);

    return (mAttack->getPawnAttacks(sq, color) & pawns) |
           (mAttack->getKnightAttacks(sq) & knights) |
           (mAttack->getBishopAttacks(sq, blockers) & bishopQueens) |
           (mAttack->getRookAttacks(sq, blockers) & rookQueens) |
           (mAttack->getKingAttacks(sq) & king);
}