cd client
C:\DevEnv\Qt\4.8.6\bin\lupdate belenus.pro
cd ..
C:\DevEnv\Qt\4.8.6\bin\linguist client/belenus_hu.ts
C:\DevEnv\Qt\4.8.6\bin\linguist client/belenus_us.ts
cd client
copy belenus_hu.qm "c:\Program Files\Belenus\lang\belenus_hu.qm"
copy belenus_us.qm "c:\Program Files\Belenus\lang\belenus_us.qm"
copy belenus_hu.qm "c:\Install\Belenus\lang\belenus_hu.qm"
copy belenus_us.qm "c:\Install\Belenus\lang\belenus_us.qm"
pause
