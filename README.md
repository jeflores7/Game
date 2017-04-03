## Dungeon Crawl Game
### v1.0.3
### Created by:
 * Dane Pitkin
 * Lawrence Khuu
 * Lindsay Stewart
 * Justin Flores


#### TO BUILD:
Compiling requires g++, the NCURSES library, and the standard libraries. The program can be compiled using the included makefile with the following command, used from within the "src" folder where the source files are stored:

    make -f game.make
This will create the executable within the "src" folder.

#### TO EXECUTE:
The program can be executed from console with the command:

    ./DungeonCrawlGame

#### HOW TO PLAY:
You control the '@' symbol. Movement uses the WASD keys or the arrow keys. Monsters are represented by the 'm', and can be attacked by attempting to move into their squares. Items are represented by the '!' symbol and can be collected by moving into their squares. To move between levels, walk onto a ladder. '<' ladders lead up, '>' ladders lead down.

#### Notes:
 * You must attack a monster several times before it dies. You will know when it is dead because it will disappear!
 * The game has been tested only on the Fedora operating system.
