cd viewer
lupdate BelenusReportViewer.pro
cd ..
linguist viewer/brv_hu.ts
linguist viewer/brv_us.ts
cd viewer
copy brv_hu.qm "c:\Program Files\Belenus\lang\brv_hu.qm"
copy brv_us.qm "c:\Program Files\Belenus\lang\brv_us.qm"
copy brv_hu.qm "c:\Install\Belenus\lang\brv_hu.qm"
copy brv_us.qm "c:\Install\Belenus\lang\brv_us.qm"
pause
