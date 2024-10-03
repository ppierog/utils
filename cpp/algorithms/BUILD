cc_library(
    name = "static-allocator",
    hdrs = ["include/static_allocator.h"],
    copts = ["-Ialgorithms/include"],
)

cc_library(
    name = "bitset",
    hdrs = ["include/bitset.h"],
    copts = ["-Ialgorithms/include"],
)

cc_library(
    name = "array",
    hdrs = ["include/array.h"],
    copts = ["-Ialgorithms/include"],
)

cc_library(
    name = "meta",
    hdrs = ["include/meta.h"],
    copts = ["-Ialgorithms/include"],
)


cc_test(
    name = "static-allocator-test",
    size = "small",
    srcs = ["test/static_allocator_test.cpp"],
    copts = ["-Ialgorithms/include", "-DENABLE_UTILS_EXCEPTIONS=1"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        ":static-allocator",
    ],
)

cc_test(
    name = "bitset-test",
    size = "small",
    srcs = ["test/bitset_test.cpp"],
    copts = ["-Ialgorithms/include"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        ":bitset",
    ],
)

cc_test(
    name = "array-test",
    size = "small",
    srcs = ["test/array_test.cpp"],
    copts = ["-Ialgorithms/include"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        ":array",
    ],
)

cc_test(
    name = "meta-test",
    size = "small",
    srcs = ["test/meta_test.cpp"],
    copts = ["-Ialgorithms/include"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        ":meta",
    ],
)
