@echo off
set dir=./build/windows

if not exist "%dir%" mkdir "%dir%"

pushd "%dir%"
cmake -j8 ../..
popd
