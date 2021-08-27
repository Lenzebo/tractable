
#include "LockedThreadsafeBackend.h"
namespace trbl::backends {

LockedThreadsafeBackend::LockedThreadsafeBackend(std::unique_ptr<Backend>&& backend) : backend_(std::move(backend)) {}

void LockedThreadsafeBackend::writeSpan(const std::string_view name, const Span& span, const Context& context)
{
    std::scoped_lock lock(mutex_);
    backend_->writeSpan(name, span, context);
}
void LockedThreadsafeBackend::beginTracing()
{
    backend_->beginTracing();
}
void LockedThreadsafeBackend::endTracing()
{
    backend_->endTracing();
}

}  // namespace trbl::backends
