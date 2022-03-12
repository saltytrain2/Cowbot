#include "ChessBoard.h"


ChessBoard::ChessBoard(std::string layout)
    : mPieceBB{}, mTurn(white), mWhiteCastle{}, mBlackCastle{}
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
    setBitboards(pieceRank);

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

Bitboard ChessBoard::getAllEmptySquares() const noexcept
{
    return mPieceBB[allEmptySquares];
}

Bitboard ChessBoard::getTurn() const noexcept
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

void ChessBoard::setBitboards(const std::vector<std::string>& piecesByRank)
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
    mPieceBB[whitePieces] = mPieceBB[whitePawns] & mPieceBB[whiteKnights] & mPieceBB[whiteBishops] & mPieceBB[whiteRooks] & mPieceBB[whiteQueens] & mPieceBB[whiteKing];
    mPieceBB[blackPieces] = mPieceBB[blackPawns] & mPieceBB[blackKnights] & mPieceBB[blackBishops] & mPieceBB[blackRooks] & mPieceBB[blackQueens] & mPieceBB[blackKing];
    mPieceBB[allPieces] = mPieceBB[whitePieces] & mPieceBB[blackPieces];
    mPieceBB[allEmptySquares] = ~mPieceBB[allPieces];
}