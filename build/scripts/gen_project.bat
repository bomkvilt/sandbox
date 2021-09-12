@echo off
set dir=./build/windows

if not exist "%dir%" mkdir "%dir%"

pushd "%dir%"
cmake -DVCPKG_TARGET_TRIPLET=x64-windows ../..
popd
