#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <GL/glew.h>

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


#include "GLSLShader.h"

#ifndef NETCDFIO_H
#include "netcdfIO.h"
#endif

//
// Colormaps
// colormaps
// static defined
#include "colormaps_bright.h"
#include "colormaps_banded.h"
#include "colormaps_rainbow.h"
#include "colormaps_jaisnb.h"
#include "colormaps_jaisnc.h"
#include "colormaps_jaisnd.h"
#include "colormaps_blu_red.h"
#include "colormaps_manga.h"
#include "colormaps_jet.h"
#include "colormaps_wheel.h"

/* the following are original colormaps from ncview */
#include "colormaps_3gauss.h"
#include "colormaps_3saw.h"
#include "colormaps_bw.h"
#include "colormaps_default.h"
#include "colormaps_detail.h"
#include "colormaps_extrema.h"
#include "colormaps_helix.h"
#include "colormaps_helix2.h"
#include "colormaps_hotres.h"
#include "colormaps_ssec.h"

// jcoolormaps
#include "magma.h"
#include "inferno.h"
#include "plasma.h"
#include "viridis.h"
#include "parula.h"

// artmaps
#include "artmap.h"
#include "artmap_starry_night.h"
#include "pauldavies1.h"
#include "pauldavies2.h"
#include "pauldavies3.h"
#include "pauldavies4.h"
#include "pauldavies5.h"
#include "pauldavies6.h"
#include "pauldavies7.h"
#include "pauldavies8.h"

#include "jacksonpollock1.h"
#include "jacksonpollock2.h"
#include "jacksonpollock3.h"

#include "edwardhopper1.h"
#include "edwardhopper2.h"

#include "jeffwall1.h"

#include "danflavin1.h"
#include "danflavin2.h"

#include "gerhardrichter1.h"

#include "warhol1.h"
#include "warhol2.h"

#include "georgebyrne1.h"
#include "georgebyrne2.h"


typedef struct{
  int r[256];
  int g[256];
  int b[256];
}cmap;

typedef enum {
  gauss3,
  detail,
  ssec,
  bright,
  banded,
  rainbow,
  jaisnb,
  jaisnc,
  jaisnd,
  blu_red,
  manga,
  jet,
  wheel,
  saw3,
  bw,
  defaultncview,
  extrema,
  helix,
  helix2,
  hotres,
  magma,
  inferno,
  plasma,
  viridis,
  parula,
  artmap,
  artmap_starry_night,
  artmap_pauldavies1,
  artmap_pauldavies2,
  artmap_pauldavies3,
  artmap_pauldavies4,
  artmap_pauldavies5,
  artmap_pauldavies6,
  artmap_pauldavies7,
  artmap_pauldavies8,
  artmap_jacksonpollock1,
  artmap_jacksonpollock2,
  artmap_jacksonpollock3,
  artmap_edwardhopper1,
  artmap_edwardhopper2,
  artmap_jeffwall1,
  artmap_danflavin1,
  artmap_danflavin2,
  artmap_gerhardrichter1,
  artmap_warhol1,
  artmap_warhol2,
  artmap_georgebyrne1,
  artmap_georgebyrne2
}colormap;

class mesh{

    //int     resolution;
    //double  radius;
    int     width;
    int     height;

    std::vector<glm::vec4> vertex_coords;
    std::vector<glm::vec2> texture_coords;
    std::vector<glm::vec3> normal_coords;
    std::vector<glm::vec4> color_coords;
    std::vector<GLuint>  indices;

    GLuint  buffers[5];

    //GLSLShader  *shader;
    // colormap stuff
    cmap      the_colormap;
    float     mesh_red;
    float     mesh_green;
    float     mesh_blue;

    public:
        void init(ncio, GLSLShader*, float, colormap, bool);
        void draw();
        //colormaps
        void init_cmaps_from_data(colormap cm);
        void init_cmap_from_data( int *data );
        void apply_colormap(float value, float min_val, float max_val );


        GLint  vertex_position_attrib_location;
        GLint  texture_coords_attrib_location;
        GLint  normals_attrib_location;
        GLint  colors_attrib_location;

        GLuint  vao[1];
};
