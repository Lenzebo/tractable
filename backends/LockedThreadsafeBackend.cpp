
#include "LockedThreadsafeBackend.h"
namespace trbl::backends {

LockedThreadsafeBackend::LockedThreadsafeBackend(std::unique_ptr<Backend>&& backend) : backend_(std::move(backend)) {}

void LockedThreadsafeBackend::writeTrace(const std::string_view name, const Trace& trace, const Context& context)
{
    std::scoped_lock lock(mutex_);
    backend_->writeTrace(name, trace, context);
}

}  // namespace trbl::backends
