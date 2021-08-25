#include "Backend.h"

namespace trbl {

std::unique_ptr<Backend>& getBackendPtr()
{
    static std::unique_ptr<Backend> backendPtr{new NullBackend};
    return backendPtr;
}

Backend& getBackend()
{
    return *getBackendPtr();
}

void setBackend(std::unique_ptr<Backend>&& backend)
{
    getBackendPtr() = std::move(backend);
}

}  // namespace trbl