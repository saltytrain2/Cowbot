add_executable(AttackTest maintest.cpp AttackTest.cpp)
list(APPEND tests AttackTest)

add_executable(ChessBoardTest maintest.cpp ChessBoardTest.cpp)
list(APPEND tests ChessBoardTest)

add_executable(MoveGenTest maintest.cpp MoveGenTest.cpp)
list(APPEND tests MoveGenTest)

add_executable(PerftTest maintest.cpp PerftTest.cpp)
list(APPEND tests PerftTest)

foreach(test ${tests})
    target_link_libraries(${test} PRIVATE ChessEngine)
    add_test(NAME ${test} COMMAND ${test})
endforeach()
