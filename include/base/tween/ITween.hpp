#pragma once

namespace Base
{
  enum struct TweenPriorityLevel
  {
    DEFAULT = 0,
    MEDIUM = 1,
    HIGH = 2,
  };

  class ITween
  {
  public:
    virtual ~ITween();
    virtual void Update(float dt) = 0;
    virtual const void *GetTarget() const = 0;
    virtual bool IsFinished() const = 0;
    virtual void OnEnd() = 0;
    virtual TweenPriorityLevel GetProrityLevel() const = 0;
  };
} // namespace Base
