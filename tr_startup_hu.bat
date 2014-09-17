cd startup
C:\DevEnv\Qt\4.8.6\bin\lupdate startup.pro
cd ..
C:\DevEnv\Qt\4.8.6\bin\linguist startup/startup_hu.ts
C:\DevEnv\Qt\4.8.6\bin\linguist startup/startup_de.ts
C:\DevEnv\Qt\4.8.6\bin\linguist startup/startup_sk.ts
cd startup
copy startup_hu.qm "c:\Program Files\Belenus\lang\startup_hu.qm"
copy startup_de.qm "c:\Program Files\Belenus\lang\startup_de.qm"
copy startup_sk.qm "c:\Program Files\Belenus\lang\startup_sk.qm"
pause
