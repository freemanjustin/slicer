###################################################################
#
#  slicer 
#
# freeman.justin@gmail.com
#
##################################################################


OBJ=	./src/main.o \
	./src/camera.o \
	./src/sphere.o
		


# Include flags
INC=   -D_OS_X_ \
	-I./include \
	-I./external/glm-0.9.6.3/glm

# Libraries
LFLAGS= -lGLEW \
	-framework OpenGL \
	-framework GLUT \

CPPFLAGS=	-O3 -g -Wall -Wno-deprecated-declarations

CXX=	g++ $(CPPFLAGS) $(INC) 


# Executable

EXEC=	./bin/slicer

$(EXEC):$(OBJ)
	$(CXX) -o $(EXEC) $(OBJ) $(LFLAGS)

clean:
	rm $(OBJ)
	rm $(EXEC)
