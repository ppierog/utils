cc_library(
    name = "static-allocator",
    srcs = ["src/static_allocator.cpp"],
    hdrs = ["include/static_allocator.h"],
    copts = ["-Ialgorithms/include"],
)

cc_test(
    name = "static-allocator-test",
    size = "small",
    srcs = ["test/static_allocator_test.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
