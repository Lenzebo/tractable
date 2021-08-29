cc_library(
    name = "core",
    srcs = [
        "Backend.cpp",
        "Context.cpp",
    ],
    hdrs = [
        "Attribute.h",
        "Backend.h",
        "Context.h",
        "Session.h",
        "Span.h",
        "Trace.h",
        "Counter.h",
    ],
)

cc_library(
    name = "backends",
    srcs = [
        "backends/AsyncBackend.cpp",
        "backends/ChromeTracingBackend.cpp",
        "backends/ConsoleBackend.cpp",
        "backends/LockedThreadsafeBackend.cpp",
    ],
    hdrs = [
        "backends/AsyncBackend.h",
        "backends/ChromeTracingBackend.h",
        "backends/ConsoleBackend.h",
        "backends/LockedThreadsafeBackend.h",
    ],
    deps = [":core"]
)

cc_library(
    name = "trbl",
    deps = [
        ":backends",
        ":core",
    ],
    visibility = ["//visibility:public"],
)
