@echo off
echo.
cd client
echo.
echo Update language files
C:\DevEnv\Qt\4.8.6\bin\lupdate belenus.pro
echo.
echo Translate language file to HUN
echo.
C:\DevEnv\Qt\4.8.6\bin\linguist belenus_hu.ts
REM echo Translate language file to HUN
REM C:\DevEnv\Qt\4.8.6\bin\linguist belenus_it.ts
echo.
echo Copy translations to PackageCreate
copy belenus_hu.qm "c:\KiwiSun\PackageCreate\lang\belenus_hu.qm"
REM copy belenus_it.qm "c:\KiwiSun\PackageCreate\lang\belenus_it.qm"
echo.
echo Copy translations to Belenus
copy belenus_hu.qm "c:\KiwiSun\Belenus\lang\belenus_hu.qm"
echo.
pause
