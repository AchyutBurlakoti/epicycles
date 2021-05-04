#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GLEW/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <math.h>
#include <vector>
#include <algorithm>

#include "arrow.h"
#include "renderer.h"
#include "data.h"

// Function declaration
void                 key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void                 epicycles(double x, double y, double rotation, std::vector<Complex> fourier);
void                 draw();
void                 quick3Sort(int l, int r);
std::vector<Complex> DFT(std::vector<Complex> c);

const unsigned int   WIDTH = 1366, HEIGHT = 768;
float                _time = 0;
std::vector<Complex> ft_datas;
std::vector<float>   pixel;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Epicycles", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;

    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    /* getdata() will return the vector data. It contains datas of image. [TheCodingTrainLogo] */
    std::vector<Complex> datas_from_image = getdata();
    const int            skip             = 20;
    std::vector<Complex> curated_datas;

    /* To get few datas to approximate the image. As more data will have more circles to draw which takes longer
       time to render. But more datas will approximate the image more precisely */
       
    for(int i = 0; i < datas_from_image.size(); i = i + skip)
    {
        curated_datas.push_back(datas_from_image[i]);
    }

    /* Apply Discrete Fourier Transform on curated datas */
    ft_datas = DFT(curated_datas);

    /* Sort the obtained fouier transform datas in descending order according to their amplitude */
    std::sort(ft_datas.begin(), ft_datas.end(), [](const Complex& a, const Complex& b) -> bool
    {
        return a.amp > b.amp;
    });

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        draw();
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

/* This function is used to rearrange the datas of pixel vector in such a way that we can render line easily
   Not complex but if you try to think for a moment on your own you can DIY */
std::vector<float> make(std::vector<float> pixel_co_value)
{
    std::vector<float> temporary;
    int count = 0;
    for (int i = 0; i < pixel_co_value.size(); i += 2)
    {
        count++;
        temporary.push_back(pixel_co_value[i]);
        temporary.push_back(pixel_co_value[i + 1]);
        if (count >= 2 && i != (pixel_co_value.size()) - 2) 
        {
            temporary.push_back(pixel_co_value[i]);
            temporary.push_back(pixel_co_value[i + 1]);
        }
    }
    return temporary;
}

/* Simple functions for maths and rendering. Source : The Coding Train */
void epicycles(double x, double y, double rotation, std::vector<Complex> fourier)
{
    for (int i = 0; i < fourier.size(); i += 1)
    {
        double _freq = fourier[i].freq;
        double _radius = fourier[i].amp;
        double _phase = fourier[i].phase;
        double _phi = _freq * _time + _phase + rotation;

        hot::linephase(x, y,2 * _radius, _phi);
        x = hot::getArrowCoordX();
        y = hot::getArrowCoordY();
    }
    pixel.push_back(hot::getArrowCoordX());
    pixel.push_back(hot::getArrowCoordY());
    render(make(pixel));
}

void draw()
{
    epicycles(WIDTH / 2, HEIGHT / 2, 0, ft_datas);
    float dt = ( 2 * 3.14 ) / ft_datas.size();
    _time += dt;

    if (_time > (2 * 3.14)) { _time = 0; pixel.clear(); }
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

std::vector<Complex> DFT(std::vector<Complex> x)
{
     std::vector<Complex> _X;
     int N = x.size();
     for (int k = 0; k < N; k++)
     {
         Complex sum(0, 0);
         for (int n = 0; n < N; n++)
         {
             double phi = (2  * 3.14 * k * n) / N;
             Complex c(cos(phi), -sin(phi));
             sum.add(x[n].mult(c));
         }
         sum.re = sum.re / N;
         sum.im = sum.im / N;
         double freq = k;
         double amp = sqrt(sum.re * sum.re + sum.im * sum.im);
         double phase = atan2(sum.im, sum.re);
         Complex _c(sum.re, sum.im, freq, amp, phase);
         Complex temp;
         temp.copy(_c);
         _X.push_back(temp);
     }
     return _X;
 }
