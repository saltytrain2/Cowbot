add_library(ChessEngine ChessBoard.cpp Move.cpp)
target_include_directories(ChessEngine PUBLIC ../include)

add_executable(driver main.cpp)
target_include_directories(driver PUBLIC ../include)
target_link_libraries(driver PRIVATE ChessEngine)