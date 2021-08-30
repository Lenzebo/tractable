#pragma once
#include "Backend.h"

namespace trbl {
class Session
{
  public:
    Session(Level l = Level::LOWEST) : oldLevel_(getLevel())
    {
        setLevel(l);
        getBackend().beginTracing();
    }
    ~Session()
    {
        getBackend().endTracing();
        setLevel(oldLevel_);
    }

  private:
    Level oldLevel_;
};
}  // namespace trbl
