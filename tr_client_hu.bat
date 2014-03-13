cd client
lupdate belenus.pro
cd ..
linguist client/belenus_hu.ts
linguist client/belenus_us.ts
cd client
copy belenus_hu.qm "c:\Program Files\Belenus\lang\belenus_hu.qm"
copy belenus_us.qm "c:\Program Files\Belenus\lang\belenus_us.qm"
copy belenus_hu.qm "c:\Install\Belenus\lang\belenus_hu.qm"
copy belenus_us.qm "c:\Install\Belenus\lang\belenus_us.qm"
pause
