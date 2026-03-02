cmake_minimum_required(VERSION 3.10)
project(wh40k_nfc)

set(CMAKE_CXX_STANDARD 17)

add_executable(wh40k_nfc
    src/main.cpp
    src/main.hpp
    src/scardHandling.cpp
    src/scardHandling.hpp
    src/scardReading.cpp
    src/scardReading.hpp
    src/scardWriting.cpp
    src/scardWriting.hpp
    src/troopInfoFunctions.cpp
    # troopInfoFunctions_class_version.cpp

    )    
target_link_libraries(wh40k_nfc PRIVATE winscard)