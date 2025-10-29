cmake_minimum_required(VERSION 3.10)
project(wh40k_nfc)

set(CMAKE_CXX_STANDARD 17)

add_executable(wh40k_nfc
    src/main.cpp
    src/scardHandling.cpp
)    
target_link_libraries(wh40k_nfc PRIVATE winscard)
