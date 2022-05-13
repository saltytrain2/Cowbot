#include "ChessBoard.h"


ChessBoard::ChessBoard(std::string layout)
    : mPieceBB{}, mTurn(white), mWhiteCastle{}, mBlackCastle{}, mSquareBoard{}, mMoveList()
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
        mTurn = black;
    }

    // initializing mWhiteCastle and mBlackCastle
    for (auto i: fenFields[2]) {
        switch (i) {
            case '-':
                break;
            case 'K':
                mWhiteCastle[kingside] = true;
                break;
            case 'Q':
                mWhiteCastle[queenside] = true;
                break;
            case 'k':
                mBlackCastle[kingside] = true;
                break;
            case 'q':
                mBlackCastle[queenside] = true;
                break;
        }
    }
    updateSquareBoard();
}

Bitboard ChessBoard::getWhitePawns() const noexcept
{
    return mPieceBB[whitePawns];
}

Bitboard ChessBoard::getWhiteKnights() const noexcept
{
    return mPieceBB[whiteKnights];
}

Bitboard ChessBoard::getWhiteBishops() const noexcept
{
    return mPieceBB[whiteBishops];
}

Bitboard ChessBoard::getWhiteRooks() const noexcept
{
    return mPieceBB[whiteRooks];
}

Bitboard ChessBoard::getWhiteQueens() const noexcept
{
    return mPieceBB[whiteQueens];
}

Bitboard ChessBoard::getWhiteKing() const noexcept
{
    return mPieceBB[whiteKing];
}

Bitboard ChessBoard::getBlackPawns() const noexcept
{
    return mPieceBB[blackPawns];
}

Bitboard ChessBoard::getBlackKnights() const noexcept
{
    return mPieceBB[blackKnights];
}

Bitboard ChessBoard::getBlackBishops() const noexcept
{
    return mPieceBB[blackBishops];
}

Bitboard ChessBoard::getBlackRooks() const noexcept
{
    return mPieceBB[blackRooks];
}

Bitboard ChessBoard::getBlackQueens() const noexcept
{
    return mPieceBB[blackQueens];
}

Bitboard ChessBoard::getBlackKing() const noexcept
{
    return mPieceBB[blackKing];
}

Bitboard ChessBoard::getWhitePieces() const noexcept
{
    return mPieceBB[whitePieces];
}

Bitboard ChessBoard::getBlackPieces() const noexcept
{
    return mPieceBB[blackPieces];
}

Bitboard ChessBoard::getAllPieces() const noexcept
{
    return mPieceBB[allPieces];
}

Bitboard ChessBoard::getEmptySquares() const noexcept
{
    return mPieceBB[emptySquares];
}

Color ChessBoard::getTurn() const noexcept
{
    return mTurn;
}

bool ChessBoard::getWhiteCastleRights(Castling side) const noexcept
{
    return mWhiteCastle[side];
}

bool ChessBoard::getBlackCastleRights(Castling side) const noexcept
{
    return mBlackCastle[side];
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
                    mPieceBB[whitePawns] |= uint64_t(1) << shiftIndex;
                } else if (j == 'N') {
                    mPieceBB[whiteKnights] |= uint64_t(1) << shiftIndex;
                } else if (j == 'B') {
                    mPieceBB[whiteBishops] |= uint64_t(1) << shiftIndex;
                } else if (j == 'R') {
                    mPieceBB[whiteRooks] |= uint64_t(1) << shiftIndex;
                } else if (j == 'Q') {
                    mPieceBB[whiteQueens] |= uint64_t(1) << shiftIndex;
                } else if (j == 'K') {
                    mPieceBB[whiteKing] |= uint64_t(1) << shiftIndex;
                } else if (j == 'p') {
                    mPieceBB[blackPawns] |= uint64_t(1) << shiftIndex;
                } else if (j == 'n') {
                    mPieceBB[blackKnights] |= uint64_t(1) << shiftIndex;
                } else if (j == 'b') {
                    mPieceBB[blackBishops] |= uint64_t(1) << shiftIndex;
                } else if (j == 'r') {
                    mPieceBB[blackRooks] |= uint64_t(1) << shiftIndex;
                } else if (j == 'q') {
                    mPieceBB[blackQueens] |= uint64_t(1) << shiftIndex;
                } else if (j == 'k') {
                    mPieceBB[blackKing] |= uint64_t(1) << shiftIndex;
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
    mPieceBB[whitePieces] = mPieceBB[whitePawns] & mPieceBB[whiteKnights] & mPieceBB[whiteBishops] & mPieceBB[whiteRooks] & mPieceBB[whiteQueens] & mPieceBB[whiteKing];
    mPieceBB[blackPieces] = mPieceBB[blackPawns] & mPieceBB[blackKnights] & mPieceBB[blackBishops] & mPieceBB[blackRooks] & mPieceBB[blackQueens] & mPieceBB[blackKing];
    mPieceBB[allPieces] = mPieceBB[whitePieces] & mPieceBB[blackPieces];
    mPieceBB[emptySquares] = ~mPieceBB[allPieces];
}

