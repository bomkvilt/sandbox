@echo off
REM -------------------------------------------------------
set "root=./cpp/tests"
set "dest=./cpp-debug/main.target.txt"
call :cmd_%1 %*
exit /B 0
REM -------------------------------------------------------
:cmd_add
    set "complexity=%2"
    set "testID=%3"
    set "testName=%4"
    call :call_any "node" "./builder/main.mjs" "add" %complexity% %testID% %testName% -r %root%
    exit /B 0
:cmd_test
    set "testID=%2"
    call :call_any "node" "./builder/main.mjs" "test" %testID% -r %root% --tf %dest%
    exit /B 0
REM -------------------------------------------------------
:call_any
    call %1 %2 %3 %4 %5 %6 %7 %8 %9
