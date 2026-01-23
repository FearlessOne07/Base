#include "internal/rendering/Window.hpp"
#include "GLFW/glfw3.h"
#include <iostream>

namespace Base
{
  static int ToGLFWKey(Key key)
  {
    switch (key)
    {
    case Key::Null:
      return GLFW_KEY_UNKNOWN;

    case Key::Apostrophe:
      return GLFW_KEY_APOSTROPHE;
    case Key::Comma:
      return GLFW_KEY_COMMA;
    case Key::Minus:
      return GLFW_KEY_MINUS;
    case Key::Period:
      return GLFW_KEY_PERIOD;
    case Key::Slash:
      return GLFW_KEY_SLASH;

    case Key::Zero:
      return GLFW_KEY_0;
    case Key::One:
      return GLFW_KEY_1;
    case Key::Two:
      return GLFW_KEY_2;
    case Key::Three:
      return GLFW_KEY_3;
    case Key::Four:
      return GLFW_KEY_4;
    case Key::Five:
      return GLFW_KEY_5;
    case Key::Six:
      return GLFW_KEY_6;
    case Key::Seven:
      return GLFW_KEY_7;
    case Key::Eight:
      return GLFW_KEY_8;
    case Key::Nine:
      return GLFW_KEY_9;

    case Key::Semicolon:
      return GLFW_KEY_SEMICOLON;
    case Key::Equal:
      return GLFW_KEY_EQUAL;

    case Key::A:
      return GLFW_KEY_A;
    case Key::B:
      return GLFW_KEY_B;
    case Key::C:
      return GLFW_KEY_C;
    case Key::D:
      return GLFW_KEY_D;
    case Key::E:
      return GLFW_KEY_E;
    case Key::F:
      return GLFW_KEY_F;
    case Key::G:
      return GLFW_KEY_G;
    case Key::H:
      return GLFW_KEY_H;
    case Key::I:
      return GLFW_KEY_I;
    case Key::J:
      return GLFW_KEY_J;
    case Key::K:
      return GLFW_KEY_K;
    case Key::L:
      return GLFW_KEY_L;
    case Key::M:
      return GLFW_KEY_M;
    case Key::N:
      return GLFW_KEY_N;
    case Key::O:
      return GLFW_KEY_O;
    case Key::P:
      return GLFW_KEY_P;
    case Key::Q:
      return GLFW_KEY_Q;
    case Key::R:
      return GLFW_KEY_R;
    case Key::S:
      return GLFW_KEY_S;
    case Key::T:
      return GLFW_KEY_T;
    case Key::U:
      return GLFW_KEY_U;
    case Key::V:
      return GLFW_KEY_V;
    case Key::W:
      return GLFW_KEY_W;
    case Key::X:
      return GLFW_KEY_X;
    case Key::Y:
      return GLFW_KEY_Y;
    case Key::Z:
      return GLFW_KEY_Z;

    case Key::LeftBracket:
      return GLFW_KEY_LEFT_BRACKET;
    case Key::Backslash:
      return GLFW_KEY_BACKSLASH;
    case Key::RightBracket:
      return GLFW_KEY_RIGHT_BRACKET;
    case Key::Grave:
      return GLFW_KEY_GRAVE_ACCENT;

    case Key::Space:
      return GLFW_KEY_SPACE;
    case Key::Escape:
      return GLFW_KEY_ESCAPE;
    case Key::Enter:
      return GLFW_KEY_ENTER;
    case Key::Tab:
      return GLFW_KEY_TAB;
    case Key::Backspace:
      return GLFW_KEY_BACKSPACE;
    case Key::Insert:
      return GLFW_KEY_INSERT;
    case Key::Delete:
      return GLFW_KEY_DELETE;

    case Key::Right:
      return GLFW_KEY_RIGHT;
    case Key::Left:
      return GLFW_KEY_LEFT;
    case Key::Down:
      return GLFW_KEY_DOWN;
    case Key::Up:
      return GLFW_KEY_UP;

    case Key::PageUp:
      return GLFW_KEY_PAGE_UP;
    case Key::PageDown:
      return GLFW_KEY_PAGE_DOWN;
    case Key::Home:
      return GLFW_KEY_HOME;
    case Key::End:
      return GLFW_KEY_END;

    case Key::CapsLock:
      return GLFW_KEY_CAPS_LOCK;
    case Key::ScrollLock:
      return GLFW_KEY_SCROLL_LOCK;
    case Key::NumLock:
      return GLFW_KEY_NUM_LOCK;
    case Key::PrintScreen:
      return GLFW_KEY_PRINT_SCREEN;
    case Key::Pause:
      return GLFW_KEY_PAUSE;

    case Key::F1:
      return GLFW_KEY_F1;
    case Key::F2:
      return GLFW_KEY_F2;
    case Key::F3:
      return GLFW_KEY_F3;
    case Key::F4:
      return GLFW_KEY_F4;
    case Key::F5:
      return GLFW_KEY_F5;
    case Key::F6:
      return GLFW_KEY_F6;
    case Key::F7:
      return GLFW_KEY_F7;
    case Key::F8:
      return GLFW_KEY_F8;
    case Key::F9:
      return GLFW_KEY_F9;
    case Key::F10:
      return GLFW_KEY_F10;
    case Key::F11:
      return GLFW_KEY_F11;
    case Key::F12:
      return GLFW_KEY_F12;

    case Key::LeftShift:
      return GLFW_KEY_LEFT_SHIFT;
    case Key::LeftControl:
      return GLFW_KEY_LEFT_CONTROL;
    case Key::LeftAlt:
      return GLFW_KEY_LEFT_ALT;
    case Key::LeftSuper:
      return GLFW_KEY_LEFT_SUPER;

    case Key::RightShift:
      return GLFW_KEY_RIGHT_SHIFT;
    case Key::RightControl:
      return GLFW_KEY_RIGHT_CONTROL;
    case Key::RightAlt:
      return GLFW_KEY_RIGHT_ALT;
    case Key::RightSuper:
      return GLFW_KEY_RIGHT_SUPER;

    case Key::KbMenu:
      return GLFW_KEY_MENU;

    case Key::Keypad0:
      return GLFW_KEY_KP_0;
    case Key::Keypad1:
      return GLFW_KEY_KP_1;
    case Key::Keypad2:
      return GLFW_KEY_KP_2;
    case Key::Keypad3:
      return GLFW_KEY_KP_3;
    case Key::Keypad4:
      return GLFW_KEY_KP_4;
    case Key::Keypad5:
      return GLFW_KEY_KP_5;
    case Key::Keypad6:
      return GLFW_KEY_KP_6;
    case Key::Keypad7:
      return GLFW_KEY_KP_7;
    case Key::Keypad8:
      return GLFW_KEY_KP_8;
    case Key::Keypad9:
      return GLFW_KEY_KP_9;

    case Key::KeypadDecimal:
      return GLFW_KEY_KP_DECIMAL;
    case Key::KeypadDivide:
      return GLFW_KEY_KP_DIVIDE;
    case Key::KeypadMultiply:
      return GLFW_KEY_KP_MULTIPLY;
    case Key::KeypadSubtract:
      return GLFW_KEY_KP_SUBTRACT;
    case Key::KeypadAdd:
      return GLFW_KEY_KP_ADD;
    case Key::KeypadEnter:
      return GLFW_KEY_KP_ENTER;
    case Key::KeypadEqual:
      return GLFW_KEY_KP_EQUAL;

    default:
      return GLFW_KEY_UNKNOWN;
    }
  }

