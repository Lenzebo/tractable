#include "Backend.h"

namespace trbl {

namespace {
std::unique_ptr<Backend>& getBackendPtr()
{
    static std::unique_ptr<Backend> backendPtr{new NullBackend};
    return backendPtr;
}

Level& getLevelMutable()
{
    static Level l = Level::OFF;
    return l;
}
}  // namespace

Backend& getBackend()
{
    return *getBackendPtr();
}

void setBackend(std::unique_ptr<Backend>&& backend)
{
    getBackendPtr() = std::move(backend);
}

Level getLevel()
{
    return getLevelMutable();
}

void setLevel(Level l)
{
    getLevelMutable() = l;
}

}  // namespace trbl