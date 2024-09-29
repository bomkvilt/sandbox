load("//tools/modules:modules.bzl", "std_cc_module")


std_cc_module(
    name="example",
    src="lib.cppm",
    impl_srcs=[
        "lib.cpp",
    ],
    copts = [
        "-fmodules",
        "-fbuiltin-module-map",
    ],
)
