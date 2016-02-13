@echo off

echo Delete and recreate belenus database
c:\wamp\bin\mysql\mysql5.5.24\bin\mysql.exe -u root -padminpass belenus < belenus_recreate.sql

echo Fulfill belenus database
c:\wamp\bin\mysql\mysql5.5.24\bin\mysql.exe -u root -padminpass belenus < belenus_archive.sql

echo Process finished
pause

