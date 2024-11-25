import os
import pathlib
import subprocess
import sys
from typing import Final


_, target, link = sys.argv

# build the target
subprocess.run(
    " ".join(["bazelisk", "build", target]),
    shell=True,
    check=True,
)

# fetch an actual path of the built binary
target_relpath_process: Final = subprocess.run(
    " ".join(
        [
            *["bazelisk", "cquery", target],
            *["-c", "dbg"],
            *["--output=starlark", '--starlark:expr="target.files.to_list()[0].path"'],
        ]
    ),
    stdout=subprocess.PIPE,
    shell=True,  # NOTE: does not work for shell=False
    check=True,
    text=True,
)

target_path: Final = pathlib.Path(target_relpath_process.stdout.strip()).resolve()
assert target_path.exists(), f"binary does not exist: '{target_path}'"

# create a link to the path
link_path: Final = pathlib.Path(link)
if not (link_root := link_path.parent).exists():
    link_root.mkdir()

if link_path.exists(follow_symlinks=False):
    link_path.unlink()

link_path.symlink_to(target_path)
