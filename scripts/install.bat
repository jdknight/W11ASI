@echo off
setlocal enabledelayedexpansion

set script_path=%~dp0
set base_path=%script_path:~0,-1%
set exe_path="%base_path%\w11asi.exe"

if exist %exe_path% (
    reg add HKCU\Software\Microsoft\Windows\CurrentVersion\Run /t REG_SZ ^
        /v "W11ASI" /d "%exe_path%" /f >NUL
    if !ERRORLEVEL! neq 0 exit /b !ERRORLEVEL!

    echo W11ASI has been configured to startup.
) else (
    echo W11ASI executable is missing!
)
