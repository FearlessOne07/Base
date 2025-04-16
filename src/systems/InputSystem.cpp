#include "base/systems/InputSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/EventBus.hpp"
#include "base/components/InputComponent.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include <memory>

namespace Base
{
  void InputSystem::Start()
  {
    EventBus *bus = EventBus::GetInstance();
    bus->SubscribeEvent<KeyEvent>([this](const std::shared_ptr<Event> &event) {
      this->OnKeyEvent(std::static_pointer_cast<KeyEvent>(event)); //
    });
  }

  void InputSystem::Update(float dt, EntityManager *entitymanager)
  {
    _entities = entitymanager->Query<InputComponent>();
  }

  void InputSystem::OnKeyEvent(const std::shared_ptr<KeyEvent> &event)
  {
    for (auto &e : _entities)
    {
      if (event->isHandled)
      {
        break;
      }

      auto *inpcmp = e->GetComponent<InputComponent>();
      if (event->action == KeyEvent::Action::PRESSED)
      {
        for (auto &[key, action] : inpcmp->keyPressedBinds)
        {
          if (event->key == key)
          {
            action();
            event->isHandled = true;
          }
        }
      }
      else if (event->action == KeyEvent::Action::HELD)
      {
        for (auto &[key, action] : inpcmp->keyDownBinds)
        {
          if (event->key == key)
          {
            action();
            event->isHandled = true;
          }
        }
      }
      else if (event->action == KeyEvent::Action::RELEASED)
      {
        for (auto &[key, action] : inpcmp->keyReleasedBinds)
        {
          if (event->key == key)
          {
            action();
            event->isHandled = true;
          }
        }
      }
    }
  }
} // namespace Base
