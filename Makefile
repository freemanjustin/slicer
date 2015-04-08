###################################################################
#
#  slicer 
#
# freeman.justin@gmail.com
#
##################################################################


OBJ=	./src/main.o \
	./src/camera.o \
	./src/sphere.o \
	./src/GLSLShader.o \
	./external/SOIL/src/SOIL.o \
	./external/SOIL/src/image_DXT.o \
	./external/SOIL/src/image_helper.o \
	./external/SOIL/src/stb_image_aug.o
		


# Include flags
INC=   -D_OS_X_ \
	-I./include \
	-I./external/glm-0.9.6.3/glm \
	-I./external/SOIL/src

# Libraries
LFLAGS= -lGLEW \
	-framework OpenGL \
	-framework GLUT 

CPPFLAGS=	-O3 -g -Wall -Wno-deprecated-declarations

CXX=	g++ $(CPPFLAGS) $(INC) 


# Executable

EXEC=	./bin/slicer

$(EXEC):$(OBJ)
	$(CXX) -o $(EXEC) $(OBJ) $(LFLAGS)

clean:
	rm $(OBJ)
	rm $(EXEC)
