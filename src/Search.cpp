#include "Search.h"

#include <map>
#include <algorithm>

#include "Move.h"
#include "ChessBoard.h"
#include "MoveOrdering.h"

using namespace Cowbot;

Search::Search(ChessBoard* board, MoveGen* moveGen, FakeEval* fakeEval, TT* tt, MoveOrdering* moveOrdering)
    : mBoard(board), mMoveGen(moveGen), mFakeEval(fakeEval), mTT(tt), mMoveOrdering(moveOrdering)
{}

std::pair<Move, int16_t> Search::search(uint8_t depth)
{
    auto legalMoves = mMoveGen->generateLegalMoves(mBoard->getTurn());
    mMoveOrdering->orderMoves(legalMoves.begin(), legalMoves.end());
    
    std::vector<std::pair<Move, int16_t>> rootMoves;
    std::transform(legalMoves.begin(), legalMoves.end(), std::back_inserter(rootMoves), [](auto x) { return std::make_pair(x, 0); });
    std::pair<Move, int16_t> bestMove;
    bestMove = rootMoves[0];

    for (int i = 1; i <= depth; ++i) {
        for (auto& rootPair: rootMoves) {
            mBoard->makeMove(rootPair.first);
            rootPair.second = -alphaBeta(INT16_MIN + 1, INT16_MAX, i - 1);
            if (rootPair.second > bestMove.second) {
                bestMove = rootPair;
            }
            mBoard->undoMove();
        }
        std::sort(rootMoves.begin(), rootMoves.end(), [](const auto& x, const auto& y) { return x.second > y.second; });
    }
    mMoveOrdering->clearHistory();
    return bestMove;
}

int16_t Search::alphaBeta(int16_t alpha, int16_t beta, uint8_t depthleft)
{
    if (depthleft == 0) {
        return qsearch(alpha, beta);
    }
    
    // attempt to search for this position in the transposition table
    int16_t origAlpha = alpha;
    ZobristHash posHash = mBoard->getHash();
    TTEntry entry = mTT->getEntry(posHash);
    Move bestMove;
    if (entry.key == posHash && mBoard->isPseudoLegal(entry.bestMove) && entry.depth >= depthleft) {
        if (entry.node == NodeType::EXACT) {
            return entry.score;
        } else if (entry.node == NodeType::UPPERBOUND) {
            beta = std::min(beta, entry.score);
        } else {
            alpha = std::max(alpha, entry.score);
        }

        if (alpha >= beta) {
            return entry.score;
        }
        bestMove = entry.bestMove;
    }

    auto legalMoves = mMoveGen->generateLegalMoves(mBoard->getTurn());
    if (legalMoves.empty()) {
        return mBoard->isKingUnderAttack() ? -INT16_MAX : 0;
    }

    if (bestMove) {
        for (auto& move: legalMoves) {
            if (move == bestMove) {
                std::swap(legalMoves[0], move);
                break;
            }
        }
        mMoveOrdering->orderMoves(legalMoves.begin() + 1, legalMoves.end());
    } else {
        mMoveOrdering->orderMoves(legalMoves.begin(), legalMoves.end());
    }

    for (auto move: legalMoves) {
        mBoard->makeMove(move);
        int16_t score = -alphaBeta(-beta, -alpha, depthleft - 1);
        mBoard->undoMove();
        if (score >= beta) {
            mTT->setEntry(posHash, TTEntry(posHash, move, score, depthleft, NodeType::LOWERBOUND));
            if (!mBoard->isCapture(move)) {
                mMoveOrdering->addHistory(move, depthleft);
            }
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            bestMove = move;
        }
    }

    if (!bestMove) {
        bestMove = legalMoves[0];
    }
    NodeType node = alpha <= origAlpha ? NodeType::UPPERBOUND : NodeType::EXACT;
    mTT->setEntry(posHash, TTEntry(posHash, bestMove, alpha, depthleft, node));
    return alpha;
}

int16_t Search::qsearch(int16_t alpha, int16_t beta)
{
    int16_t currentEval = mBoard->getTurn() == Color::White ? mFakeEval->eval(*mBoard) : -mFakeEval->eval(*mBoard);

    if (currentEval >= beta) {
        return beta;
    }

    int16_t maxMaterialSwing = 1025;
    if (mBoard->containsPromotingPawns()) {
        maxMaterialSwing += 1025 - 82;
    }

    // hopeless situation, simply return alpha
    if (currentEval < alpha - maxMaterialSwing) {
        return alpha;
    }

    if (alpha < currentEval) {
        alpha = currentEval;
    }

    auto prunedCaptures = mMoveGen->generateLegalCaptures(mBoard->getTurn());
    mMoveOrdering->orderMovesQuiescent(prunedCaptures.begin(), prunedCaptures.end());

    for (auto capture : prunedCaptures) {
        mBoard->makeMove(capture);
        int16_t score = -qsearch(-beta, -alpha);
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