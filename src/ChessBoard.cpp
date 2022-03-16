#include "ChessBoard.h"


ChessBoard::ChessBoard(std::string layout)
    : mPieceBB{}, mTurn(white), mWhiteCastle{}, mBlackCastle{}, m88Board{}, mMoveList()
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
    update88Board();
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

int ChessBoard::makeMove(const Move& nextMove) noexcept
{
    Bitmove move = nextMove.getMove();
    uint16_t startingSquare = move & 0x3F;
    uint16_t endingSquare = (move & 0xFC0) >> 6;
    std::cout << startingSquare << endingSquare << " ";

    // // TODO
    // // assume that the move given is valid for now

    PieceSets movedPiece = m88Board[startingSquare];
    PieceSets capturedPiece = m88Board[endingSquare];
    std::cout << movedPiece << capturedPiece;
    mPieceBB[movedPiece] |= uint64_t(1) << endingSquare;
    mPieceBB[movedPiece] &= ~(uint64_t(1) << startingSquare);

    if (capturedPiece != emptySquares) {
        mPieceBB[capturedPiece] &= ~(uint64_t(1) << endingSquare);
    }

    updateRedundantBitboards();
    update88Board();

    mMoveList.push_back(move);
    return 0;
}

void ChessBoard::update88Board() noexcept
{
    for (Square i = a1; i < null; ++i) {
        for (PieceSets j = whitePawns; j < whitePieces; ++j) {
            if (mPieceBB[j] & Bitboard(1) << i) {
                m88Board[i] = j;
                break;
            }
            m88Board[i] = emptySquares;
        }
    }
}

void ChessBoard::print88Board() const noexcept
{
    for (Square i = null; i > a1; --i) {
        if (m88Board[i - 1] == whitePawns) {
            std::cout << "P";
        } else if (m88Board[i - 1] == whiteKnights) {
            std::cout << "N";
        } else if (m88Board[i - 1] == whiteBishops) {
            std::cout << "B";
        } else if (m88Board[i - 1] == whiteRooks) {
            std::cout << "R";
        } else if (m88Board[i - 1] == whiteQueens) {
            std::cout << "Q";
        } else if (m88Board[i - 1] == whiteKing) {
            std::cout << "K";
        } else if (m88Board[i - 1] == blackPawns) {
            std::cout << "p";
        } else if (m88Board[i - 1] == blackKnights) {
            std::cout << "n";
        } else if (m88Board[i - 1] == blackBishops) {
            std::cout << "b";
        } else if (m88Board[i - 1] == blackRooks) {
            std::cout << "r";
        } else if (m88Board[i - 1] == blackQueens) {
            std::cout << "q";
        } else if (m88Board[i - 1] == blackKing) {
            std::cout << "k";
        } else {
            std::cout << " ";
        }

        // if last square in row
        if (i % 8 == 1) {
            std::cout << std::endl;
        } 
    }
}