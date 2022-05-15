set(TEST_FILES
    maintest.cpp
    ChessBoardTest.cpp
    AttackTest.cpp)

add_executable(AttackTest maintest.cpp AttackTest.cpp)
list(APPEND tests AttackTest)

add_executable(ChessBoardTest maintest.cpp ChessBoardTest.cpp)
list(APPEND tests ChessBoardTest)

foreach(test ${tests})
    target_link_libraries(${test} PRIVATE ChessEngine)
    add_test(NAME ${test} COMMAND ${test})
endforeach()
