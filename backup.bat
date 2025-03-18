@echo off
echo.
echo Backup database
echo.
echo Save licences table
c:\wamp\bin\mysql\mysql5.5.24\bin\mysqldump.exe -u belenus -pbelenus belenus licences > C:/backup/backup_test.sql
echo.
echo Save settings table
c:\wamp\bin\mysql\mysql5.5.24\bin\mysqldump.exe -u belenus -pbelenus belenus settings >> C:/backup/backup_test.sql
echo.
pause
