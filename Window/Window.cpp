// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Window.h"

#include <assert.h>
#include <iostream>
//#include <tools\Log.h>

Window::Window(const glm::uvec2 &size)
  : mSize(size)
{
  GLFWmonitor *monitor = nullptr;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 0);

  mWindow = (decltype(mWindow))(glfwCreateWindow(mSize.x, mSize.y, "title", monitor, nullptr));

  if (!mWindow)
  {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 0);

    mWindow = (decltype(mWindow))(glfwCreateWindow(mSize.x, mSize.y, "title", monitor, nullptr));
  }

  if (!mWindow)
  {
    throw "Window not created.";
  }

  /// Привязываем к glfw окну указатель на объект WindowGL.
  glfwSetWindowUserPointer(mWindow.get(), this);

  glfwSetKeyCallback(mWindow.get(), [](GLFWwindow *win, int key, int scancode, int action, int mods)
  {
    Window *window = static_cast<Window *>(glfwGetWindowUserPointer(win));
    assert(window);
    if (window->mKeyboardCallback && key > 0) window->mKeyboardCallback(key, scancode, action, mods);
    //ImGui_ImplGlfwGL3_KeyCallback(win, key, scancode, action, mods);
  });
  /*
  glfwSetMouseButtonCallback(mWindow.get(), ImGui_ImplGlfwGL3_MouseButtonCallback);
  glfwSetScrollCallback(mWindow.get(), ImGui_ImplGlfwGL3_ScrollCallback);
  glfwSetCharCallback(mWindow.get(), ImGui_ImplGlfwGL3_CharCallback);
  glfwSetWindowSizeCallback(mWindow.get(), ResizeCallback);
  */
}

Window::~Window()
{
}

void Window::WindowSystemInitialize()
{
  glfwSetErrorCallback([](int ,const char* description)
    {
    });

  if (glfwInit() != GL_TRUE)
  {
    throw "GLFW not initialized.";
  }

  //LOG(info) << "GLFW: " << glfwGetVersionString();
}

void Window::WindowSystemFinally()
{
  glfwTerminate();
}

void Window::SetCurrentContext()
{
  assert(mWindow);
  glfwMakeContextCurrent(mWindow.get());

  glfwSwapInterval(0);

  const GLubyte* renderer = glGetString (GL_RENDERER);
  const GLubyte* version = glGetString (GL_VERSION);
  std::cout << "Renderer: " << renderer << std::endl
    << "OpenGL version supported: " << version << std::endl;
}

bool Window::WindowShouldClose()
{
  assert(mWindow);
  return glfwWindowShouldClose(mWindow.get()) == GL_TRUE;
}

void Window::Update()
{
  assert(mWindow);
  glfwSwapBuffers(mWindow.get());
  glfwPollEvents();
}

const glm::uvec2 & Window::GetSize() const
{
  return mSize;
}


void Window::SetTitle(const std::string &title)
{
  assert(mWindow);
  glfwSetWindowTitle(mWindow.get(), title.c_str());
}

void Window::SetKeyboardCallback(const std::function<void(int, int, int, int)> &callback)
{
  mKeyboardCallback = callback;
}


GLFWwindow * Window::Get()
{
  return mWindow.get();
}
