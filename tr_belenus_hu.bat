@echo off
echo.
cd client
echo.
echo Update language files
C:\DevEnv\Qt\4.8.6\bin\lupdate belenus.pro
echo.
echo Translate language file to HUN
C:\DevEnv\Qt\4.8.6\bin\linguist belenus_hu.ts
echo.
echo Copy translations to PackageCreate
copy belenus_hu.qm "c:\KiwiSun\PackageCreate\lang\belenus_hu.qm"
echo.
echo Copy translations to Belenus
copy belenus_hu.qm "c:\KiwiSun\Belenus\lang\belenus_hu.qm"
echo.
pause
