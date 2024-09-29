""" The file contains helper functions that should setup module build environment.
"""

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")


def setup_modules(_ctx=None):
    git_repository(
        name="rnburn_bazel_cpp20_modules",
        commit="0ce4752384fcf0eb903aedaa4dbc8102862d5c21",
        remote="https://github.com/rnburn/rules_cc_module"
    )


setup_modules_extension = module_extension(
    implementation=setup_modules,
)
