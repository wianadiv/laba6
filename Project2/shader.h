#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include <stdlib.h>

char* readShaderSrc(const char* filePath); // ������ �����

GLuint compShader(GLenum shadType, const char* src); // ���������� ��������

//GLuint createShader(const char* vertex, const char* fragment, GLint* time_location); // �������� ��������

GLuint createShader(const char* vertex, const char* fragment); // �������� ��������

void setUniform(GLuint prog, const char* name, float r, float g, float b, float a); // uniform ����������

void setUniformMat(GLuint prog, const char* name, glm::mat4);

void setUniformMat3(GLuint prog, const char* name, glm::mat3);

void setUniformVec(GLuint prog, const char* name, glm::vec3);

void setUniformFloat(GLuint prog, const char* name, float value);

#endif SHADER_H