#find_library(ChessEngine REQUIRED)
add_executable(my_tests maintest.cpp ChessBoardTest.cpp)
target_link_libraries(my_tests PRIVATE ChessEngine)

add_test(NAME chess-engine-tests COMMAND my_tests)
