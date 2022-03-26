#include "../../header/opengl_gui/shader.hpp"

opengl_gui::Shader::Shader(std::string vertexCode, std::string fragmentCode)
{
    if (fragmentCode.size() == 0)
        fragmentCode = "#version 330 core void main(){}";

    const char *vertex_c_str = vertexCode.c_str(),
               *fragment_c_str = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_c_str, 0);
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_c_str, 0);
    glCompileShader(fragmentShader);
    compileErrors(fragmentShader, "FRAGMENT");

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);

    glLinkProgram(id);
    compileErrors(id, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

opengl_gui::Shader::~Shader()
{
    glDeleteProgram(id);
}

void opengl_gui::Shader::activate()
{
    glUseProgram(id);
}

void opengl_gui::Shader::compileErrors(unsigned int shader, std::string type)
{
    int hasCompiled;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (!hasCompiled)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("opengl_gui::Shader SHADER_COMPILATION_ERROR for: " + type + '\n' + infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (!hasCompiled)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("opengl_gui::Shader SHADER_LINKING_ERROR for: " + type + '\n' + infoLog);
        }
    }
}