# ld41

Failed attempt at writing a NES game for [Ludum Dare 41](https://ldjam.com/events/ludum-dare/41).

The theme for Ludum Dare 41 was "Combine 2 Incompatible Genres".  I decided to combine shmup and real-time strategy genres by writing a shmup that you played in reverse.  Normally, you control a single powerful spaceship, flying over the enemy's base and destroying it.  Instead, the computer would control that spaceship, and you'd need to place defences to protect your base.  You'd win if the enemy was shot down, the computer would win if not.  As well as defences, you could place power stations that do not defend your base but, if the enemy fails to destroy them (because you've defended them well) they allow you to place more powerful defences in future.

I thought it would be fun to write this as a NES game, using [Shiru's Programming NES games in C tutorial](http://shiru.untergrund.net/articles/programming_nes_games_in_c.htm), basing my work off [these examples](https://github.com/RichardJohnn/cc65-nes-examples).  I used [NES Screen Tool](http://forums.nesdev.com/viewtopic.php?t=7237) for graphics, which seems to run fine under [WINE](https://www.winehq.org/) on Linux.

Unfortunately, I had a lot else on this weekend, and ran out of time.  The code is very hacky, but does run a skeleton of the game (enemy flying over scrolling landscape with cursor control - no gameplay yet) - drag-and-drop ld41.nes onto [JSNES](https://jsnes.org/) in your browser to try it out.
