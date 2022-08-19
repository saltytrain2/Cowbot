find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

add_executable(CowbotGUI GUIDriver.cpp)
target_link_libraries(CowbotGUI PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries (CowbotGUI PRIVATE ChessEngine)
target_compile_options(CowbotGUI PRIVATE ${MY_COMPILE_FLAGS})
target_link_options(CowbotGUI PRIVATE ${MY_LINK_FLAGS})
