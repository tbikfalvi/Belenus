@echo off
cls
echo.
echo ============================================================
echo    Belenus language translator process
echo ============================================================
echo.
SET dirCurrent=%cd%
echo Current directory is: %dirCurrent%
echo.
echo This process will guide you the Belenus language translator process.
echo.
pause

:: *********************************************************************************************
:: 
:: Main menu
:: 
:: *********************************************************************************************
:main_menu
cls
echo.
echo ============================================================
echo    Belenus language translator process
echo ============================================================
echo.
echo  1. Rebuild language files ...
echo  2. Translate application language ...
echo  3. Update language files
echo  0. Exit translator process
echo.
echo ------------------------------------------------------------
echo.
echo Select from the menu above and enter the menu item number.
SET /P cmdMenu="> "
IF "%cmdMenu%" == "1" ( GOTO :rebuild_menu )
IF "%cmdMenu%" == "2" ( GOTO :translate_menu )
IF "%cmdMenu%" == "3" ( GOTO :update_files )
IF "%cmdMenu%" == "0" ( GOTO :end_process )
GOTO :proc_error

:: *********************************************************************************************
:: 
:: Main menu / 1. Rebuild language files ...
:: 
:: *********************************************************************************************
:rebuild_menu
cls
echo.
echo ============================================================
echo    Belenus language translator process - rebuild
echo ============================================================
echo.
echo  1. Rebuild 'belenus'
echo  2. Rebuild 'advertisement'
echo  3. Rebuild 'report viewer'
echo  4. Rebuild 'websync'
echo  5. Rebuild 'dbbackup'
echo  0. Return to previous menu ...
echo.
echo ------------------------------------------------------------
echo.
echo Select from the menu above and enter the menu item number.
SET /P cmdMenu="> "
IF "%cmdMenu%" == "1" ( GOTO :rebuild_belenus )
IF "%cmdMenu%" == "2" ( GOTO :rebuild_advertisement )
IF "%cmdMenu%" == "3" ( GOTO :rebuild_reportviewer )
IF "%cmdMenu%" == "4" ( GOTO :rebuild_websync )
IF "%cmdMenu%" == "5" ( GOTO :rebuild_dbbackup )
IF "%cmdMenu%" == "0" ( GOTO :main_menu )
GOTO :proc_error

:: *********************************************************************************************
:rebuild_belenus
cd ../client
C:\DevEnv\Qt\4.8.6\bin\lupdate belenus.pro
echo.
copy *.ts %dirCurrent%\*.ts
cd %dirCurrent%
:return_rebuild_belenus
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :rebuild_menu )
GOTO :return_rebuild_belenus

:: *********************************************************************************************
:rebuild_advertisement
cd ../advertisement
C:\DevEnv\Qt\4.8.6\bin\lupdate advertisement.pro
echo.
copy *.ts %dirCurrent%\*.ts
cd %dirCurrent%
:return_rebuild_advertisement
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :rebuild_menu )
GOTO :return_rebuild_advertisement

:: *********************************************************************************************
:rebuild_reportviewer
cd ../viewer
C:\DevEnv\Qt\4.8.6\bin\lupdate BelenusReportViewer.pro
echo.
copy *.ts %dirCurrent%\*.ts
cd %dirCurrent%
:return_rebuild_reportviewer
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :rebuild_menu )
GOTO :return_rebuild_reportviewer

:: *********************************************************************************************
:rebuild_websync
cd ../websync
C:\DevEnv\Qt\4.8.6\bin\lupdate websync.pro
echo.
copy *.ts %dirCurrent%\*.ts
cd %dirCurrent%
:return_rebuild_websync
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :rebuild_menu )
GOTO :return_rebuild_websync

:: *********************************************************************************************
:rebuild_dbbackup
cd ../dbbackup
C:\DevEnv\Qt\4.8.6\bin\lupdate dbbackup.pro
echo.
copy *.ts %dirCurrent%\*.ts
cd %dirCurrent%
:return_rebuild_dbbackup
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :rebuild_menu )
GOTO :return_rebuild_dbbackup

:: *********************************************************************************************
:: 
:: Main menu / 2. Translate application language ...
:: 
:: *********************************************************************************************
:translate_menu
cls
echo.
echo ============================================================
echo    Belenus language translator process - translate
echo ============================================================
echo.
echo  1. Translate 'belenus'
echo  2. Translate 'advertisement'
echo  3. Translate 'report viewer'
echo  4. Translate 'websync'
echo  5. Translate 'dbbackup'
echo  0. Return to previous menu ...
echo.
echo ------------------------------------------------------------
echo.
echo Select from the menu above and enter the menu item number.
SET /P cmdMenu="> "
IF "%cmdMenu%" == "1" ( GOTO :translate_belenus )
IF "%cmdMenu%" == "2" ( GOTO :translate_advertisement )
IF "%cmdMenu%" == "3" ( GOTO :translate_reportviewer )
IF "%cmdMenu%" == "4" ( GOTO :translate_websync )
IF "%cmdMenu%" == "5" ( GOTO :translate_dbbackup )
IF "%cmdMenu%" == "0" ( GOTO :main_menu )
GOTO :proc_error

:: *********************************************************************************************
:translate_belenus
C:\DevEnv\Qt\4.8.6\bin\linguist belenus_hu.ts
C:\DevEnv\Qt\4.8.6\bin\linguist belenus_ro.ts
:return_translate_belenus
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :translate_menu )
GOTO :return_translate_belenus

:: *********************************************************************************************
:translate_advertisement
C:\DevEnv\Qt\4.8.6\bin\linguist advertisement_hu.ts
C:\DevEnv\Qt\4.8.6\bin\linguist advertisement_ro.ts
:return_translate_advertisement
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :translate_menu )
GOTO :return_translate_advertisement

:: *********************************************************************************************
:translate_reportviewer
C:\DevEnv\Qt\4.8.6\bin\linguist brv_hu.ts
C:\DevEnv\Qt\4.8.6\bin\linguist brv_ro.ts
:return_translate_reportviewer
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :translate_menu )
GOTO :return_translate_reportviewer

:: *********************************************************************************************
:translate_websync
C:\DevEnv\Qt\4.8.6\bin\linguist websync_hu.ts
C:\DevEnv\Qt\4.8.6\bin\linguist websync_ro.ts
:return_translate_websync
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :translate_menu )
GOTO :return_translate_websync

:: *********************************************************************************************
:translate_dbbackup
C:\DevEnv\Qt\4.8.6\bin\linguist dbbackup_hu.ts
C:\DevEnv\Qt\4.8.6\bin\linguist dbbackup_ro.ts
:return_translate_dbbackup
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :translate_menu )
GOTO :return_translate_dbbackup

:: *********************************************************************************************
:: 
:: Main menu / 3. Update language files ...
:: 
:: *********************************************************************************************
:update_files
cd %dirCurrent%
copy *.qm c:\KiwiSun\Belenus\lang\*.qm
copy *.qm c:\KiwiSun\PackageCreate\lang\*.qm
:return_update_files
SET /P cmdMenu="Enter '0' to return > "
IF "%cmdMenu%" == "0" ( GOTO :main_menu )
GOTO :return_update_files

:: *********************************************************************************************
:: 
:: END
:: 
:: *********************************************************************************************
:end_process
echo.
echo Thanks for using this tool. Have a good day! :)
echo.
pause
cls
GOTO:eof

:: *********************************************************************************************
:proc_error
echo.
echo Something went wrong
pause

:: *********************************************************************************************
