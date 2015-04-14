#include "texture.h"

#include <stdlib.h>

void texture::load_texture(string filename){
    
    this->texture_id = SOIL_load_OGL_texture(
                                           filename.c_str(),
                                           SOIL_LOAD_AUTO,
                                           SOIL_CREATE_NEW_ID,
                                           SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS//| SOIL_FLAG_TEXTURE_RECTANGLE
                                           );
    
    if( this->texture_id == 0 ){
        cerr << "SOIL loading error: " << SOIL_last_result() << endl;
        exit(1);
    }
    
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
}
