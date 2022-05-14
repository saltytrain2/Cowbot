set(LIBRARY_FILES
    ChessBoard.cpp
    Move.cpp
    Attack.cpp
    BitboardUtils.cpp)

add_library(ChessEngine ${LIBRARY_FILES})
target_include_directories(ChessEngine PUBLIC ../include)

add_executable(driver main.cpp)
target_link_libraries(driver PRIVATE ChessEngine)