#include "engine.h"
#include "io/keyHandle.h"
#include "modules/render/shader.h"
#include "objects/cuboid.h"
#include "render/camera.h"
#include "scene.h"

#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

extern Engine engine;

void loadDefaultScene() {
  Window window = engine.window;

  unsigned int shader;
  createFullShader(&shader, "assets/shaders/default.vert",
                   "assets/shaders/default.frag", NULL);

  initCuboid();
  Camera *camera = newCamera();
  bindCamera(shader, camera);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  double lastMeasure = glfwGetTime();
  int fps = 0;

  unsigned int uResolution = glGetUniformLocation(shader, "uResolution");
  unsigned int uTime = glGetUniformLocation(shader, "uTime");
  while (!glfwWindowShouldClose(engine.window)) {
    int windowSize[2];
    glfwGetWindowSize(window, windowSize, windowSize + 1);
    glUniform2i(uResolution, windowSize[0], windowSize[1]);

    glUniform1f(uTime, (float)glfwGetTime());

    glClearColor(0.1, 0.15, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader);
    // drawCuboidsInstanced(125000);
    drawCuboid();

    handleCameraMovement(camera);

    // Center cursor if mouseCaptured
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      int windowSize[2];
      glfwGetWindowSize(window, windowSize, windowSize + 1);
      engine.mouseCaptured = 1;
      glfwSetCursorPos(window, windowSize[0] / 2.0, windowSize[1] / 2.0);
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      engine.mouseCaptured = 0;
    }

    glfwSwapBuffers(window);
    if (glfwGetTime() - lastMeasure > 1) {
      printf("FPS: %d\n", fps);
      lastMeasure = glfwGetTime();
      fps = 0;
    } else
      ++fps;
    glfwPollEvents();
  }

  terminateCuboid();
  destroyCamera(camera);

  glUseProgram(0);
  glDeleteProgram(shader);
}