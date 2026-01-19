#include "internal/rendering/Window.hpp"
#include <iostream>

namespace Base
{
  void Window::CreateWindow(const WindowSpec &spec)
  {
    // Initialize GLFW
    if (!glfwInit())
    {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      exit(-1);
    }

    // Set the GLFW window hints for OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create a windowed mode window and its OpenGL context
    _window = glfwCreateWindow(spec.MinSize.x, spec.MinSize.y, spec.Title.c_str(), NULL, NULL);
    if (!_window)
    {
      std::cerr << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      exit(-1);
    }

    // Let the WM finish mapping the window
    glfwShowWindow(_window);
    glfwPollEvents();

    // THEN apply limits
    glfwSetWindowSizeLimits(_window, spec.MinSize.x, spec.MinSize.y, GLFW_DONT_CARE, GLFW_DONT_CARE);

    // Make the window's context current
    glfwMakeContextCurrent(_window);

    glfwSetWindowUserPointer(_window, this);
    glfwSwapInterval(0);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cerr << "Failed to initialize GLAD" << std::endl;
      exit(-1);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << glGetString(GL_VERSION) << "\n";
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (int *)(&_glContexData.MaxTextureUnits));

    // Call Backs
    glfwSetWindowSizeCallback(_window, Window::WindowResizeCallBack);
    glfwSetFramebufferSizeCallback(_window, Window::FrameBufferResizeCallBack);
    glfwSetCursorPosCallback(_window, Window::MouseMoveCallBack);

    glfwSetKeyCallback(_window, Window::KeyCallBack);
    glfwSetMouseButtonCallback(_window, Window::MouseButtonCallBack);
    glfwSetCharCallback(_window, Window::CharCallBack);
    glfwSetScrollCallback(_window, Window::ScrollCallBack);

    UpdateFrameBufferSize(spec.MinSize.x, spec.MinSize.y);
    UpdateWindowSize(spec.MinSize.x, spec.MinSize.y);
  }

  bool Window::WindowClosed() const
  {
    return glfwWindowShouldClose(_window);
  }

  void Window::SwapBuffers()
  {
    glfwSwapBuffers(_window);
  }

  void Window::SetMode(const WindowMode &mode)
  {
    switch (mode)
    {
    case WindowMode::Fullscreen: {
      GLFWmonitor *monitor = glfwGetWindowMonitor(_window);
      const GLFWvidmode *mode = glfwGetVideoMode(monitor);

      glfwSetWindowMonitor( //
        _window,
        monitor, // fullscreen
        0, 0,    // ignored in fullscreen
        mode->width, mode->height,
        0 // or 0 if you don't care
      );
      break;
    }
    case WindowMode::Windowed: {

      glfwSetWindowAttrib(_window, GLFW_DECORATED, GL_TRUE);
      break;
    }
    case WindowMode::Borderless: {
      glfwSetWindowAttrib(_window, GLFW_DECORATED, GLFW_FALSE);
      GLFWmonitor *monitor = glfwGetWindowMonitor(_window);
      const GLFWvidmode *mode = glfwGetVideoMode(monitor);

      glfwSetWindowPos(_window, 0, 0);
      glfwSetWindowSize(_window, mode->width, mode->height);
      break;
    }
    }
  }

  void Window::Poll() const
  {
    glfwPollEvents();
  }

  int Window::GetFrameBufferWidth() const
  {
    return _frameBufferWidth;
  }

  Vector2 Window::GetMousePosition() const
  {
    return _mousePosition;
  }

  int Window::GetFrameBufferHeight() const
  {
    return _frameBufferHeight;
  }

  int Window::GetWindowWidth() const
  {
    return _windowWidth;
  }

  int Window::GetWindowHeight() const
  {
    return _windowHeight;
  }

  void Window::Close()
  {
    glfwDestroyWindow(_window);
    glfwTerminate();
  }

  void Window::FrameBufferResizeCallBack(GLFWwindow *window, int width, int height)
  {
    Window *_this = (Window *)glfwGetWindowUserPointer(window);
    _this->UpdateFrameBufferSize(width, height);
  }

  void Window::MouseMoveCallBack(GLFWwindow *window, double x, double y)
  {
    Window *_this = (Window *)glfwGetWindowUserPointer(window);
    _this->_mousePosition.x = x;
    _this->_mousePosition.y = y;
  }

  void Window::KeyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods)
  {
    Window *_this = (Window *)glfwGetWindowUserPointer(window);
    if (_this->_keyCallBack)
    {
      _this->_keyCallBack(key, scancode, action, mods);
    }
  }

  void Window::MouseButtonCallBack(GLFWwindow *window, int button, int action, int mods)
  {
    Window *_this = (Window *)glfwGetWindowUserPointer(window);
    if (_this->_mouseButtonCallBack)
    {
      _this->_mouseButtonCallBack(button, action, mods);
    }
  }

  void Window::ScrollCallBack(GLFWwindow *window, double xoffset, double yoffset)
  {
    Window *_this = (Window *)glfwGetWindowUserPointer(window);
    if (_this->_scrollCallBack)
    {
      _this->_scrollCallBack(xoffset, yoffset);
    }
  }

  void Window::CharCallBack(GLFWwindow *window, unsigned int codepoint)
  {
    Window *_this = (Window *)glfwGetWindowUserPointer(window);
    if (_this->_charCallBack)
    {
      _this->_charCallBack(codepoint);
    }
  }

  void Window::UpdateFrameBufferSize(int width, int height)
  {
    _frameBufferWidth = width;
    _frameBufferHeight = height;
  }

  void Window::WindowResizeCallBack(GLFWwindow *window, int width, int height)
  {
    Window *_this = (Window *)glfwGetWindowUserPointer(window);
    _this->UpdateWindowSize(width, height);
  }

  void Window::UpdateWindowSize(int width, int height)
  {
    _windowWidth = width;
    _windowHeight = height;
  }

  const GLContexData &Window::GetGlContexData() const
  {
    return _glContexData;
  }

  void Window::SetKeyCallback(const KeyCallback &keyCallback)
  {
    _keyCallBack = keyCallback;
  }

  void Window::SetMouseButtonCallback(const MouseButtonCallback &mouseButtonCallback)
  {
    _mouseButtonCallBack = mouseButtonCallback;
  }

  void Window::SetScrollCallback(const ScrollCallback &scrollCallback)
  {
    _scrollCallBack = scrollCallback;
  }

  void Window::SetCharCallback(const CharCallback &charCallback)
  {
    _charCallBack = charCallback;
  }
} // namespace Base
