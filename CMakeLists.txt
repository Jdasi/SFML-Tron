cmake_minimum_required(VERSION 3.2.0)
project(TronGame)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH}")


###############################################################################

# Assume SFML is here for now...
set(SFML_ROOT ${CMAKE_SOURCE_DIR}/../SFML/build/install)
find_package(SFML 2 REQUIRED system window graphics network audio)

###############################################################################

add_library(TronGame STATIC
    Code/Game/Bike.cpp
    Code/Game/Bike.h
    Code/Game/Constants.h
    Code/Game/Noncopyable.h
    Code/Game/PacketID.h
    Code/Game/State.h
    Code/Game/StateHandler.h
    Code/Game/TronGame.cpp
    Code/Game/TronGame.h
)

target_include_directories(TronGame
    PUBLIC Code
    PRIVATE ${SFML_INCLUDE_DIR}
)

target_link_libraries(TronGame
    ${SFML_LIBRARIES}
)

###############################################################################

add_executable(TronClient
    Code/Client/ClientData.h
    Code/Client/ClientState.h
    Code/Client/ClientStateGame.cpp
    Code/Client/ClientStateGame.h
    Code/Client/ClientStates.h
    Code/Client/ClientStateStart.cpp
    Code/Client/ClientStateStart.h
    Code/Client/GameAction.h
    Code/Client/InputHandler.cpp
    Code/Client/InputHandler.h
    Code/Client/main_client.cpp
    Code/Client/NetworkClient.h
    Code/Client/NetworkManager.cpp
    Code/Client/NetworkManager.h
    Code/Client/ObjectRenderer.cpp
    Code/Client/ObjectRenderer.h
    Code/Client/Scheduler.cpp
    Code/Client/Scheduler.h
    Code/Client/SimpleTimer.cpp
    Code/Client/SimpleTimer.h
    Code/Client/ThreadDispatcher.h
    Code/Client/TronClient.cpp
    Code/Client/TronClient.h
    Code/Client/TronNetworkManager.cpp
    Code/Client/TronNetworkManager.h
)

target_include_directories(TronClient
    PUBLIC Code
    PRIVATE ${SFML_INCLUDE_DIR}
)

target_link_libraries(TronClient
    TronGame
    ${SFML_LIBRARIES}
)

###############################################################################

add_executable(TronServer
    Code/Server/main_server.cpp
    Code/Server/TronServer.cpp
    Code/Server/TronServer.h
    Code/Server/User.cpp
    Code/Server/User.h
)

target_include_directories(TronServer
    PUBLIC Code
    PRIVATE ${SFML_INCLUDE_DIR}
)

target_link_libraries(TronServer
    TronGame
    ${SFML_LIBRARIES}
)

