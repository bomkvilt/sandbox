load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name="c++20.modules",
    module_interfaces=[
        "module.cppm",
        "submodule/a.cppm",
    ],
    srcs=[
        "module.cpp",
        "submodule/a.cpp",
    ],
    deps=[],
    features=[
        "cpp20_module"
    ],
)
