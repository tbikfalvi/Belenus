cd update
lupdate BelenusUpdater.pro
cd ..
linguist update/BelenusUpdater_hu.ts
cd update
copy BelenusUpdater_hu.qm "c:\Program Files\Belenus\lang\BelenusUpdater_hu.qm"
copy BelenusUpdater_hu.qm "c:\Install\Belenus\lang\BelenusUpdater_hu.qm"
pause
