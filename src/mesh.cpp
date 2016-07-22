#include "mesh.h"
#include "defines.h"

void mesh::init(ncio data, GLSLShader *shader, float scale_factor, colormap the_colormap, bool do_fabs){

    enum    {vertex, normal, color, texture, index};

    int i,j;
    float   r = 1.0f;
    float   lat, lon;

    float min_value = 9e12;
    float max_value = -9e12;
    float average;

    //GLint   max;
    //glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &max);
    //cout << "max vertices = " << max << endl;

    // setup colormap data memebers
    // create colormap lut
    init_cmaps_from_data(the_colormap);

    // define the mesh
    // including vertex position, texture coords, normals and/or colors

    width = data.nlon;
    height = data.nlat;


    // 1. collect art assets
    // 2. make colormaps - store colormap output images for inclusion in preentation
    // 3. need around 4 or 5 colormaps per field
    // 4. can use sampe colormap for different data sets
    // 5. need to start collecting art asset metadata for inclusion in presentation
    // 5a. metadata: Artist Name, Name of Work, Year of Work.

    // SLICER UPDATES
    // 1. can we add the read colormap from file code to this?
    // 2. add colormap parameter to command line arguments
    // 3. hotkey different view settings to make image generation easy
    // 4. only need 1 or 2 views per colormap (maybe only 1 view for some fields)
    //    view will be different depending on data field

    // FIELDS TO VISUALIZE

    // 1. OCEANMAPS SST (checked)
    // 2. OCEANMAPS VELOCITY MAGNITUDE (checked)
    // 3. TSUNAMI MAX HEIGHT (checked)
    // 4. ETOPO 1minute TOPOGRAPHY (checked)

    // not tested:
    // 5. WAVEWATCH III Sig Wave Height (checked)
    // 6. access-r - use a storm surge field

    // generate k-means images
    // can we do better than gnuplot??

    // handle _FillValue
    // accessg
    /*
    for (j=height-1;j>=0;j--) {
        for (i=0;i<width;i++) { // longitude
            if (data.field[ i + (width*j)] >= 1e6){
                data.field[ i + (width*j)] = 0.0;
            }
            //else
            //    data.field[ i + (width*j)] = log(data.field[ i + (width*j)]);
        }
    }
    */

    // map the bathymetry to only values greater than 0.0
    // will give a flat ocean...

    if(do_fabs == false){
      for (j=height-1;j>=0;j--) {
          for (i=0;i<width;i++) { // longitude
              if (data.field[ i + (width*j)] <= 0.0){
                  data.field[ i + (width*j)] = 0.0;
              }
              else{
                  //data.field[ i + (width*j)] = log(data.field[ i + (width*j)]);
                  data.field[ i + (width*j)] = sqrt( data.field[ i + (width*j)] );
    							data.field[ i + (width*j)] = sqrt( data.field[ i + (width*j)] );
              }
          }
      }

      // map the geolocated coords into cartesian coords.
      min_value = *min_element(data.field.begin(), data.field.end());
      max_value = *max_element(data.field.begin(), data.field.end());
    }

    else{

      #ifdef _TSUNAMI_
      // tsunami hamaxall
      // access-g accum_ls_prcp
      // make fill values zero
      for (j=height-1;j>=0;j--) {
          for (i=0;i<width;i++) { // longitude
              if (data.field[ i + (width*j)] <= 0.0f){
                  data.field[ i + (width*j)] = 0.0;
                  //cout << "did it" << endl;
              }
              // tranform LOW
              else{
                data.field[ i + (width*j)] = sqrt( data.field[ i + (width*j)] );
  							data.field[ i + (width*j)] = sqrt( data.field[ i + (width*j)] );
              }
              //else
              //  cout << "i,j = " << i << " " << j << " data = " << data.field[ i + (width*j)] << endl;
              //else
              //    data.field[ i + (width*j)] = log(data.field[ i + (width*j)]);
          }
      }
      min_value = *min_element(data.field.begin(), data.field.end());
      max_value = *max_element(data.field.begin(), data.field.end());
      #endif

      #ifdef _WW3_
      // tsunami hamaxall
      // access-g accum_ls_prcp
      // make fill values zero
      for (j=height-1;j>=0;j--) {
          for (i=0;i<width;i++) { // longitude
              if (data.field[ i + (width*j)] <= 0.0f){
                  data.field[ i + (width*j)] = 0.0;
                  //cout << "did it" << endl;
              }
              // tranform LOW
              else{
                data.field[ i + (width*j)] = sqrt( data.field[ i + (width*j)] );
  							data.field[ i + (width*j)] = sqrt( data.field[ i + (width*j)] );
              }
              //else
              //  cout << "i,j = " << i << " " << j << " data = " << data.field[ i + (width*j)] << endl;
              //else
              //    data.field[ i + (width*j)] = log(data.field[ i + (width*j)]);
          }
      }
      min_value = *min_element(data.field.begin(), data.field.end());
      max_value = *max_element(data.field.begin(), data.field.end());
      #endif

      #ifdef _ACCESSR_
      // tsunami hamaxall
      // access-g accum_ls_prcp
      // make fill values zero
      for (j=height-1;j>=0;j--) {
          for (i=0;i<width;i++) { // longitude
              if (data.field[ i + (width*j)] <= 0.0f){
                  data.field[ i + (width*j)] = 0.0;
                  //cout << "did it" << endl;
              }
              // tranform LOW
              else{
                data.field[ i + (width*j)] = sqrt( data.field[ i + (width*j)] );
  							data.field[ i + (width*j)] = sqrt( data.field[ i + (width*j)] );
              }
              //else
              //  cout << "i,j = " << i << " " << j << " data = " << data.field[ i + (width*j)] << endl;
              //else
              //    data.field[ i + (width*j)] = log(data.field[ i + (width*j)]);
          }
      }
      min_value = *min_element(data.field.begin(), data.field.end());
      max_value = *max_element(data.field.begin(), data.field.end());
      #endif


      // OCEANMAPS
      #ifdef _OCEANMAPS_
      // get average value for the field
      // get min and max values for the field
      int count = 0;
      average = 0.0;
      for (j=height-1;j>=0;j--) {
          for (i=0;i<width;i++) { // longitude
              // unpack the data
              //data.field[ i + (width*j)] = data.field[ i + (width*j)]*0.001678518f + 45.0;

              if (data.field[ i + (width*j)] > -1.e+20f){

                // for velocity:
                //data.field[ i + (width*j)] = log(data.field[ i + (width*j)]);
                // we don't mess with the SST field

                  average += data.field[ i + (width*j)];
                  count++;

              }
          }
      }
      average = average/count;
      cout << "mean value = " << average << endl;

      // make fill values equal to the average value
      for (j=height-1;j>=0;j--) {
          for (i=0;i<width;i++) { // longitude
              //data.field[ i + (width*j)] = data.field[ i + (width*j)]*0.001678518f + 45.0;
              if (data.field[ i + (width*j)] <= -1.e+20f){
                  //data.field[ i + (width*j)] = 0.0;
                  //data.field[ i + (width*j)] = average;
                  data.field[ i + (width*j)] = -999.0;
              }
              else{
                  //data.field[ i + (width*j)] -= average;
                  if(data.field[ i + (width*j)] > max_value)
                    max_value = data.field[ i + (width*j)];
                  if(data.field[ i + (width*j)] < min_value)
                    min_value = data.field[ i + (width*j)];
              }

              //else
              //  cout << "i,j = " << i << " " << j << " data = " << data.field[ i + (width*j)] << endl;
              //else
              //    data.field[ i + (width*j)] = log(data.field[ i + (width*j)]);
          }
      }
      #endif
      // END OCEANMAPS

    } // close the if - else split between bathymetry and ocean

    // map the geolocated coords into cartesian coords.
    //min_value = *min_element(data.field.begin(), data.field.end());
    //max_value = *max_element(data.field.begin(), data.field.end());


    // hack for hamaxall
    //min_value = 0.0;
    //max_value = 40.0;


    float range = max_value - min_value;

    cout << "min val = " << min_value << " max val = " << max_value << " range = " << range << endl;


    //cout << "width = " << width << " height = " << height << endl;

    glm::vec4 vert;
    glm::vec2 text_coords;
    glm::vec4 colors;

    for (j=height-1;j>=0;j--) {
        for (i=0;i<width;i++) { // longitude

            // normalized data field value
            //float data_value =( (data.field[ i + (width*j)]- min_value) / range);
            float data_value =( (data.field[ i + (width*j)]));

            //cout << data_value << endl;
            // set up vertex array
            // convert spherical coords to cartesian
            lat = data.lat[j] * M_PI/180.0f;
            lon = data.lon[i] * M_PI/180.0f;

            // map each point to a position in our scene
            // we rescale bathymetry data here
            /*
            vert.x = ( (r+data_value*0.001f) * -cos(lat) * sin(lon));    // x
            vert.y = ( (r+data_value*0.001f) * sin(lat));                // y
            vert.z = ( (r+data_value*0.001f) * -cos(lat) * cos(lon));    // z
            */

            /*
            vert.x = ( (r+data_value*0.001f) * -cos(lat) * sin(lon));    // x
            vert.y = ( (r+data_value*0.001f) * sin(lat));                // y
            vert.z = ( (r+data_value*0.001f) * -cos(lat) * cos(lon));    // z
            */


            // oceanmaps sst
            if(data_value == -999.0){
              vert.x = ( (r+fabs(average)*scale_factor) * -cos(lat) * sin(lon));    // x
              vert.y = ( (r+fabs(average)*scale_factor) * sin(lat));                // y
              vert.z = ( (r+fabs(average)*scale_factor) * -cos(lat) * cos(lon));    // z
            }
            else if(do_fabs == true){ // do this for OCEANMAPS SST
              vert.x = ( (r+fabs(data_value)*scale_factor) * -cos(lat) * sin(lon));    // x
              vert.y = ( (r+fabs(data_value)*scale_factor) * sin(lat));                // y
              vert.z = ( (r+fabs(data_value)*scale_factor) * -cos(lat) * cos(lon));    // z
            }
            else{
              vert.x = ( (r+(data_value)*scale_factor) * -cos(lat) * sin(lon));    // x
              vert.y = ( (r+(data_value)*scale_factor) * sin(lat));                // y
              vert.z = ( (r+(data_value)*scale_factor) * -cos(lat) * cos(lon));    // z
            }
            vert.w = 1.0f;
            vertex_coords.push_back(vert);

            // set up texture coords
            text_coords.x = (1.0f - (float)(i)/(float)(width));
            text_coords.y = (1.0f - (float)j/(float)(height));
            texture_coords.push_back(text_coords);

            // set vertex colors
            // this is a greyscale range
            /*
            colors.x = ( ( (data.field[ i + (width*j)]- min_value) / range) );
            colors.y = ( ( (data.field[ i + (width*j)]- min_value) / range) );
            colors.z = ( ( (data.field[ i + (width*j)]- min_value) / range) );
            */


            if(data_value == -999.0){
              colors.x = 0.2f;
              colors.y = 0.2f;
              colors.z = 0.2f;
              colors.w = 1.0;
            }
            else{
              apply_colormap(data_value, min_value, max_value );
              //cout << mesh_red << " " << mesh_green << " " << mesh_blue << endl;
              colors.x = mesh_red/255.0f;
              colors.y = mesh_green/255.0f;
              colors.z = mesh_blue/255.0f;
              colors.w = 1.0;
            }

            color_coords.push_back(colors);
        }
    }

    //cout << "we have " << vertex_coords.size()/3 << " vertex_coords" << endl;
    // setup triangle face list - this is done per triangle
    // for each triangle we just store the index into the vertex array
    for (j=1;j<height;j++) { // latitude
        // first triangle for this strip

        indices.push_back(j*width - width);
        indices.push_back(j*width);
        indices.push_back( (j*width - width) +1);

        for (i=1;i<width;i++) { // longitude
            indices.push_back(i+(j*width));
            indices.push_back(i+(j*width - width)+1);
        }
        indices.pop_back();
    }

    // triangle list - trianles[i] = index coords for this triangle
    std::vector<glm::vec3> triangles;
    int row=1;
    for(i=0;i<indices.size();i++){
        if(i==row*(width*2)-2){
            i+=2;
            row++;
        }
        triangles.push_back(glm::vec3(indices[i],indices[i+1],indices[i+2]));
    }
    triangles.pop_back(); // trash the last entry

    //cout << "nVertex = " << vertex_coords.size() << endl;
    //cout << "nIndices = " << indices.size() << endl;
    //cout << "ntriangles = " << triangles.size() << endl;

    // calculate vertex normals
    normal_coords.resize(vertex_coords.size(), glm::vec3(0.0f, 0.0f, 0.0f)); // initialize the normals to the zero vector
    std::vector<GLint>  seen;   // count of how many triangles this vertex is a member of
    seen.resize(vertex_coords.size(), 0);

    for (i = 0; i < triangles.size(); i+=2) {
        // down pointing triangle
        // the vertices of this triangle
        GLint ia = triangles[i].x;
        GLint ib = triangles[i].y;
        GLint ic = triangles[i].z;

        glm::vec3 norm = glm::cross(glm::vec3(vertex_coords[ib]) - glm::vec3(vertex_coords[ia]),
                                    glm::vec3(vertex_coords[ic]) - glm::vec3(vertex_coords[ia]));

        //cout<<"norm is "<<glm::to_string(norm)<<endl;

        int v[3];
        v[0] = ia;
        v[1] = ib;
        v[2] = ic;
        for (int j = 0; j < 3; j++) {
            GLint cur_v = v[j];
            normal_coords[cur_v] += norm;
            seen[cur_v]++;
        }

        // up pointing triangle
        // the vertices of this triangle:
        ia = triangles[i+1].x;
        ib = triangles[i+1].y;
        ic = triangles[i+1].z;

        norm = glm::cross(glm::vec3(vertex_coords[ic]) - glm::vec3(vertex_coords[ia]),
                                    glm::vec3(vertex_coords[ib]) - glm::vec3(vertex_coords[ia]));

        //cout<<"norm is "<<glm::to_string(norm)<<endl;

        v[0] = ia;
        v[1] = ib;
        v[2] = ic;
        for (int j = 0; j < 3; j++) {
            GLint cur_v = v[j];
            normal_coords[cur_v] += norm;
            seen[cur_v]++;
        }
    }

    // average the vertex normals
    for (i = 0; i < normal_coords.size(); i++) {
        if (seen[i] > 1) {
            // average
            normal_coords[i].x /= seen[i];
            normal_coords[i].y /= seen[i];
            normal_coords[i].z /= seen[i];
        }
        normal_coords[i] = glm::normalize(normal_coords[i]);
    }


    // color map testing

    /*
    struct RGB colorConverter(int hexValue)
    {
        struct RGB rgbColor;
        rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
        rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
        rgbColor.b = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte

        return rgbColor;
    }
    */

    /*
    // color mapping

    int hexColor[9];
    float   xCoord[9];

    hexColor[0] =  0x352a87;
    hexColor[1] =  0x0363e1;
    hexColor[2] =  0x1485d4;
    hexColor[3] =  0x06a7c6;
    hexColor[4] =  0x38b99e;
    hexColor[5] =  0x92bf73;
    hexColor[6] =  0xd9ba56;
    hexColor[7] =  0xfcce2e;
    hexColor[8] =  0xf9fb0e;

    float red, green, blue;

    for(i=0;i<9;i++){
        red = ((hexColor[i] >> 16) & 0xFF) / 255.0f;
        green = ((hexColor[i] >> 8) & 0xFF) / 255.0f;
        blue = ((hexColor[i]) & 0xFF) / 255.0f;

        xCoord[i] = (float)i/8.0f;
        cout << xCoord[i] << "," << red << "," << green << "," << blue << ","<<endl;
    }
    */


    // create VBO
    //
    // each buffer corresponds to:
    //
    //      buffer[0] = vertices
    //      buffer[1] = normals
    //      buffer[2] = colors
    //      buffer[3] = texture_coords
    //
    // the buffer indexing below uses the enum declared above
    // (enum    {vertex, normal, color, texture};)
    //

    // create VBO's
    glGenBuffers(5, buffers);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << "glGenBuffers failed: error " << error << std::endl;
        exit( 1);
    }


    // create VAO
    glGenVertexArrays(1, vao);
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << "glGenVertexArrays failed: error " << error << std::endl;
        exit( 1);
    }
    glBindVertexArray(vao[0]);
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << "glBindVertexArray failed: error " << error << std::endl;
        exit( 1);
    }

    // set the VAO pointers to the VBO
    if(shader->vertex_coords_name != ""){

        // bind buffer for vertices
        glBindBuffer(GL_ARRAY_BUFFER, buffers[vertex]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, vertex_coords.size()*sizeof(vertex_coords[0]), &vertex_coords[0], GL_STATIC_DRAW);
        // get the attribute location
        vertex_position_attrib_location = shader->GetAttributeLocation(shader->vertex_coords_name);
        // connect the vertex.xyz data to the "v3Position" attribute of the vertex shader
        glEnableVertexAttribArray(vertex_position_attrib_location);
        glVertexAttribPointer(vertex_position_attrib_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

        // now bind the index buffer
        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[index]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    }


    if(shader->normals_name != ""){

        // bind buffer for vertices
        glBindBuffer(GL_ARRAY_BUFFER, buffers[normal]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, normal_coords.size()*sizeof(normal_coords[0]), &normal_coords[0], GL_STATIC_DRAW);
        // get attribute location in shader
        normals_attrib_location = shader->GetAttributeLocation(shader->normals_name);
        // connect the uv texture coords to the normal attribute of the vertex shader
        glEnableVertexAttribArray(normals_attrib_location);
        glVertexAttribPointer(normals_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        //cout << "done normals" << " normals name is " << shader->normals_name << endl;

    }

    if(shader->colors_name != ""){

        // bind buffer for color
        glBindBuffer(GL_ARRAY_BUFFER, buffers[color]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, color_coords.size()*sizeof(color_coords[0]), &color_coords[0], GL_STATIC_DRAW);
        // get attribute location in shader
        colors_attrib_location = shader->GetAttributeLocation(shader->colors_name);
        // connect the color to the color attribute of the vertex shader
        glEnableVertexAttribArray(colors_attrib_location);
        glVertexAttribPointer(colors_attrib_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    }

    if(shader->texture_coords_name != ""){

        // bind buffer for vertices
        glBindBuffer(GL_ARRAY_BUFFER, buffers[texture]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, texture_coords.size()*sizeof(texture_coords[0]), &texture_coords[0], GL_STATIC_DRAW);
        // get attribute location in shader
        texture_coords_attrib_location = shader->GetAttributeLocation(shader->texture_coords_name);
        // connect the uv texture coords to the "v2TextCoord" attribute of the vertex shader
        glEnableVertexAttribArray(texture_coords_attrib_location);
        glVertexAttribPointer(texture_coords_attrib_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }

    // unbind the VAO
    glBindVertexArray(0);

    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << "?? failed: error " << error << std::endl;
        exit( 1);
    }

}


void mesh::draw(){



    //GLenum error;
    /*
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << " pre bindvertexarray failed error = " << error << std::endl;
        //exit(1);
    }
    */

    glBindVertexArray(vao[0]);

    /*
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << " ### bindvertexarray failed error = " << error << std::endl;
        //exit(1);
    }
     */


        // draw strips
    for(int i=0;i<height-1;i++){
    //for(int i=0;i<(height/2)-1;i++){
        glDrawElements(GL_TRIANGLE_STRIP, width*2, GL_UNSIGNED_INT, (void*) ((width*2*i)*sizeof(GLuint)));
        //glDrawElements(GL_LINE_STRIP, width*2, GL_UNSIGNED_INT, (void*) ((width*2*i)*sizeof(GLuint)));
    }
    /*
    for(int i=(height/2)-1;i<(height)-1;i++){
        glDrawElements(GL_TRIANGLE_STRIP, width*2, GL_UNSIGNED_INT,
                       (void*) ((width*2*i)*sizeof(GLuint)));
    }
    */
    /*
    // check for errors
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << "XX error " << error << std::endl;
        exit( 1);
    }
     */

}

// some of these functions are derived from ncview
// here is the header from ncview to cover the GPL requirements

/*
 * Ncview by David W. Pierce.  A visual netCDF file viewer.
 * Copyright (C) 1993 through 2010 David W. Pierce
 *
 * This program  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 3, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License, version 3, for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * David W. Pierce
 * 6259 Caminito Carrena
 * San Diego, CA  92122
 * pierce@cirrus.ucsd.edu
 */


void mesh::init_cmaps_from_data(colormap cm){
  /* the following are original colormaps from ncview */


  if(cm == gauss3){
    init_cmap_from_data( cmap_3gauss   );
    return;
  }
  if(cm == detail){
  init_cmap_from_data( cmap_detail   );
    return;
  }

  if(cm == ssec){
    init_cmap_from_data( cmap_ssec    );
    return;
  }
  /* A. Shchepetkin: new colormaps are added here */

  if(cm == bright){
    init_cmap_from_data( cmap_bright  );
    return;
  }
  if(cm == banded){
    init_cmap_from_data( cmap_banded   );
    return;
  }
  if(cm == rainbow){
    init_cmap_from_data( cmap_rainbow  );
    return;
  }
  if(cm == jaisnb){
    init_cmap_from_data( cmap_jaisnb   );
    return;
  }
  if(cm == jaisnc){
    init_cmap_from_data( cmap_jaisnc   );
    return;
  }
  if(cm == jaisnd){
    init_cmap_from_data( cmap_jaisnd   );
    return;
  }
  if(cm == blu_red){
    init_cmap_from_data( cmap_blu_red  );
    return;
  }
  if(cm == manga){
    init_cmap_from_data( cmap_manga    );
    return;
  }
  if(cm == jet){
    init_cmap_from_data( cmap_jet     );
    return;
  }
  if(cm == wheel){
    init_cmap_from_data( cmap_wheel    );
    return;
  }

  /* the following are the rest of the original colormaps from ncview */

  if(cm == saw3){
    init_cmap_from_data( cmap_3saw     );
    return;
  }
  if(cm == bw){
    init_cmap_from_data( cmap_bw       );
    return;
  }
  if(cm == defaultncview){
    init_cmap_from_data( cmap_default  );
    return;
  }
  if(cm == extrema){
    init_cmap_from_data( cmap_extrema  );
    return;
  }
  if(cm == helix){
    init_cmap_from_data( cmap_helix    );
    return;
  }
  if(cm == helix2){
    init_cmap_from_data( cmap_helix2   );
    return;
  }
  if(cm == hotres){
    init_cmap_from_data( cmap_hotres   );
    return;
  }

  // j colormaps
  if(cm == magma){
    init_cmap_from_data( cmap_j_magma );
    return;
  }
  if(cm == inferno){
    init_cmap_from_data( cmap_j_inferno );
    return;
  }
  if(cm == plasma){
    init_cmap_from_data( cmap_j_plasma );
    return;
  }
  if(cm == viridis){
    init_cmap_from_data( cmap_j_viridis );
    return;
  }
  if(cm == parula){
    init_cmap_from_data( cmap_j_parula );
    return;
  }
  if(cm == artmap){
    init_cmap_from_data( cmap_artmap );
    return;
  }
}

void mesh::init_cmap_from_data( int *data ){

  int	i;

  //fprintf( stderr, "    ... initting cmap >%s< from supplied data\n", colormap_name );

  for( i=0; i<256; i++ ) {
    the_colormap.r[i] = (unsigned char)data[i*3+0];
    the_colormap.g[i] = (unsigned char)data[i*3+1];
    the_colormap.b[i] = (unsigned char)data[i*3+2];
    //cout << the_colormap.r[i] << " " << the_colormap.g[i] << " " << the_colormap.b[i] << endl;
  }
}

void mesh::apply_colormap(float value, float min_val, float max_val ){

  int index;

  index = (255.0/(max_val - min_val)) * (value - min_val);

  //cout << "apply_colormap: index = " << index << " ";
  // clamp
  if(index < 0) index = 0;
  else if(index > 255) index = 255;

  mesh_red = the_colormap.r[index];
  mesh_green = the_colormap.g[index];
  mesh_blue = the_colormap.b[index];

  //cout << "(r,g,b) = " << mesh_red << ", " << mesh_green << ", " << mesh_blue << endl;

}
