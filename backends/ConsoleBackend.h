#pragma once

#include "../Backend.h"

#include <fstream>

namespace trbl::backends {
class ConsoleBackend : public Backend
{
  public:
    void writeTrace(const std::string_view name, const Trace& trace, const Context& context) override;
};
}  // namespace trbl::backends
