set(LIBRARY_FILES
    ChessBoard.cpp
    Move.cpp
    Attack.cpp
    BitboardUtils.cpp)

add_library(ChessEngine ${LIBRARY_FILES})
target_include_directories(ChessEngine PUBLIC ../include)
