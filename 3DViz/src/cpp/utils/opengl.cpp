#include "opengl.h"

#include "exception.h"

GLuint createShader(const std::string& source, GLenum type) {
    const char* data = source.data();

    // Create the shader, set the shader source and compile the shader
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &data, nullptr);
    glCompileShader(shader);

    //The shader compilation status
    GLint status;

    // Get the shader compilation status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    // Throw the exception with the shader compilation log if the shader isn't compiled
    if (status == GL_FALSE) {
        GLint length;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        char* log = new char[length];

        glGetShaderInfoLog(shader, length, nullptr, log);

        std::string message = "createShader: the ";

        if (type == GL_VERTEX_SHADER) message += "vertex";
        else if (type == GL_FRAGMENT_SHADER) message += "fragment";

        message += " shader compilation completed with errors:\n";
        message += log;

        delete[] log;

        throw Exception(message);
    }

    return shader;
}

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) {
    return createProgram({ vertexShader, fragmentShader });
}

GLuint createProgram(const std::vector<GLuint>& shaders) {
    return createProgram(shaders, { });
}

GLuint createProgram(const std::vector<GLuint>& shaders, const std::vector<const char*>& transformFeedbackVaryings) {
    // Create the program, attach the shaders and link the program
    GLuint program = glCreateProgram();

    for (GLuint shader : shaders) glAttachShader(program, shader);

    if (!transformFeedbackVaryings.empty()) {
        glTransformFeedbackVaryings(program, transformFeedbackVaryings.size(), transformFeedbackVaryings.data(), GL_INTERLEAVED_ATTRIBS);
    }

    glLinkProgram(program);

    // The program linking status
    GLint status;

    // Get the program linking status
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    // Throw the exception with the program linking log if the program isn't linked
    if (status == GL_FALSE) {
        GLint length;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        char* log = new char[length];

        glGetProgramInfoLog(program, length, nullptr, log);

        std::string message = "createProgram: the program linking completed with errors:\n";
        message += log;

        delete[] log;

        throw Exception(message);
    }

    return program;
}
