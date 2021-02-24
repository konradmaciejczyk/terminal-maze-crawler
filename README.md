# Super Terminal Dungeon Crawler
I made this simple game as end project during my C course at my university.

The point of this game is to collect coins and make through a procedural generated map. You can destroy walls by planting bombs. You lose health when you find yourself in a range of explosion. The game ends when you lose all health points or time is up.

Whole project is written in C and based on ncurses library. If you want to compile this code on your own, make sure you have both gcc compiler and ncurses library installed on your Linux machine. If not, then do so with following commands (for debian based distros):

<!-- Code Blocks -->
```
    libncurses5-dev libncursesw5-dev    (ncurses)
    sudo apt install build-essential    (gcc)
```

Most of modern Linux terminals should be able to display this game just fine.

This project is far from being finished, so you may encounter some bugs during the play, but it's playable at this state. Maybe one day I'll find some time and will to make it finished. For now I decided to leave it as it is.

![](screenshots/main_menu.png)
![](screenshots/game.png)





