project(slicer)
set(PROJECT_VERSION "0.1")

# OpenGL etc
# ==========
find_package(OpenGL REQUIRED)
find_package(GLEW   )
find_package(GLUT   REQUIRED)


if(NOT GLEW_FOUND)
include_directories("/apps/glew/1.12.0/include")
set(GLEW_LIBRARIES "/apps/glew/1.12.0/lib/libGLEW.dylib")
endif()


# Find all source and include files, which make the dependencies
file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "include/*.h")
file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp" "external/SOIL/*.c")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include 
                     ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
                     ${CMAKE_CURRENT_SOURCE_DIR}/external/glm-0.9.6.3/glm
                     ${CMAKE_CURRENT_SOURCE_DIR}/external/SOIL/src        )

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_link_libraries(
    ${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLUT_LIBRARY}
    /apps/netcdf/4.1.3/lib/libnetcdf.dylib
)

# Atm slicer expects the following directories:
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ln -fs ../bin/bluemarble .)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ln -fs ../bin/sample .)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ln -fs ../bin/shaders .)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ln -fs ../bin/texture .)
