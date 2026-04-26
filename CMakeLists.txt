cmake_minimum_required(VERSION 3.16)
project(wh40k_nfc)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

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
    src/mainWindow.hpp
    src/mainWindow.cpp
    src/readerConnectionWindow.hpp
    src/readerConnectionWindow.cpp
    src/cardWaitThread.hpp
    src/cardWaitThread.cpp
    src/modelInfoWindow.hpp
    src/modelInfoWindow.cpp
    # troopInfoFunctions_class_version.cpp
)

target_link_libraries(wh40k_nfc PRIVATE
    winscard
    Qt6::Widgets
)