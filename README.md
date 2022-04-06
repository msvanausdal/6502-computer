# 6502-computer
This is a build log of a 6502 computer built on breadboards.
## Clock Module
The first thing I wanted to do was build a clock module. The design I came up with was heavily inspired by Ben Eater's clock module in his [how to build an 8bit breadboard computer series](https://eater.net/8bit/), although I wanted to make it my own. My rough plan was to have an astable, monostable, and 1MHz stable clock and then use a multiplexer to switch between the three options.
### Astable Clock
I wanted the astable clock to run quite slow so that I could catch any issues
