#pragma once

#include "trbl/Backend.h"

#include <fstream>

namespace trbl::backends {
class ConsoleBackend : public Backend
{
  public:
    void writeSpan(const std::string_view name, const Span& span, const Context& context) override;
};
}  // namespace trbl::backends
