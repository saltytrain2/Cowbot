#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>
#include <chrono>
#include <iostream>

#include "ChessBoard.h"
#include "Attack.h"
#include "MoveGen.h"
#include "Search.h"
#include "FakeEval.h"
#include "TT.h"
#include "MoveOrdering.h"

using namespace Cowbot;

TEST_CASE("Mate in 2") {
    std::ios::sync_with_stdio(false);

    auto attack = std::make_shared<Attack>();
    auto board = std::make_shared<ChessBoard>(attack.get());
    auto moveGen = std::make_shared<MoveGen>(board.get(), attack.get());
    auto fakeEval = std::make_shared<FakeEval>();
    auto tt = std::make_shared<TT>();
    auto moveOrdering = std::make_shared<MoveOrdering>(board.get(), fakeEval.get());
    auto search = std::make_shared<Search>(board.get(), moveGen.get(), fakeEval.get(), tt.get(), moveOrdering.get());
    tt->setSize(128);

    board->updateChessBoard("2bqkbn1/2pppp2/np2N3/r3P1p1/p2N2B1/5Q2/PPPPKPP1/RNB2r2 w KQkq - 0 1");
    auto start = std::chrono::system_clock::now();
    auto res = search->search(6).first;
    auto end = std::chrono::system_clock::now();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    CHECK_EQ(res.toString(), "f3f7");
    std::cout << "time: " << double(microseconds.count()) / 1000000 << std::endl;

    board->updateChessBoard("1Q6/3r1p1k/6pp/8/1pP1p3/1PbqB3/5PPP/5RK1 b - - 1 1");
    start = std::chrono::system_clock::now();
    res = search->search(6).first;
    end = std::chrono::system_clock::now();
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    CHECK_EQ(res.toString(), "d3f1");
    std::cout << "time: " << double(microseconds.count()) / 1000000 << std::endl;

    board->updateChessBoard("r3k2r/p1ppqpb1/Bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPB1PPP/R3K2R b KQkq - 0 1");
    start = std::chrono::system_clock::now();
    res = search->search(6).first;
    end = std::chrono::system_clock::now();
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    res.toString() == "b4c3" ? CHECK_UNARY(true) : CHECK_EQ(res.toString(), "h3g2");
    std::cout << "time: " << double(microseconds.count()) / 1000000 << std::endl;

    board->updateChessBoard("r1bqkb1r/pppp1ppp/5n2/3Pp3/2Pn4/2N1P3/PP3PPP/R1BQKBNR b KQkq - 0 5");
    start = std::chrono::system_clock::now();
    res = search->search(6).first;
    end = std::chrono::system_clock::now();
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    CHECK_EQ(res.toString(), "d4f5");
    std::cout << "time: " << double(microseconds.count()) / 1000000 << std::endl;
}