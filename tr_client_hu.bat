cd client
C:\DevEnv\Qt\4.8.6\bin\lupdate belenus.pro
cd ..
C:\DevEnv\Qt\4.8.6\bin\linguist client/belenus_hu.ts
cd client
copy belenus_hu.qm "c:\Program Files\Belenus\lang\belenus_hu.qm"
pause