int ChessBoard::makeMove(Move& nextMove) noexcept
{
    Bitmove move = nextMove.getMove();
    Square startingSquare = Square(move & 0x003F);
    Square endingSquare = Square((move & 0x0FC0) >> 6);

    // // TODO
    // // assume that the move given is valid for now

    PieceSets movedPiece = mSquareBoard[startingSquare];
    PieceSets capturedPiece = mSquareBoard[endingSquare];

    mPieceBB[movedPiece] |= Bitboard(1) << endingSquare;
    mPieceBB[movedPiece] &= ~(Bitboard(1) << startingSquare);

    if (capturedPiece != emptySquares) {
        nextMove.setCapturedPiece(capturedPiece);
        mPieceBB[capturedPiece] &= ~(Bitboard(1) << endingSquare);
    }

    updateRedundantBitboards();
    updateSquareBoard();

    mMoveList.push_back(nextMove);
    return 0;
}


void ChessBoard::updateSquareBoard() noexcept
{
    for (Square i = a1; i < null; ++i) {
        bool changed = false;
        for (PieceSets j = whitePawns; j < whitePieces; ++j) {
            if (mPieceBB[j] & uint64_t(1) << i) {
                mSquareBoard[i] = j;
                changed = true;
            }
        }
        if (!changed) {
            mSquareBoard[i] = emptySquares;
        }
    }
}

void ChessBoard::printSquareBoard() const noexcept
{
    std::cout << "-----------------" << std::endl;
    std::string row;
    for (Square i = null; i > a1; --i) {
        row += "|";
        if (mSquareBoard[i - 1] == whitePawns) {
            row += "P";
        } else if (mSquareBoard[i - 1] == whiteKnights) {
            row += "N";
        } else if (mSquareBoard[i - 1] == whiteBishops) {
            row += "B";
        } else if (mSquareBoard[i - 1] == whiteRooks) {
            row += "R";
        } else if (mSquareBoard[i - 1] == whiteQueens) {
            row += "Q";
        } else if (mSquareBoard[i - 1] == whiteKing) {
            row += "K";
        } else if (mSquareBoard[i - 1] == blackPawns) {
            row += "p";
        } else if (mSquareBoard[i - 1] == blackKnights) {
            row += "n";
        } else if (mSquareBoard[i - 1] == blackBishops) {
            row += "b";
        } else if (mSquareBoard[i - 1] == blackRooks) {
            row += "r";
        } else if (mSquareBoard[i - 1] == blackQueens) {
            row += "q";
        } else if (mSquareBoard[i - 1] == blackKing) {
            row += "k";
        } else {
            row += " ";
        }
        //std::cout << "|";

        // if last square in row
        if (i % 8 == 1) {
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

    PieceSets movedPiece = mSquareBoard[endingSquare];
    PieceSets capturedPiece = mMoveList[mMoveList.size() - 1].getCapturedPiece();
    mPieceBB[movedPiece] |= Bitboard(1) << startingSquare;
    mPieceBB[movedPiece] &= ~(Bitboard(1) << endingSquare);
    
    if (capturedPiece != emptySquares) {
        mPieceBB[capturedPiece] |= Bitboard(1) << endingSquare;
    }

    updateRedundantBitboards();
    updateSquareBoard();
    mMoveList.pop_back();
}

bool ChessBoard::isLegal(const Move& nextMove)
{
    (void)nextMove;
    return true;
}