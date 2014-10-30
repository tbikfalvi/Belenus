cd startup
C:\DevEnv\Qt\4.8.6\bin\lupdate startup.pro
C:\DevEnv\Qt\4.8.6\bin\linguist startup_hu.ts
C:\DevEnv\Qt\4.8.6\bin\linguist startup_de.ts
echo C:\DevEnv\Qt\4.8.6\bin\linguist startup_sk.ts
copy startup_hu.qm "c:\Install\Belenus\Desktop\MasterCD\lang\startup_hu.qm"
copy startup_de.qm "c:\Install\Belenus\Desktop\MasterCD\lang\startup_de.qm"
echo copy startup_sk.qm "c:\Install\Belenus\Desktop\MasterCD\lang\startup_sk.qm"
pause
