set(OBJECT_FILES
    ChessBoard.cpp
    Move.cpp
    Attack.cpp
    Utils.cpp
    MoveGen.cpp
    PNRG.cpp
    Search.cpp
    FakeEval.cpp
    TT.cpp
    MoveOrdering.cpp
)

add_library(ChessEngine STATIC ${OBJECT_FILES})
target_include_directories(ChessEngine PUBLIC ../include)
target_compile_options(ChessEngine PRIVATE ${MY_COMPILE_FLAGS})
target_link_options(ChessEngine PRIVATE ${MY_LINK_FLAGS})
