#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>

/**
 * Creates the shader
 * @param source the shader source
 * @param type the shader type
 * @return the created shader
 */
GLuint createShader(const std::string& source, GLenum type);

/**
 * Creates the program
 * @param vertexShader the vertex shader
 * @param fragmentShader the fragment shader
 * @return the created program
 */
GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);

/**
 * Creates the program
 * @param shaders the shaders
 * @return the created program
 */
GLuint createProgram(const std::vector<GLuint>& shaders);

/**
 * Creates the program
 * @param shaders the shaders
 * @param transformFeedbackVaryings the transform feedback varyings (the variables that are output to the buffer)
 * @return the created program
 */
GLuint createProgram(const std::vector<GLuint>& shaders, const std::vector<const char*>& transformFeedbackVaryings);
