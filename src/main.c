#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "ga.h"

static void error(const char* message)
{
    fprintf(stderr, "%s", message);
    glfwTerminate();
    exit(EXIT_FAILURE);
}

static void error_callback(int code, const char* description)
{
    (void)code;
    error(description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void) window;
    glViewport(0, 0, width, height);

    PGA2D p = {0};
    ga_point(p, width / 2.0f, height / 2.0f);
    ga_print(p);
    pga2d_muls(p, p, 0.5f);
    printf("norm: %f\n", pga2d_norm(p));
    ga_print(p);
    printf("norm: %f\n", pga2d_norm(p));

    PGA2D r = {0};
    ga_rotor(r, p, (float) M_PI / 3.0f);
    ga_print(r);
    printf("norm: %f\n", pga2d_norm(r));

    printf("width: %d height: %d\n", width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

struct {
    int width;
    int height;
} typedef Screen;

// world
Screen screen = {640, 480};

int main()
{
    // init
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) error("Failed to initialize GLFW\n");

    // hints
    glfwWindowHint(GLFW_SAMPLES, 4);

    // window
    GLFWwindow* window = glfwCreateWindow(screen.width, screen.height, "SCPC", NULL, NULL);
    if (!window) error("Failed to open GLFW window\n");

    // callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // load gl
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    // setup
    glClearColor(0.4f, 0.3f, 0.4f, 0.0f);

    // loop
    for (;;)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwWindowShouldClose(window)) break;
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}