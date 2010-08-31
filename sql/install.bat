echo off
echo .
echo Starting database creation process:
ehco .
echo  1. Creating database ...
mysql -u root -pgiraffe11 <belenus.sql
echo  2. Initializing database ...
mysql -u root -pgiraffe11 <init.sql
echo  3. Creating privileges ...
mysql -u root -pgiraffe11 <user.sql
echo .
echo Database creation process finished.
echo .
