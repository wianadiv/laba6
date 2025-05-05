#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include <stdlib.h>

char* readShaderSrc(const char* filePath) {
    FILE* f;
    if (fopen_s(&f, filePath, "r") != 0) {
        fprintf(stderr, "Error opening shader file %s\n", filePath);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long l = ftell(f); // длина
    fseek(f, 0, SEEK_SET);

    char* src = (char*)malloc(l + 1);
    if (src == NULL) {
        fprintf(stderr, "Malloc failed\n");
        fclose(f);
        return NULL;
    }

    fread(src, 1, l, f);
    src[l] = '\0'; // завершение строки нулевым символом

    fclose(f);
    return src;
}

GLuint compShader(GLenum shadType, const char* src) {
    GLuint shader = glCreateShader(shadType);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Error compiling shader %d %s\n", shadType, infoLog);
    }

    return shader;
}

GLuint createShader(const char* vertex, const char* fragment) {
    char* vertsrc = readShaderSrc(vertex);
    char* fragsrc = readShaderSrc(fragment);

    if (!vertsrc || !fragsrc) {
        fprintf(stderr, "Error loading shader sources\n");
        free(vertsrc);
        free(fragsrc);
        return 0;
    }

    GLuint vertShader = compShader(GL_VERTEX_SHADER, vertsrc);
    GLuint fragShader = compShader(GL_FRAGMENT_SHADER, fragsrc);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vertShader);
    glAttachShader(prog, fragShader);
    glLinkProgram(prog);

    GLint success;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(prog, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Error linking program %s\n", infoLog);
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    free(vertsrc);
    free(fragsrc);

    return prog;
}

void setUniform(GLuint prog, const char* name, float r, float g, float b, float a) {
    glUniform4f(glGetUniformLocation(prog, name), r, g, b, a);
}

void setUniformMat(GLuint prog, const char* name, glm::mat4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(prog, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void setUniformMat3(GLuint prog, const char* name, glm::mat3 mat) {
    glUniformMatrix3fv(glGetUniformLocation(prog, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void setUniformVec(GLuint prog, const char* name, glm::vec3 vec) {
    glUniform3fv(glGetUniformLocation(prog, name), 1, glm::value_ptr(vec));
}

void setUniformFloat(GLuint prog, const char* name, float value) {
    glUniform1f(glGetUniformLocation(prog, name), value);
}