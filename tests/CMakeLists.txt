add_executable(AttackTest AttackTest.cpp)
list(APPEND tests AttackTest)

add_executable(ChessBoardTest ChessBoardTest.cpp)
list(APPEND tests ChessBoardTest)

add_executable(MoveGenTest MoveGenTest.cpp)
list(APPEND tests MoveGenTest)

add_executable(PerftTest PerftTest.cpp)
list(APPEND tests PerftTest)

foreach(test ${tests})
    target_link_libraries(${test} PRIVATE ChessEngine)
    target_compile_options(${test} PRIVATE ${MY_COMPILE_FLAGS})
    target_link_options(${test} PRIVATE ${MY_LINK_FLAGS})
    add_test(NAME ${test} COMMAND ${test})
endforeach()
