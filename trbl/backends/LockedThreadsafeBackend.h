#pragma once

#include "trbl/Backend.h"

#include <fstream>

namespace trbl::backends {
class LockedThreadsafeBackend : public Backend
{
  public:
    LockedThreadsafeBackend(std::unique_ptr<Backend>&& backend);
    void writeSpan(const std::string_view name, const Span& span, const Context& context) override;
    void beginTracing() override;
    void endTracing() override;

  private:
    std::unique_ptr<Backend> backend_;
    std::mutex mutex_;
};
}  // namespace trbl::backends