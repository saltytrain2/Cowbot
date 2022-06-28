#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdint>
#include <memory>
#include <vector>
#include <chrono>

#include "ChessBoard.h"
#include "Attack.h"
#include "MoveGen.h"

uint64_t helper(MoveGen* moveGen, ChessBoard* board, int depth)
{
    if (depth == 0) {
        return 1;
    }

    uint64_t res = 0;
    auto moveList = moveGen->generateLegalMoves();

    for (auto i : moveList) {
        board->makeMove(i);
        res += helper(moveGen, board, depth - 1);
        board->undoMove();
    }

    return res;
}

uint64_t perft(MoveGen* moveGen, ChessBoard* board, int depth)
{
    if (depth == 0) {
        return 1;
    }

    uint64_t res = 0;
    uint64_t prev = 0;
    auto moveList = moveGen->generateLegalMoves();

    for (auto i : moveList) {
        prev = res;
        board->makeMove(i);
        res += helper(moveGen, board, depth - 1);
        board->undoMove();
        std::cout << i.toString() << " " << res - prev << std::endl;
    }

    std::cout << "Total nodes: " << res << std::endl;
    return res;
}

TEST_CASE("Perft") {
    auto attack = std::make_shared<Attack>();
    attack->initTables();
    auto board = std::make_shared<ChessBoard>(attack.get());
    auto moveGen = std::make_shared<MoveGen>(board.get(), attack.get());

    // SUBCASE("Debug") {
    //     board->updateChessBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    //     board->printSquareBoard();
    //     CHECK(board->isLegal(Move(Square::B5, Square::B6, MoveType::Quiet)) == false);
    //     // Move move(Square::A2, Square::A3, MoveType::Quiet);
    //     // board->makeMove(move);
    //     // move.updateMove("b4b3");
    //     // board->makeMove(move);
    //     // move.updateMove("g6h8");
    //     // board->makeMove(move);
    //     perft(moveGen.get(), board.get(), 1);
    //     CHECK(board->getWhitePawns() == 0x200005000);
    //     CHECK(board->getWhiteRooks() == 0x2000000);
    //     CHECK(board->getWhiteKing() == 0x100000000);
    //     CHECK(board->getBlackPawns() == 0x4080020000000);
    //     CHECK(board->getBlackRooks() == 0x8000000000);
    //     CHECK(board->getBlackKing() == 0x80000000);
    //     perft(moveGen.get(), board.get(), 2);
    // }

    SUBCASE("First Game") {
        board->updateChessBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        std::vector<uint64_t> ans = {1, 20, 400, 8902, 197281, 4865609, 119060324};
        std::cout << "Position 1" << std::endl;
        for (uint32_t i = 0; i < ans.size(); ++i) {
            auto start = std::chrono::system_clock::now();
            auto res = perft(moveGen.get(), board.get(), i);
            auto end = std::chrono::system_clock::now();
            auto milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            CHECK(ans[i] == res);
            std::cout << "Perft " << i << " in " << double(milliseconds.count()) / 1000000 << " s" << std::endl;
        }
        std::cout << std::endl;
    }

    SUBCASE("Second Game") {
        board->updateChessBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        board->printSquareBoard();
        std::vector<uint64_t> ans = {1, 48, 2039, 97862, 4085603, 193690690};
        std::cout << "Position 2" << std::endl;
        for (uint32_t i = 0; i < ans.size(); ++i) {
            auto start = std::chrono::system_clock::now();
            auto res = perft(moveGen.get(), board.get(), i);
            auto end = std::chrono::system_clock::now();
            auto milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            CHECK(ans[i] == res);
            std::cout << "Perft " << i << " in " << double(milliseconds.count()) / 1000000 << " s" << std::endl;
        }
        std::cout << std::endl;
    }

    SUBCASE("Third Game") {
        board->updateChessBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        board->printSquareBoard();
        std::vector<uint64_t> ans = {1, 14, 191, 2812, 43238, 674624, 11030083};
        std::cout << "Position 3" << std::endl;
        for (uint32_t i = 0; i < ans.size(); ++i) {
            auto start = std::chrono::system_clock::now();
            auto res = perft(moveGen.get(), board.get(), i);
            auto end = std::chrono::system_clock::now();
            auto milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            CHECK(ans[i] == res);
            std::cout << "Perft " << i << " in " << double(milliseconds.count()) / 1000000 << " s" << std::endl;
        }
        std::cout << std::endl;
    }

    SUBCASE("Fourth Game") {
        board->updateChessBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
        board->printSquareBoard();
        std::vector<uint64_t> ans = {1, 6, 264, 9467, 422333, 15833292, 706045033};
        std::cout << "Position 4" << std::endl;
        for (uint32_t i = 0; i < ans.size() - 1; ++i) {
            auto start = std::chrono::system_clock::now();
            auto res = perft(moveGen.get(), board.get(), i);
            auto end = std::chrono::system_clock::now();
            auto milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            CHECK(ans[i] == res);
            std::cout << "Perft " << i << " in " << double(milliseconds.count()) / 1000000 << " s" << std::endl;
        }
        std::cout << std::endl;
    }

    SUBCASE("Fifth Game") {
        board->updateChessBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ");
        board->printSquareBoard();
        std::vector<uint64_t> ans = {1, 44, 1486, 62379, 2103487, 89941194};
        std::cout << "Position 5" << std::endl;
        for (uint32_t i = 0; i < ans.size(); ++i) {
            auto start = std::chrono::system_clock::now();
            auto res = perft(moveGen.get(), board.get(), i);
            auto end = std::chrono::system_clock::now();
            auto milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            CHECK(ans[i] == res);
            std::cout << "Perft " << i << " in " << double(milliseconds.count()) / 1000000 << " s" << std::endl;
        }
        std::cout << std::endl;
    }

    SUBCASE("Sixth Game") {
        board->updateChessBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ");
        board->printSquareBoard();
        std::vector<uint64_t> ans = {1, 46, 2079, 89890, 3894594, 164075551};
        std::cout << "Position 6" << std::endl;
        for (uint32_t i = 0; i < ans.size(); ++i) {
            auto start = std::chrono::system_clock::now();
            auto res = perft(moveGen.get(), board.get(), i);
            auto end = std::chrono::system_clock::now();
            auto milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            CHECK(ans[i] == res);
            std::cout << "Perft " << i << " in " << double(milliseconds.count()) / 1000000 << " s" << std::endl;
        }
        std::cout << std::endl;
    }
}