#pragma once

#include "base/input/Keys.hpp"
#include "base/util/Type.hpp"
#include "glad/glad.h"
#include "internal/rendering/WindowMode.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <functional>
#include <string>

namespace Base
{

  using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
  using MouseButtonCallback = std::function<void(int button, int action, int mods)>;
  using ScrollCallback = std::function<void(double xoffset, double yoffset)>;
  using CharCallback = std::function<void(unsigned int codepoint)>;

  struct GLContexData
  {
    uint8_t MaxTextureUnits = 0;
  };

  struct WindowSpec
  {
    std::string Title = "Window";
    IVector2 MinSize = {1280, 720};
    bool Vsync = false;
  };

  class Window
  {
  private:
    GLFWwindow *_window = nullptr;

    WindowSpec _spec;
    int _windowWidth = 0;
    int _windowHeight = 0;

    int _frameBufferHeight = 0;
    int _frameBufferWidth = 0;

    Vector2 _mousePosition = {0, 0};

    GLContexData _glContexData;

    // Call Backs
    KeyCallback _keyCallBack;
    MouseButtonCallback _mouseButtonCallBack;
    ScrollCallback _scrollCallBack;
    CharCallback _charCallBack;

  private:
    static void WindowResizeCallBack(GLFWwindow *window, int width, int height);
    static void FrameBufferResizeCallBack(GLFWwindow *window, int width, int height);
    static void MouseMoveCallBack(GLFWwindow *window, double x, double y);

    static void ScrollCallBack(GLFWwindow *, double xoffset, double yoffset);
    static void CharCallBack(GLFWwindow *, unsigned int codepoint);

    void UpdateFrameBufferSize(int width, int height);
    void UpdateWindowSize(int width, int height);

  public:
    const GLContexData &GetGlContexData() const;
    void CreateWindow(const WindowSpec &spec);
    void Close();
    bool IsClosed() const;
    bool IsMinimized();
    void SwapBuffers();
    void Poll() const;
    void SetKeyCallback(const KeyCallback &keyCallback);
    void SetMouseButtonCallback(const MouseButtonCallback &mouseButtonCallback);
    void SetScrollCallback(const ScrollCallback &scrollCallback);
    void SetCharCallback(const CharCallback &charCallback);
    void SetMode(const WindowMode &mode);
    bool GetKeyState(const Key &key);
    bool GetMouseButtonState(const MouseKey &key);

    // Size
    int GetFrameBufferWidth() const;
    int GetFrameBufferHeight() const;

    int GetWindowWidth() const;
    int GetWindowHeight() const;

    Vector2 GetMousePosition() const;
  };

} // namespace Base
