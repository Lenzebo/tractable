#pragma once
#include "Context.h"
#include "Trace.h"

#include <string_view>

namespace trbl {

class Backend
{
  public:
    virtual ~Backend() = default;

    virtual void beginTracing(){};
    virtual void endTracing(){};

    virtual void writeTrace(const std::string_view name, const Trace& trace, const Context& context = getContext()) = 0;
};

class NullBackend : public Backend
{
  public:
    void writeTrace(const std::string_view, const Trace&, const Context&) override{};
};

Backend& getBackend();
void setBackend(std::unique_ptr<Backend>&& backend);

}  // namespace trbl
