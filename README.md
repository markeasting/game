# Libraries
The libraries below are also present in the `lib` directory.

## MacOS

`brew install glfw`

`brew install glm`

## Linux

`sudo apt install libglfw3 libglfw3-dev`

`sudo apt install libglm-dev`

@TODO: replace GLAD with GLEW (`libglew-dev`)? 

# Build (Linux / MacOS)

`make -j 4` 

Available targets: `debug` and `release`

## Notes
- on MacOS, irrKlang is loaded by placing the .dylib files in /usr/local/lib, but they should be included in an App structure, see `-rpath`. 
