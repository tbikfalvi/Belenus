@echo off
echo.
echo Start Belenus update process
echo ----------------------------
echo.
echo Belenus program directory : %BelenusTarget%
echo Update directory          : %BelenusStartup%
echo Backup directory          : %BelenusBackup%
echo Download directory        : %BelenusResource%
echo.
echo Copy downloaded files to 'Download' directory
copy *.zip "%BelenusResource%/"
echo.
echo Copy update info file to 'Download' directory
copy *.xml "%BelenusResource%/"
echo.
echo Start Belenus updater application
cd %BelenusStartup%
start %BelenusStartup%/BelenusUpdate.exe
echo.
echo Exiting ...
echo.
