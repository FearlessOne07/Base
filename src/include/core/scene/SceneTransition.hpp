#pragma once
#include <any>
#include <stdexcept>
#include <string>
#include <typeinfo>

enum class SceneRequest
{
  NONE = 0,
  QUIT,
  PUSH_NEW_SCENE,
  POP_CURRENT_SCENE,
  REPLACE_CURRENT_SCENE
};

class SceneData
{
  std::any _data;

public:
  SceneData() = default;
  ~SceneData() = default;

  template <typename T> void Set(T value)
  {
    _data = std::move(value);
  }

  template <typename T> T Get() const
  {
    if (!_data.has_value())
    {
      throw std::runtime_error("SceneData is empty.");
    }

    // Check type safety
    if (_data.type().hash_code() != typeid(T).hash_code())
    {
      throw std::runtime_error("SceneData type mismatch. Expected: " + std::string(typeid(T).name()) +
                               ", but got: " + std::string(_data.type().name()));
    }

    return std::any_cast<T>(_data);
  }
  bool HasValue() const
  {
    return _data.has_value();
  }
};

struct SceneTransition
{
  SceneRequest request = SceneRequest::NONE;
  int sceneID = -1;
  SceneData data = SceneData();
};