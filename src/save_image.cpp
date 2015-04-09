#include "save_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void WindowDump_PPM(void)
{
    int i,j;
    FILE *fptr = NULL;
    static int counter = 0; /* This supports animation sequences */
    char fname[64];
    //char fname2[64];
    //char sysCommand[256];
    unsigned char *image;
    GLint width;
    GLint height;
    GLint viewport[4];
    
    glGetIntegerv (GL_VIEWPORT, viewport);
    width = viewport[2];
    height = viewport[3];
    
    //width = screenWidth;
    //height = screenHeight;
    
    //printf("width = %d  height = %d \n",width,height);
    
    
    // Allocate our buffer for the image
    if ((image = (unsigned char *)malloc(3*width*height*sizeof(char))) == NULL) {
        fprintf(stderr,"Failed to allocate memory for image \n");
        return;
    }
    
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    
    // Open the file
    sprintf(fname,"window%04d.ppm",counter);
    //sprintf(fname2,"window%04d.jpg",counter);
    
    if ((fptr = fopen(fname,"w")) == NULL) {
        fprintf(stderr,"Failed to open file for window dump \n");
        return;
    }
    
    // Copy the image into our buffer
    glReadBuffer(GL_BACK_LEFT);
    glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);
    
    // Write the raw file
    fprintf(fptr,"P6\n%d %d\n255\n",(int)width,(int)height); //for ppm
    for (j=height-1;j>=0;j--) {
        for (i=0;i<width;i++) {
            fputc(image[3*j*width+3*i+0],fptr);
            fputc(image[3*j*width+3*i+1],fptr);
            fputc(image[3*j*width+3*i+2],fptr);
        }
    }
    
    fclose(fptr);
    
    // Clean up
    counter++;
    free(image);
    //sprintf(sysCommand,"convert -type TrueColorMatte %s %s",fname,fname2);
    //sprintf(sysCommand,"convert %s %s",fname,fname2);
    //system(sysCommand);
    //sprintf(sysCommand,"rm %s",fname);
    //system(sysCommand);
    
    //printf("Saved PPM image\n");
}


void WindowDump_PNG(void)
{
    FILE *fptr = NULL;
    static int counter = 0; /* This supports animation sequences */
    char fname[64];
    //unsigned char *image;
    char	*image;
    GLint width;
    GLint height;
    GLint viewport[4];
    
    glGetIntegerv (GL_VIEWPORT, viewport);
    width = viewport[2];
    height = viewport[3];
    
    // Allocate our buffer for the image
    if ((image = (char *) malloc(height*width*3*sizeof(char)) ) == NULL) {
        fprintf(stderr,"write_png: Failed to allocate memory for image \n");
        return;
    }
    
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    
    sprintf(fname,"window%04d.png",counter);
    
    // Copy the image into our buffer
    glReadBuffer(GL_BACK_LEFT);
    glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);
    
    stbi_write_png(fname, width, height, 3, (const void *)image, 0);
    
    counter++;
    free(image);
    fclose(fptr);
    //printf("Saved png image\n");
}

