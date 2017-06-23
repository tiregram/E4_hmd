# Install

```bash
git clone https://github.com/tiregram/E4_hmd.git
cd E4_hmd
cmake .
make
```
You need to have a symbolic link in lib/x_server to the libwm library(check on my github to get it).

For the Xserver they need to be run in the :99 DISPLAY address(look on the ./start.sh file)
For more information look on the report(sent soon) and presentation (in the doc directory)


# 3D/VR (OpenGL part)
## Done 

 - [x] Do a window,opengl Context
 - [X] Scene 
 - [x] OpenHmdConnection C++ wrapper
 - [x] FBO object for left and right side
 - [x] Shader to correct lens and chromatic distortion of any HMD
 - [x] Simple Object (init,draw,update,remove)
 - [x] TextureSurface Object with PBO
 - [x] 3D Objects
 - [x] Object Container
 - [X] Cube Map

## To DO

 - [ ] Do a VAO by object to improve performance
 - [ ] Extract the mesh to do algorithm like ray 
 - [ ] Improve TextureScreen update to 30,
 - [ ] Do an event manager 
 - [ ] positional binding of the glove in 3D

## corect
 - [ ] flush memory leaks
 - [ ] do only one update texture for 2 side


# Glove
## DONE
 - [x] Flex Sensor
 - [x] Accelerometer
 - [x] real C++ tool chain
 - [x] Bluetooth
 - [x] Homemade Android app to debug
 - [x] C++ computer-side library to collect input data 

## TODO
 - [ ] Improve fixation in the glove
 - [ ] Improve tracking with a band pass filter

## CORRECT


# project 
## DONE
 - [x] CMake
 - [x] Open taiga to new people and clean


## TODO
 - [ ] script to install the dependencies (install_dep.sh(arch,debian))

## corect
