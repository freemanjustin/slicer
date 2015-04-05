//A simple class for handling GLSL shader compilation
//Auhtor: Movania Muhammad Mobeen
#pragma once
#include <GL/glew.h>
#include <map>
#include <string>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;

class GLSLShader
{
public:
	GLSLShader(void);
	~GLSLShader(void);	
	void LoadFromString(GLenum whichShader, const string& source);
	void LoadFromFile(GLenum whichShader, const string& filename);
	void CreateAndLinkProgram();
	void enable();
	void disable();
	void AddAttribute(const string& attribute);
	void AddUniform(const string& uniform);
	GLuint GetProgram() const;
	//An indexer that returns the location of the attribute/uniform
	GLuint operator[](const string& attribute);
	GLuint operator()(const string& uniform);
	//Program deletion
	void DeleteProgram() {glDeleteProgram(_program);_program=-1;}
    // j additions
    void SetUniform(const string& parameter, int n);
    void SetUniform(const string& parameter, float p1);
    void SetUniform(const string& parameter, glm::vec3 p);
private:
	enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};
	GLuint	_program;
	int _totalShaders;
	GLuint _shaders[3];//0->vertexshader, 1->fragmentshader, 2->geometryshader
	map<string,GLuint> _attributeList;
	map<string,GLuint> _uniformLocationList;
};	
