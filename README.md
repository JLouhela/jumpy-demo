# jumpy-demo
cocos2d-x testing and demonstration. To put it short, make rabbits jump and dodge bees.

Three weeks of coding after kids are asleep. No previous cocos2d-x knowledge. Not intended for display of best practices, learning by doing and no time for refactoring or proper design. There are no future plans.

# Build instructions
TBH I'm not sure what is required, and I do not have time to really find out. Since cocos2d is part of the build environment, I would suspect that Windows build works out of the box by checking out the repository and opening a solution under proj.win32 directory.

For Android you need NDK r16b and android studio. Follow cocos2d-x instructions on setting up the environment variables. Pre-build android binary can be found under release folder.

Tested with Windows and Galaxy S7 Edge, designed with quite small 1280 x 720 resolution. Android device handles scaling and I do not know what actually happens if the screen ratio is something else.

# Game instructions
Jump bunnies and dodge bees rhytmically. Right bunny is "synced" to bass drum, left bunny is "synced" to snare drum.

## Controls
Swipe up to jump. Swipe up again while airborne to double jump. Swipe down while airborne to dive down faster.

Left side of the screen controls left bunny, right side of the screen controls right bunny.

Game is intented to be played handheld using your thumbs, but be creative!