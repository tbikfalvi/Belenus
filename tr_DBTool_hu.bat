@echo off
echo.
cd dbtool
echo.
echo Update language files
C:\DevEnv\Qt\4.8.6\bin\lupdate DBTool.pro
echo.
echo Translate language file to HUN
C:\DevEnv\Qt\4.8.6\bin\linguist dbtool_hu.ts
echo.
echo Copy translations to PackageCreate
copy dbtool_hu.qm "c:\KiwiSun\PackageCreate\lang\dbtool_hu.qm"
echo.
echo Copy translations to Belenus
copy dbtool_hu.qm "c:\KiwiSun\Belenus\lang\dbtool_hu.qm"
echo.
pause
