#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GLEW/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>

namespace hot {

unsigned int shaderProgram, VAO, VBO;
int height = 1366, width = 768;
float x2, y2;

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 position;\n"
    "uniform mat4 MVP;\n"
    "void main()\n"
    "{\n"
    "gl_Position = MVP * vec4(position, 0.0f, 1.0f);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "uniform vec3 u_color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(u_color.xyz, 1.0f);\n"
    "}\n\0";

unsigned int compileandlinkShader()
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

    return shaderProgram;
}

void setline(float x1, float y1, float x2, float y2)
{
    // Set up vertex data (and buffer(s)) and attribute pointers
    float vertices[] = {
        // Positions
        x1, y1,
        x2, y2
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
}

void settriangle(std::vector<float> t)
{
            // Set up vertex data (and buffer(s)) and attribute pointers
    std::vector<float> triangle = t;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), &triangle[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
}

void Pixelstorke(int stroke)
{
    glPointSize(stroke);
}

void linephase(float x1, float y1, float radius, float phi, unsigned int shader_program, float theta = 30.0f)
{
    //--------------         To Draw Arrow's Line         ----------//

    x2 = radius * cos(phi) + x1;
    y2 = radius * sin(phi) + y1;
    setline(x1, y1, x2, y2);

    float L1 = sqrt( (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) );
    float L2 = 10.0f;
    theta = theta * (3.14/180);

    // triangle's coordinates
    float x3 = x2 + (L2/L1) * ((x1 - x2) * cos(theta) + (y1 - y2) * sin(theta));
    float y3 = y2 + (L2/L1) * ((y1 - y2) * cos(theta) - (x1 - x2) * sin(theta));
    float x4 = x2 + (L2/L1) * ((x1 - x2) * cos(theta) - (y1 - y2) * sin(theta));
    float y4 = y2 + (L2/L1) * ((y1 - y2) * cos(theta) + (x1 - x2) * sin(theta));
    std::vector<float> t = {x4, y4, x2, y2, x3, y3};

    glUseProgram(shader_program);
    glUniform3f(glGetUniformLocation(shader_program, "u_color"), 1.0f, 1.0f, 1.0f);
    glBindVertexArray(VAO);

    glm::mat4 projection;
    projection = glm::ortho(0.0f, GLfloat(height), GLfloat(width), 0.0f, 0.0f, 1000.0f);
    glm::mat4 trans(1.0f);
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 mvp = trans * projection;

    // Get their uniform location
    GLint modelLoc = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    // Draw the triangle
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //--------------         To Draw Arrow Head         ----------//
    settriangle(t);
    glUseProgram(shader_program);
    glUniform3f(glGetUniformLocation(shader_program, "u_color"), 1.0f, 1.0f, 1.0f);
    glBindVertexArray(VAO);

    projection = glm::ortho(0.0f, GLfloat(height), GLfloat(width), 0.0f, 0.0f, 1000.0f);
    glm::mat4 trans_one(1.0f);
    trans_one = glm::scale(trans_one, glm::vec3(0.5, 0.5, 0.5));
    mvp = trans_one * projection;

    // Get their uniform location
    modelLoc = glGetUniformLocation(shader_program, "MVP");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

float getArrowCoordX() { return x2; }
float getArrowCoordY() { return y2; }

}