set(TEST_FILES
    maintest.cpp
    ChessBoardTest.cpp
    AttackTest.cpp)

add_executable(my_tests ${TEST_FILES})
target_link_libraries(my_tests PRIVATE ChessEngine)

add_test(NAME chess-engine-tests COMMAND my_tests)
