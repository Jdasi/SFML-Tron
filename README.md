LLP Exercise 2
--> Code: Headers & Source
--> Documentation: Supporting Docs
--> Libs: SFML
--> Projects: Visual Studio Projects
--> Props: Shared Properties
--> Resources: Assets

==================================
Developer Notes:
==================================

- The game supports a maximum of 4 players.
- The game requires a minimum of 2 players to start (though all connected players must be ready).
- The game has Xbox controller and Keyboard support (See documentation for controls).
- The game has audio, enjoy it!
- Take note of the colour the server assigns you in the lobby as it is the colour of your bike.
- Your boost charges replenish over time. Be aggressive!
- After a round, all players must go back to the lobby before a new one can be started.
- Players who connect while a round is in progress must wait until it is over to play.

==================================
Server Settings:
==================================

The game uses a server_settings.txt file in Resources to determine the destination server.
The first line of the file is the server's IP; the second line is the server's TCP port.
