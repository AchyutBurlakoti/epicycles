#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GLEW/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>
#include <vector>

unsigned int shaderProgram, VAO, VBO;
int          height = 1366, width = 768;
float        x2, y2;

const char*  vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 position;\n"
    "uniform mat4 MVP;\n"
    "void main()\n"
    "{\n"
    "gl_Position = MVP * vec4(position, 0.0f, 1.0f);\n"
    "}\0";

const char*  fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

void compileandlinkShader()
{
    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void setvertices(std::vector<float> t)
{
    // Set up vertex data (and buffer(s)) and attribute pointers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, t.size() * sizeof(GLfloat), &t[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
}

void render(std::vector<float> points) 
{
    compileandlinkShader();
    setvertices(points);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glm::mat4 projection = glm::ortho(0.0f, GLfloat(height), GLfloat(width), 0.0f, 0.0f, 1000.0f);
    glm::mat4 trans(1.0f);
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 mvp = trans * projection;

    // Get their uniform location
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mvp[0][0]);

    // Draw the triangle
    glDrawArrays(GL_LINES, 0, points.size() / 2);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}