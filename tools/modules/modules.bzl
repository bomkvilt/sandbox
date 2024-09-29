"""
The module implements wrappers for module library and binary targets with automatic `use std;` support.
"""

load("@rnburn_bazel_cpp20_modules//cc_module:defs.bzl", "cc_module", "cc_module_binary")


_DEFAULT_DEPS = [
    # "//tools/modules:std",
]


def _patch_deps(deps):
    # TODO: dedup
    return deps + _DEFAULT_DEPS


def std_cc_module(name, deps=[], *args, **kwargs):
    cc_module(
        name=name,
        deps=_patch_deps(deps),
        *args, **kwargs,
    )


def std_cc_binary(name, deps=[], *args, **kwargs):
    cc_module_binary(
        name=name,
        deps=_patch_deps(deps),
        *args, **kwargs,
    )
