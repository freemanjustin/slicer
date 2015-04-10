# slicer
OpenGL data presentation project for animating Earth system modelling and observation data on a globe.

## Build
slicer depends on the following libraries
- [GLEW](http://glew.sourceforge.net/) 
- a GLUT implementation

On linux I have tested this with [freeglut](http://freeglut.sourceforge.net/). On OSX I am using the Apple provided GLUT (`-framework GLUT`).

Two Makefiles are provided, one for OSX and another for Linux. You will (probably) need to edit the Makefile to point the location of `include` and `lib` directories on your system. 

To build slicer type `make -f Makefile.[platform]` at the top level directory where `[platform]` is either `OSX` or `Linux`. The app will be created in the `bin` directory.

## Usage

slicer is a work in progress and things are changing rapidly. slicer will load a sample texture and render this on a sphere. You can navigate the scene using the mouse and keyboard.

| Key | Action          |
| ------------- | ----------- |
| w      | move camera forward|
| s     | move camera backward    |
| a     | move camera left    |
| d     | move camera right    |
| q     | move camera down    |
| e     | move camera up    |
| j     | rotate sphere left    |
| l     | rotate sphere right    |
| r     | reset the view    |
| v     | save current scene as png    |
| esc     | quit    |

## Sample Output
![slicer sample](https://raw.github.com/freemanjustin/slicer/master/bin/sample/sample.png)
![slicer sample2](https://raw.github.com/freemanjustin/slicer/master/bin/sample/sample2.png)

## Sample animations

Sea surface height anomaly

[![Sea surface height anomaly](http://img.youtube.com/vi/t8sZM0F1Noc/0.jpg)](https://www.youtube.com/watch?v=t8sZM0F1Noc)

Sea surface velocity magnitude

[![Sea surface velocity magnitude](http://img.youtube.com/vi/InvkqPuB8mo/0.jpg)](https://www.youtube.com/watch?v=InvkqPuB8mo)

# Author
freeman.justin@gmail.com
