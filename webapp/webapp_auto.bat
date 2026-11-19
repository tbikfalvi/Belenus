@echo off
echo.
echo Belenus WebApp automatikus inditasanak beallitasa
echo.

net session >nul 2>&1
if errorlevel 1 (
    echo HIBA: Rendszergazdai jogosultsag szukseges!
    pause
    exit /b 1
)

REGEDIT.EXE /S "%~dp0webapp_auto.reg"

if errorlevel 1 (
    echo HIBA: A Registry beallitas sikertelen!
) else (
    echo A beallitas megtortent.
)

echo.
pause
