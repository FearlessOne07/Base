#include "base/ui/UIElement.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include <memory>
namespace Base
{
  void UIElement::Update() {};
  void UIElement::OnKeyEvent(const std::shared_ptr<KeyEvent> &event)
  {
  }
  void UIElement::OnKeyEvent(const std::shared_ptr<MouseButtonEvent> &event)
  {
  }
} // namespace Base
