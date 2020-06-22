@echo off
REM -------------------------------------------------------
call :cmd_%1 %*
exit 1
REM -------------------------------------------------------
:cmd_gen
    call :call_script "gen_project"
:cmd_vs
    call :call_script "run_project"
:cmd_cf
    pushd "./codeforces"
    call "./control.bat" %2 %3 %4 %5 %6 %7 %8 %9
    popd
    call :cmd_gen
REM -------------------------------------------------------
:call_script
    call "./build/scripts/%~1.bat"
    exit 0
:call_any
    call %1 %2 %3 %4 %5 %6 %7 %8 %9
    exit 0
