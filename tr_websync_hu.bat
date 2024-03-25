@echo off
echo.
cd websync
echo.
echo Update language files
C:\DevEnv\Qt\4.8.6\bin\lupdate websync.pro
echo.
echo Translate language file to HUN
C:\DevEnv\Qt\4.8.6\bin\linguist websync_hu.ts
echo.
echo Copy translations to PackageCreate
copy websync_hu.qm "c:\KiwiSun\PackageCreate\lang\websync_hu.qm"
echo.
echo Copy translations to Belenus
copy websync_hu.qm "c:\KiwiSun\Belenus\lang\websync_hu.qm"
echo.
pause
