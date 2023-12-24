![Example](https://i.imgur.com/CGqqISA.png)

[More screenshots](https://imgur.com/a/ijCiC3F)

## Download
- [x64](https://github.com/BennyExtreme/KBotExt/releases/latest/download/KBotExt.exe)
- [x86](https://github.com/BennyExtreme/KBotExt/releases/latest/download/KBotExt_x86.exe)

## Technologies used
* FreeType
* cpr
* JsonCpp
* DirectX 11 with ImGui

## Features
* Works every patch
* Launch multiple clients
* Language changer
* Start any lobby/game
* Custom bots difficulty
* The fastest instalock, auto accept, instant message and auto ban
* Automatically pick a secondary, or dodge if your champion is banned
* Instantly mute everyone in champion select
* Dodge lobbies without closing the client
* Mass invite all friends to lobby
* Multi-search op.gg/u.gg/poro.gg etc. with all players in a champ select (works in ranked - lobby reveal)
* Set the best runes for your selected champion from op.gg (works even when runes aren't unlocked)
* Shows which map side you are on, in all gamemodes
* Create lobbies for hidden gamemodes
* Force Jungle/Lane on Nexus Blitz
* Set custom icon/background/status/rank/mastery/challenges visible for everyone
* Set glitched or empty challenge badges (tokens)
* Set invisible profile/lobby banner
* Info of any player using his nickname or id
* List of champions/skins with all info and ability to sort them
* Force close the client instantly
* Mass delete all friends sorted by folders
* Accept or delete all friend requests
* Check the email of any account
* Set custom in-game minimap scale
* Disenchant any loot you want with 1 click (mass disenchant)
* Champion name to id lookup
* Send any custom request to LCU, Riot Client, RTMP, Store and Ledge
* Stream proof
* IFEO debugger for using HTTP debuggers (Fiddler, Charles, etc.)
* Log cleaner
* Ban checker
* Automatically saves your preferences
* Unicode support and customizable window size
* 1 click login with automated client open
* Force client to run without admin

## Development/Building

- **Install [GIT for windows](https://git-scm.com/download/win)**
- **Installing [vcpkg](https://github.com/microsoft/vcpkg#quick-start-windows), [FreeType](https://github.com/ocornut/imgui/tree/master/misc/freetype), [cpr](https://github.com/libcpr/cpr), [JsonCpp](https://github.com/open-source-parsers/jsoncpp)**
  1. Open Windows Command Prompt (**CMD**)
  2. Run `git clone https://github.com/microsoft/vcpkg.git`
  3. `cd vcpkg`
  4. `bootstrap-vcpkg.bat`
  5. `vcpkg integrate install`
  6. `vcpkg install freetype:x64-windows-static` or/and `vcpkg install freetype:x86-windows-static`
  7. `vcpkg install cpr:x64-windows-static` or/and `vcpkg install cpr:x86-windows-static`
  8. `vcpkg install jsoncpp:x64-windows-static` or/and `vcpkg install jsoncpp:x86-windows-static`
- **Building the project**
  * Open Windows Command Prompt (**CMD**)
  * Clone the repository with `git clone https://github.com/BennyExtreme/KBotExt.git`
  * Open **KBotExt.sln** in Visual Studio (Recommended Visual Studio 2022)
  * Set the solution target platform to the x64 or x86
  * Build the project
  * Feel free to make a pull request with your changes :-)

## Upgrading solution
* `devenv "KBotExt.sln" /upgrade`

## FAQ

* When i open KBotExt the program gets stuck on "Fetching skin data..."
  * First solution:
    * Close KBotExt if you have it open
    * Open "Windows Firewall" (`firewall.cpl`)
    * Click on "Turn Windows Defender Firewall on or off"
    * Select "Turn off Windows Defender Firewall (not recommended)" in all the networks fields
    * Try to open KBotExt again, it should work, otherwise follow the steps described in the next solution.
    > If this solution works for you, consider adding an exception for KBotExt in the Firewall instead of disabling it completely.
  * Second solution:
    * Close KBotExt if you have it open
    * Open all the antivirus software you have installed on your PC (including Windows Defender)
    * Disable them
    * Try to open KBotExt again, it should work, otherwise follow the steps described in the next solution.
    > If this solution works for you, consider adding an exception for KBotExt in your antivirus instead of disabling it completely.
* Does this tool injects something inside League of Legends process or its client?
  * No, as also written in the repository description this tool only interfaces with the internal League of Legends API (called LCU) through simple network requests, all this is done externally and using a wrapper called cpr (that uses libcurl).
* Does the avoid button allow you to not have the time restriction on the queue?
  * No, as also written within the tool the button was created to allow you to avoid a game without having to close and reopen the game but simply with a click.
* Does the "Start queue" button allow you to bypass any queue restriction?
  * No.
* I want to play Wukong but I can't find him in KBotExt
  * In KBotExt Wukong is called by its internal name that is MonkeyKing.
