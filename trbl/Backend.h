#pragma once
#include "Context.h"
#include "Span.h"

#include <string_view>

namespace trbl {

struct CounterEvent
{
    TimeStamp timestamp{};
    uint64_t value{};
};

enum class Level
{
    LOWEST,
    LOW,
    MEDIUM,
    HIGH,
    OFF,
};

class Backend
{
  public:
    virtual ~Backend() = default;

    virtual void beginTracing(){};
    virtual void endTracing(){};

    virtual void writeSpan(const std::string_view name, const Span& span, const Context& context = getContext()) = 0;
    virtual void writeCounter(const std::string_view name, const CounterEvent event,
                              const Context& context = getContext()) = 0;
    virtual void writeEvent(const std::string_view name, const TimeStamp ts, const Context& context = getContext()) = 0;
};

class NullBackend final : public Backend
{
  public:
    void writeSpan(const std::string_view, const Span&, const Context&) override{};
    void writeCounter(const std::string_view name, const CounterEvent event, const Context& context) override {}
    void writeEvent(const std::string_view name, const TimeStamp ts, const Context& context) override {}
};

Backend& getBackend();
void setBackend(std::unique_ptr<Backend>&& backend);

Level getLevel();
void setLevel(Level l);

}  // namespace trbl
