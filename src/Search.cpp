#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "Search.h"
#include "Move.h"

Search::Search(ChessBoard* board, MoveGen* moveGen, FakeEval* fakeEval, TT* tt)
    : mBoard(board), mMoveGen(moveGen), mFakeEval(fakeEval), mTT(tt)
{}

int16_t Search::alphaBetaWhite(int16_t alpha, int16_t beta, uint8_t depthleft, uint8_t depth)
{
    if (depthleft == 0) {
        return mFakeEval->eval(*mBoard);
    }


    // // attempt to search for this position in the transposition table
    // int16_t origAlpha = alpha;
    // ZobristHash posHash = mBoard->getHash();
    // TTEntry entry = mTT->getEntry(posHash);
    // if (entry && entry.depth >= depth) {
    //     if (entry.node == NodeType::PV_NODE) {
    //         return entry.score;
    //     } else if (entry.node == NodeType::ALL_NODE) {
    //         beta = std::min(beta, entry.score);
    //     } else {
    //         alpha = std::max(alpha, entry.score);
    //     }

    //     if (alpha >= beta) {
    //         return entry.score;
    //     }
    // }

    auto legalMoves = mMoveGen->generateLegalMoves();
    if (legalMoves.size() == 0) {
        return mBoard->isKingUnderAttack() ? INT16_MIN + depth : 0; 
    }

    // Move bestMove;
    for (auto move: legalMoves) {
        mBoard->makeMove(move);
        int16_t score = alphaBetaBlack(alpha, beta, depthleft - 1, depth + 1);
        mBoard->undoMove();
        if (score >= beta) {
            // mTT->setEntry(posHash, TTEntry(posHash, move, score, depth, NodeType::CUT_NODE));
            return beta;
        }
        if (score > alpha) {
            // bestMove = move;
            alpha = score;
        }
    }
    // if (!bestMove) {
    //     bestMove = legalMoves[0];
    // }
    // NodeType node = alpha <= origAlpha ? NodeType::ALL_NODE : NodeType::PV_NODE;
    // mTT->setEntry(posHash, TTEntry(posHash, bestMove, alpha, depth, node));
    return alpha;
}

int16_t Search::alphaBetaBlack(int16_t alpha, int16_t beta, uint8_t depthleft, uint8_t depth)
{
    if (!depthleft) {
        return mFakeEval->eval(*mBoard);
    }

    // attempt to search for this position in the transposition table


    auto legalMoves = mMoveGen->generateLegalMoves();
    if (legalMoves.size() == 0) {
        return mBoard->isKingUnderAttack() ? INT16_MAX - depth : 0; 
    }

    for (auto move: legalMoves) {
        mBoard->makeMove(move);
        int16_t score = alphaBetaWhite(alpha, beta, depthleft - 1, depth + 1);
        mBoard->undoMove();
        if (score <= alpha) {
            return alpha;
        }
        if (score < beta) {
            beta = score;
        }
    }
    return beta;
}

std::pair<std::string, int16_t> Search::search(uint8_t depth)
{
    auto rootMoves = mMoveGen->generateLegalMoves();
    std::unordered_map<std::string, int16_t> rootMoveScores;

    for (int i = 1; i <= depth; ++i) {
        for (auto j: rootMoves) {
            mBoard->makeMove(j);
            // int16_t score = toMove == Color::White ? alphaBetaBlack(INT16_MIN, INT16_MAX, depth-1, 1) : alphaBetaWhite(INT16_MIN, INT16_MAX, depth-1, 1);
            int16_t score = -alphaBeta(INT16_MIN + 1, INT16_MAX, i - 1, 1);
            rootMoveScores[j.toString()] = score;
            mBoard->undoMove();
        }
    }
    // for (auto i: rootMoves) {
    //     mBoard->makeMove(i);
    //     // int16_t score = toMove == Color::White ? alphaBetaBlack(INT16_MIN, INT16_MAX, depth-1, 1) : alphaBetaWhite(INT16_MIN, INT16_MAX, depth-1, 1);
    //     int16_t score = -alphaBeta(INT16_MIN + 1, INT16_MAX, depth - 1, 1);
    //     rootMoveScores[i.toString()] = score;
    //     mBoard->undoMove();
    // }

    // for (auto i: rootMoveScores) {
    //     std::cout << i.first << ' ' << i.second << std::endl;
    // }
    auto compare = [](const std::pair<std::string, int16_t>& x, const std::pair<std::string, int16_t>& y) { return x.second < y.second; };
    return *std::max_element(rootMoveScores.begin(), rootMoveScores.end(), compare);
}

int16_t Search::alphaBeta(int16_t alpha, int16_t beta, uint8_t depthleft, uint8_t depth)
{
    if (depthleft == 0) {
        return qsearch(alpha, beta);
    }

    // attempt to search for this position in the transposition table
    int16_t origAlpha = alpha;
    ZobristHash posHash = mBoard->getHash();
    TTEntry entry = mTT->getEntry(posHash);
    if (mTT->keyExists(posHash) && entry.depth >= depthleft) {
        if (entry.node == NodeType::PV_NODE) {
            return entry.score;
        } else if (entry.node == NodeType::ALL_NODE) {
            beta = std::min(beta, entry.score);
        } else {
            alpha = std::max(alpha, entry.score);
        }

        if (alpha >= beta) {
            return entry.score;
        }
    }

    auto legalMoves = mMoveGen->generateLegalMoves();
    if (legalMoves.empty()) {
        return mBoard->isKingUnderAttack() ? INT16_MIN + depth : 0;
    }

    Move bestMove;
    for (auto move: legalMoves) {
        mBoard->makeMove(move);
        int16_t score = -alphaBeta(-beta, -alpha, depthleft - 1, depth + 1);
        mBoard->undoMove();
        if (score >= beta) {
            mTT->setEntry(posHash, TTEntry(posHash, move, score, depthleft, NodeType::CUT_NODE));
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }

    if (!bestMove) {
        bestMove = legalMoves[0];
    }
    NodeType node = alpha <= origAlpha ? NodeType::ALL_NODE : NodeType::PV_NODE;
    mTT->setEntry(posHash, TTEntry(posHash, bestMove, alpha, depthleft, node));
    return alpha;
}

int16_t Search::qsearch(int16_t alpha, int16_t beta)
{
    int16_t currentEval = mFakeEval->eval(*mBoard);
    if (currentEval >= beta) {
        return beta;
    }
    if (alpha < currentEval) {
        alpha = currentEval;
    }

    auto legalMoves = mMoveGen->generateLegalMoves();
    Color toMove = mBoard->getTurn();
    for (auto i : legalMoves) {
        // check only captures
        Bitboard enemyPieces = mBoard->getPieces(!toMove);
        if (!(Utils::getBitboard(i.getEndingSquare()) & enemyPieces)) {
            continue;
        }
        mBoard->makeMove(i);
        int16_t score = -qsearch(alpha, beta);
        mBoard->undoMove();

        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    return alpha;
}