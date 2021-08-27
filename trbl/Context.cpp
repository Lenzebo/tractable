#include "Context.h"

namespace trbl {

Context& getContext()
{
    static thread_local Context context;
    return context;
}

}  // namespace trbl