  static int ToGLFWMouseButton(MouseKey button)
  {
    switch (button)
    {
    case MouseKey::Left:
      return GLFW_MOUSE_BUTTON_LEFT;
    case MouseKey::Right:
      return GLFW_MOUSE_BUTTON_RIGHT;
    case MouseKey::Middle:
      return GLFW_MOUSE_BUTTON_MIDDLE;
    case MouseKey::Side:
      return GLFW_MOUSE_BUTTON_4;
    case MouseKey::Extra:
      return GLFW_MOUSE_BUTTON_5;
    case MouseKey::Forward:
      return GLFW_MOUSE_BUTTON_6;
    case MouseKey::Back:
      return GLFW_MOUSE_BUTTON_7;
    default:
      return GLFW_MOUSE_BUTTON_LEFT;
    }
  }

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

    glfwSetCharCallback(_window, Window::CharCallBack);
    glfwSetScrollCallback(_window, Window::ScrollCallBack);

    UpdateFrameBufferSize(spec.MinSize.x, spec.MinSize.y);
    UpdateWindowSize(spec.MinSize.x, spec.MinSize.y);
  }

  bool Window::IsClosed() const
  {
    return glfwWindowShouldClose(_window);
  }

  bool Window::IsMinimized()
  {
    return glfwGetWindowAttrib(_window, GLFW_ICONIFIED);
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

  bool Window::GetKeyState(const Key &key)
  {
    return glfwGetKey(_window, ToGLFWKey(key)) == GLFW_PRESS;
  }

  bool Window::GetMouseButtonState(const MouseKey &key)
  {
    return glfwGetMouseButton(_window, ToGLFWMouseButton(key)) == GLFW_PRESS;
  }
} // namespace Base
