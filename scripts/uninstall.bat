@echo off
reg delete HKCU\Software\Microsoft\Windows\CurrentVersion\Run ^
    /v "W11ASI" /f >NUL
