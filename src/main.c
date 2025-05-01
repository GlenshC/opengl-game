#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "cglm/cglm.h"

#include "gc_logs.h"
#include "gc_material_const.h"
#include "gc_constants.h"
#include "gc_opengl.h"

// #include "shader.h"
// #include "texture.h"
#include "breakout/breakout.h"
#include "main.h"

GLFWwindow* window;
GS_Time game_time;

int main(void)
{
    window = GS_GLFW_INIT();
    if (window == NULL)
        return -1;

    // glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, Window_FramebufferSizeCallback);

    Breakout_Init();

    while (!glfwWindowShouldClose(window))
    {
        Breakout_Update();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}







