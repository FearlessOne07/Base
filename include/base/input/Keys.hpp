#pragma once
#include <cstdint>

namespace Base
{
  enum class Key : std::uint16_t
  {
    Null = 0,

    // Alphanumeric keys
    Apostrophe,
    Comma,
    Minus,
    Period,
    Slash,

    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,

    Semicolon,
    Equal,

    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    LeftBracket,
    Backslash,
    RightBracket,
    Grave,

    // Function keys
    Space,
    Escape,
    Enter,
    Tab,
    Backspace,
    Insert,
    Delete,

    Right,
    Left,
    Down,
    Up,

    PageUp,
    PageDown,
    Home,
    End,

    CapsLock,
    ScrollLock,
    NumLock,
    PrintScreen,
    Pause,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    LeftShift,
    LeftControl,
    LeftAlt,
    LeftSuper,

    RightShift,
    RightControl,
    RightAlt,
    RightSuper,

    KbMenu,

    // Keypad keys
    Keypad0,
    Keypad1,
    Keypad2,
    Keypad3,
    Keypad4,
    Keypad5,
    Keypad6,
    Keypad7,
    Keypad8,
    Keypad9,

    KeypadDecimal,
    KeypadDivide,
    KeypadMultiply,
    KeypadSubtract,
    KeypadAdd,
    KeypadEnter,
    KeypadEqual,

    Count
  };

  enum class MouseKey : std::uint8_t
  {
    Null = 0,
    Left,
    Right,
    Middle,
    Side,
    Extra,
    Forward,
    Back,
    Count,
  };

  enum class KeyState
  {
    Pressed,
    Released
  };
} // namespace Base
