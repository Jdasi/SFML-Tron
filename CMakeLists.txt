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
    Code/Game/BikeState.h
    Code/Game/CellValue.h
    Code/Game/Constants.h
    Code/Game/GameStateIDs.h
    Code/Game/Grid.cpp
    Code/Game/Grid.h
    Code/Game/INetworkSimulation.h
    Code/Game/Jhelper.cpp
    Code/Game/Jhelper.h
    Code/Game/ListenerSubject.h
    Code/Game/MoveDirection.h
    Code/Game/Noncopyable.h
    Code/Game/PacketID.h
    Code/Game/PlayerState.h
    Code/Game/RandomEngine.cpp
    Code/Game/RandomEngine.h
    Code/Game/Scheduler.cpp
    Code/Game/Scheduler.h
    Code/Game/SimpleTimer.cpp
    Code/Game/SimpleTimer.h
    Code/Game/Simulation.cpp
    Code/Game/Simulation.h
    Code/Game/SimulationListener.h
    Code/Game/SimulationState.h
    Code/Game/State.h
    Code/Game/StateHandler.h
    Code/Game/ThreadDispatcher.h
    Code/Game/Vector2i.h
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
    Code/Client/AssetManager.cpp
    Code/Client/AssetManager.h
    Code/Client/ClientData.h
    Code/Client/ClientState.h
    Code/Client/ClientStateEnd.cpp
    Code/Client/ClientStateEnd.h
    Code/Client/ClientStateGame.cpp
    Code/Client/ClientStateGame.h
    Code/Client/ClientStateHandler.cpp
    Code/Client/ClientStateHandler.h
    Code/Client/ClientStateLobby.cpp
    Code/Client/ClientStateLobby.h
    Code/Client/GameAction.h
    Code/Client/GameManager.cpp
    Code/Client/GameManager.h
    Code/Client/INetworkClient.h
    Code/Client/InputHandler.cpp
    Code/Client/InputHandler.h
    Code/Client/LobbySlot.cpp
    Code/Client/LobbySlot.h
    Code/Client/LobbyUI.cpp
    Code/Client/LobbyUI.h
    Code/Client/main_client.cpp
    Code/Client/NetworkManager.cpp
    Code/Client/NetworkManager.h
    Code/Client/Player.cpp
    Code/Client/Player.h
    Code/Client/PlayerMarker.cpp
    Code/Client/PlayerMarker.h
    Code/Client/PrettyGrid.cpp
    Code/Client/PrettyGrid.h
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
    Code/Server/Client.cpp
    Code/Server/Client.h
    Code/Server/ISimulationServer.h
    Code/Server/SimulationThread.cpp
    Code/Server/SimulationThread.h
    Code/Server/TronServer.cpp
    Code/Server/TronServer.h
)

target_include_directories(TronServer
    PUBLIC Code
    PRIVATE ${SFML_INCLUDE_DIR}
)

target_link_libraries(TronServer
    TronGame
    ${SFML_LIBRARIES}
)

