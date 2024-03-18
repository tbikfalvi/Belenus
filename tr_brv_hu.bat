@echo off
echo.
cd viewer
echo.
echo Update language files
C:\DevEnv\Qt\4.8.6\bin\lupdate BelenusReportViewer.pro
echo.
echo Translate language file to HUN
C:\DevEnv\Qt\4.8.6\bin\linguist brv_hu.ts
echo.
echo Copy translations to PackageCreate
copy brv_hu.qm "c:\KiwiSun\PackageCreate\lang\brv_hu.qm"
echo.
echo Copy translations to Belenus
copy brv_hu.qm "c:\KiwiSun\Belenus\lang\brv_hu.qm"
pause
