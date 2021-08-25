#pragma once

#include "../Backend.h"

#include <fstream>

namespace trbl::backends {
class LockedThreadsafeBackend : public Backend
{
  public:
    LockedThreadsafeBackend(std::unique_ptr<Backend>&& backend);
    void writeTrace(const std::string_view name, const Trace& trace, const Context& context) override;

  private:
    std::unique_ptr<Backend> backend_;
    std::mutex mutex_;
};
}  // namespace trbl::backends