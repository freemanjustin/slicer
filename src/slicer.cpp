#include "slicer.h"



void slicer::init(){

    XrotationAngle = 0.0f;
    YrotationAngle = 0.0f;
    ZrotationAngle = 0.0f;

    //Setup camera
    //camera.SetMode(SPHERICAL);
    camera.SetMode(FREE);
    camera.SetPosition(glm::vec3(-2.0f, 0.0f, 1.3f));

    //camera.SetPosition(glm::vec3(-1.1f, 0.0f, 1.6f));

    camera.SetLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    /*
    cout << "----- INIT --------" << endl;
    cout << "x: " << camera.camera_position.x;
    cout << " y: " << camera.camera_position.y;
    cout << " z: " << camera.camera_position.z << endl;
    cout << "lx: " << camera.camera_look_at.x;
    cout << " ly: " << camera.camera_look_at.y;
    cout << " lz: " << camera.camera_look_at.z << endl;
    cout << "-------------------" << endl;
    */
    camera.SetClipping(.001f, 100.0f);
    camera.SetFOV(45.0f);


    // load the shaders
    groundFromSpace.LoadFromFile(GL_VERTEX_SHADER,"shaders/GroundFromSpaceVert.glsl");
    groundFromSpace.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/GroundFromSpaceFrag.glsl");
    groundFromSpace.CreateAndLinkProgram();
    groundFromSpace.SetAttributeName(GLSLShader::vertex_coords,"v3Position");

    skyFromSpace.LoadFromFile(GL_VERTEX_SHADER,"shaders/SkyFromSpaceVert.glsl");
    skyFromSpace.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/SkyFromSpaceFrag.glsl");
    skyFromSpace.CreateAndLinkProgram();
    skyFromSpace.SetAttributeName(GLSLShader::vertex_coords,"v3Position");

    groundFromAtmosphere.LoadFromFile(GL_VERTEX_SHADER,"shaders/GroundFromAtmosphereVert.glsl");
    groundFromAtmosphere.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/GroundFromAtmosphereFrag.glsl");
    groundFromAtmosphere.CreateAndLinkProgram();
    groundFromAtmosphere.SetAttributeName(GLSLShader::vertex_coords,"v3Position");

    skyFromAtmosphere.LoadFromFile(GL_VERTEX_SHADER,"shaders/SkyFromAtmosphereVert.glsl");
    skyFromAtmosphere.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/SkyFromAtmosphereFrag.glsl");
    skyFromAtmosphere.CreateAndLinkProgram();
    skyFromAtmosphere.SetAttributeName(GLSLShader::vertex_coords,"v3Position");


    // shader variables
    as.m_nSamples = 32;		// Number of sample rays to use in integral equation
    as.m_Kr = 0.0035f;		// Rayleigh scattering constant
    as.m_Kr4PI = as.m_Kr*4.0f*M_PI;
    as.m_Km = 0.0015f;		// Mie scattering constant
    as.m_Km4PI = as.m_Km*4.0f*M_PI;
    as.m_ESun = 6.0f;		// was 10 // Sun brightness constant
    as.m_g = -0.85f;		// The Mie phase asymmetry factor
    as.m_g2 = as.m_g*as.m_g;

    as.m_fInnerRadius = 1.0f;
    as.m_fInnerRadius2 = as.m_fInnerRadius*as.m_fInnerRadius;
    as.m_fOuterRadius = 1.025f;
    as.m_fOuterRadius2 = as.m_fOuterRadius*as.m_fOuterRadius;
    as.m_fScale = 1.0f / (as.m_fOuterRadius - as.m_fInnerRadius);


    as.m_fWavelength = glm::vec3(0.650f, 0.590f, 0.475f);
    // 650 nm for red
    // 570 nm for green
    // 475 nm for blue
    as.m_fWavelength4 = glm::pow(as.m_fWavelength, glm::vec3(4.0f,4.0f,4.0f));
    as.m_fWavelength4_inv = 1.0f/as.m_fWavelength4;

    as.m_fRayleighScaleDepth = 0.25f;
    as.m_fMieScaleDepth = 0.1f;

    as.m_fScaleOverScaleDepth = as.m_fScale / as.m_fRayleighScaleDepth;

    as.light_pos = glm::vec3(0.0, 0.0, -4.2);
    as.light_direction = glm::normalize(as.light_pos);

    // init sphere
    ground.init(200, glm::vec3(0.0f, 0.0f, 0.0f), as.m_fInnerRadius, &groundFromSpace);
    sky.init(200, glm::vec3(0.0f, 0.0f, 0.0f), as.m_fOuterRadius, &skyFromSpace);


    /*
     // texture mapping
     texMap.LoadFromFile(GL_VERTEX_SHADER,"shaders/textureMap.vert");
     texMap.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/textureMap.frag");
     texMap.CreateAndLinkProgram();
     texMap.SetAttributeName(vertex_coords,"v3Position");
     texMap.SetAttributeName(texture_coords,"v2TexCoord");

     texSphere.init(200, glm::vec3(0.0f, 0.0f, 0.0f), m_fInnerRadius);
     */


    /*
    texMap.LoadFromFile(GL_VERTEX_SHADER,"shaders/texBlend_contrast.vert");
    texMap.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/texBlend_contrast.frag");
    texMap.CreateAndLinkProgram();
    texMap.SetAttributeName(GLSLShader::vertex_coords,"v3Position");
    texMap.SetAttributeName(GLSLShader::texture_coords,"v2TexCoord");


    texSphere.init(300, glm::vec3(0.0f, 0.0f, 0.0f), as.m_fInnerRadius+0.005, &texMap);

    // load two sample texture maps
    continents.load_texture("bluemarble/cont8.png");
    field.load_texture("texture/test.jpg");
    */


#ifdef NCIO

    // load bathymetry
    //bathy.fname = "bathymetry/etopo1min_nc4.nc";
    //bathy.fname = "bathymetry/aust20a.nc"; // 2 lats 2 lons
    //bathy.fname = "bathymetry/aust10.nc";
    //bathy.fname = "bathymetry/strip2.nc";
    //bathy.fname = "bathymetry/sub5a.nc";
    //bathy.fname = "bathymetry/sub5.nc"; // too big for lappy
    //bathy.fname = "bathymetry/aust5a.nc"; // 5 lats, 6 lons
    //bathy.fname = "bathymetry/aust5.nc"; //



    bathy.fname = "bathymetry/sub5.nc";
    bathy.lat_name = "lat";
    bathy.lon_name = "lon";
    bathy.field_name = "z";



  // tsunami

    #ifdef _TSUNAMI_
    field.fname = "bathymetry/hamaxall.nc";
    field.lat_name = "lat";
    field.lon_name = "lon";
    field.field_name = "max_height";
    #endif

/*
    field.fname = "bathymetry/accessg.nc";
    field.lat_name = "lat";
    field.lon_name = "lon";
    field.field_name = "hi_cld";
*/

  #ifdef _WW3_
  field.fname = "bathymetry/wave.nc";
  field.lat_name = "lat";
  field.lon_name = "lon";
  field.field_name = "sig_wav_ht";
  #endif

    // oceanmaps
    #ifdef _OCEANMAPS_
      field.fname = "bathymetry/omap_speed.nc";
      field.lat_name = "yt_ocean";
      field.lon_name = "xt_ocean";
      //field.field_name = "sst";
      field.field_name = "speed";
    #endif

    #ifdef _ACCESSR_
    field.fname = "bathymetry/yasi_strs.nc";
    field.lat_name = "lat";
    field.lon_name = "lon";
    field.field_name = "strs";
    #endif

    bathy.get_data();
    field.get_data();


   #endif



    passThrough.LoadFromFile(GL_VERTEX_SHADER,"shaders/pass_through.vert");
    passThrough.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/pass_through.frag");
    passThrough.CreateAndLinkProgram();
    passThrough.SetAttributeName(GLSLShader::vertex_coords,"v4Position");
    passThrough.SetAttributeName(GLSLShader::colors,"v3Color");
    passThrough.SetAttributeName(GLSLShader::normals,"v3Normal");


    /*
    // normal vector rendering
    renderNormals.LoadFromFile(GL_VERTEX_SHADER,"shaders/normals.vert");
    renderNormals.LoadFromFile(GL_GEOMETRY_SHADER,"shaders/normals.geom");
    renderNormals.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/normals.frag");
    renderNormals.CreateAndLinkProgram();
    renderNormals.SetAttributeName(GLSLShader::vertex_coords,"position");
    renderNormals.SetAttributeName(GLSLShader::normals,"normal");
    */

    //test_sphere.init(200, glm::vec3(0.0f, 0.0f, 0.0f), as.m_fOuterRadius, &renderNormals);


    cout << "doing bathy_mesh init" << endl;
    bathy_mesh.init(bathy,&passThrough, 0.0003f, bw, false);
    cout << "doing field_mesh init" << endl;

    // OCEANMAPS
    // OCEANMAPS SST scale = 0.0008f
    #ifdef _OCEANMAPS_
    //field_mesh.init(field,&passThrough, 0.0008f, artmap, true);
    // OCEANMAPS speed
    field_mesh.init(field,&passThrough, 0.005f, artmap, true);
    #endif

    // tsunami
    #ifdef _TSUNAMI_
    field_mesh.init(field,&passThrough, 0.002f, artmap, false);
    #endif

    // waves
    #ifdef _WW3_
    field_mesh.init(field,&passThrough, 0.0001f, artmap, false);
    #endif

    #ifdef _ACCESSR_
    field_mesh.init(field,&passThrough, 0.005f, artmap, false);
    #endif

    //bathy_mesh_normals.init(bathy,&renderNormals);


    lineShader.LoadFromFile(GL_VERTEX_SHADER,"shaders/line.vert");
    //lineShader.LoadFromFile(GL_GEOMETRY_SHADER,"shaders/line.geom");
    lineShader.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/line.frag");
    lineShader.CreateAndLinkProgram();
    lineShader.SetAttributeName(GLSLShader::vertex_coords,"v3Position");
    lineShader.SetAttributeName(GLSLShader::colors,"v3Color");


    continent.init(&lineShader);
    //exit(0);

    // trash this later
    drawThis = false;

}
