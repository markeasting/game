# OpenGL game engine / racing game

# Libraries
See `lib/alure/README.md` how to build and install Alure (OpenAL). Don't forget
to run `sudo ldconfig` after building.

## MacOS

`brew install sdl2`

`brew install glm`

## Linux

`sudo apt-get install libsdl2-2.0-0 libsdl2-dev`

`sudo apt install libglm-dev`

# Building 

`make -j $(nproc)` 

Available targets: `debug` and `release`

Build and run: `make -j $(nproc) && ./bin/debug/game`
