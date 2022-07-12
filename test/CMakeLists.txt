
message("Creating Test Executable")
add_executable(EasyCLITest test.c)
target_link_libraries(EasyCLITest ${PROJECT_BINARY_DIR}/libEasyCLIStatic.a)