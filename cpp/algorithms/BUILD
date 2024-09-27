cc_library(
    name = "static-allocator",
    hdrs = ["include/static_allocator.h"],
    copts = ["-Ialgorithms/include"],
)

cc_test(
    name = "static-allocator-test",
    size = "small",
    srcs = ["test/static_allocator_test.cpp"],
    copts = ["-Ialgorithms/include", "-g", "-O0"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        ":static-allocator",
    ],
)
