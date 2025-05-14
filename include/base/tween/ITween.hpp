#pragma once

namespace Base
{
  struct ITween
  {
    virtual void Update(float dt);
    virtual void *GetTarget() const = 0;
    virtual bool IsFinished() const = 0;
  };
} // namespace Base
