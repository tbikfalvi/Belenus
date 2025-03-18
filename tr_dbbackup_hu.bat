@echo off
echo.
cd dbbackup
echo.
echo Update language files
C:\DevEnv\Qt\4.8.6\bin\lupdate dbbackup.pro
echo.
echo Translate language file to HUN
C:\DevEnv\Qt\4.8.6\bin\linguist dbbackup_hu.ts
echo.
echo Copy translations to PackageCreate
copy dbbackup_hu.qm "c:\KiwiSun\PackageCreate\lang\dbbackup_hu.qm"
echo.
echo Copy translations to Belenus
copy dbbackup_hu.qm "c:\KiwiSun\Belenus\lang\dbbackup_hu.qm"
echo.
pause
