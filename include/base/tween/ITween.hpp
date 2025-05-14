#pragma once

namespace Base
{
  struct ITween
  {
    virtual ~ITween();
    virtual void Update(float dt) = 0;
    virtual void *GetTarget() const = 0;
    virtual bool IsFinished() const = 0;
  };
} // namespace Base
