#pragma once
#include "Context.h"
#include "Span.h"

#include <string_view>

namespace trbl {

class Backend
{
  public:
    virtual ~Backend() = default;

    virtual void beginTracing(){};
    virtual void endTracing(){};

    virtual void writeSpan(const std::string_view name, const Span& span, const Context& context = getContext()) = 0;
};

class NullBackend : public Backend
{
  public:
    void writeSpan(const std::string_view, const Span&, const Context&) override{};
};

Backend& getBackend();
void setBackend(std::unique_ptr<Backend>&& backend);

}  // namespace trbl
