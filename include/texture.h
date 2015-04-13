#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>

#include "SOIL.h"

using namespace std;

class texture{
 
    //int width;
    //int height;
    //string  filename;
    

    public:
        GLuint  texture_id;
        void    load_texture(string filename);
    //void    load_texture(slicer *E, string filename, bool rectangular);
};



#endif