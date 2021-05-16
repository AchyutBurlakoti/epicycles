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

unsigned int VAO, VBO;
int          height = 1366, width = 768;
float        x2, y2;

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

void render(std::vector<float> points, unsigned int shader_program) 
{
    setvertices(points);
    glUseProgram(shader_program);
    glUniform3f(glGetUniformLocation(shader_program, "u_color"), 1.0f, 0.0f, 0.0f);
    glBindVertexArray(VAO);

    glm::mat4 projection = glm::ortho(0.0f, GLfloat(height), GLfloat(width), 0.0f, 0.0f, 1000.0f);
    glm::mat4 trans(1.0f);
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 mvp = trans * projection;

    // Get their uniform location
    unsigned int modelLoc = glGetUniformLocation(shader_program, "MVP");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mvp[0][0]);

    // Draw the triangle
    glDrawArrays(GL_LINES, 0, points.size() / 2);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}