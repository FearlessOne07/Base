#include "base/systems/InputSystem.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/signals/EventBus.hpp"
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

    bus->SubscribeEvent<MouseButtonEvent>([this](const std::shared_ptr<Event> &event) {
      this->OnMouseButtonEvent(std::static_pointer_cast<MouseButtonEvent>(event)); //
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

  void InputSystem::OnMouseButtonEvent(const std::shared_ptr<MouseButtonEvent> &event)
  {
    for (auto &e : _entities)
    {
      if (event->isHandled)
      {
        break;
      }

      auto *inpcmp = e->GetComponent<InputComponent>();
      if (event->action == MouseButtonEvent::Action::PRESSED)
      {
        for (auto &[key, action] : inpcmp->mousePressedBinds)
        {
          if (event->button == key)
          {
            action();
            event->isHandled = true;
          }
        }
      }
      else if (event->action == MouseButtonEvent::Action::HELD)
      {
        for (auto &[key, action] : inpcmp->mouseDownBinds)
        {
          if (event->button == key)
          {
            action();
            event->isHandled = true;
          }
        }
      }
      else if (event->action == MouseButtonEvent::Action::RELEASED)
      {
        for (auto &[key, action] : inpcmp->mouseReleasedBinds)
        {
          if (event->button == key)
          {
            action();
            event->isHandled = true;
          }
        }
      }
    }
  }
} // namespace Base
//
