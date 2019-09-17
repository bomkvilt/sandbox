@echo off
pushd "./build/windows"
for %%f in (*.sln) do (
    start %%f
)
popd
