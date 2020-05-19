cd websync
C:\DevEnv\Qt\4.8.6\bin\lupdate websync.pro
C:\DevEnv\Qt\4.8.6\bin\linguist websync_hu.ts
rem C:\DevEnv\Qt\4.8.6\bin\linguist websync_it.ts
C:\DevEnv\Qt\4.8.6\bin\linguist websync_de.ts
copy websync_hu.qm "c:\KiwiSun\PackageCreate\lang\websync_hu.qm"
copy websync_hu.qm "c:\KiwiSun\Belenus\lang\websync_hu.qm"
rem copy websync_it.qm "c:\KiwiSun\PackageCreate\lang\websync_it.qm"
rem copy websync_de.qm "c:\KiwiSun\PackageCreate\lang\websync_de.qm"
pause
