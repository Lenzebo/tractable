#pragma once
#include "Backend.h"

namespace trbl {
class Session
{
  public:
    Session() { getBackend().beginTracing(); }
    ~Session() { getBackend().endTracing(); }
};
}  // namespace trbl
