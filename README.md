# slicer
OpenGL data presentation project for animating Earth system modelling and observation data on a globe.

## Build
slicer depends on the following libraries
- [GLEW](http://glew.sourceforge.net/) 
- a GLUT implementation
- OpenGL

On linux I have tested this with [freeglut](http://freeglut.sourceforge.net/). On OSX I am using the Apple provided GLUT (`-framework GLUT`).

To compile,
```
mkdir bld
cd bld
cmake ..
make
```
Done. The binary will be in `bld/slicer`. 

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
| x,X   | rotate sphere along x axis |
| y,Y   | rotate sphere along y axis |
| z,Z   | rotate sphere along z axis |
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
