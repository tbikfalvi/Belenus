@echo off
echo.
echo ============================================================
echo    Belenus new version package creation process
echo ============================================================
echo.
SET dirCurrent=%cd%
echo Current directory is: %dirCurrent%
echo.
:enter_new_version
echo.
echo Please enter the version number
SET /P strVersion="Version: "
echo.
echo New version number is %strVersion%
echo Package names will be package_%strVersion%.zip
echo.

:enter_version_ok
echo Is the version number is ok? Type (y/n/ex)
SET /P cmdVersionOk="> "
IF "%cmdVersionOk%" == "y" ( GOTO :create_environment_dirs )
IF "%cmdVersionOk%" == "n" ( GOTO :enter_new_version )
IF "%cmdVersionOk%" == "ex" ( GOTO :end_process )
GOTO :proc_error

:create_environment_dirs
echo.
echo Creating package creator environment
echo Create KiwiSun
mkdir c:\KiwiSun
echo Create KiwiSun\PackageCreate
cd c:\KiwiSun
mkdir PackageCreate
echo Create KiwiSun\PackageCreate\sql
cd PackageCreate
mkdir sql
echo Create KiwiSun\LocalPackage
cd c:\KiwiSun
mkdir LocalPackage
echo Return to %dirCurrent%
cd %dirCurrent%
echo.

:open_xml_sql_files
echo ------------------------------------------------------------
echo  Update xml, sql, inf files
echo ------------------------------------------------------------
echo.
echo Create db_update_%strVersion%.sql file
echo.
copy ..\sql\db_update_.sql ..\sql\db_update_%strVersion%.sql
echo.
echo Open files with Notepad++
echo.
start "notepad++.exe" ..\xml\belenus_loc.xml 
start "notepad++.exe" ..\xml\belenus_web.xml 
start "notepad++.exe" ..\xml\install.xml 
start "notepad++.exe" ..\xml\updater.xml 
start "notepad++.exe" ..\sql\db_create.sql 
start "notepad++.exe" ..\sql\db_fill_de.sql 
start "notepad++.exe" ..\sql\db_fill_en.sql 
start "notepad++.exe" ..\sql\db_fill_hu.sql 
start "notepad++.exe" ..\sql\db_update_%strVersion%.sql 
start "notepad++.exe" ..\system.inf 
echo.

echo Shall we continue? Type (y/n)
SET /P cmdVersionOk="> "
IF "%cmdVersionOk%" == "y" ( GOTO :copy_xml_sql_files )
IF "%cmdVersionOk%" == "n" ( GOTO :end_process )
GOTO :proc_error

:copy_xml_sql_files
echo ------------------------------------------------------------
echo  Retrieve xml, sql files
echo ------------------------------------------------------------
echo.
echo Copy files
echo.
copy ..\xml\belenus_loc.xml %dirCurrent%\belenus_loc.xml
copy ..\xml\belenus_web.xml %dirCurrent%\belenus_web.xml
copy ..\xml\install.xml %dirCurrent%\install.xml
copy ..\xml\updater.xml %dirCurrent%\updater.xml
copy ..\sql\db_create.sql %dirCurrent%\db_create.sql
copy ..\sql\db_fill_de.sql %dirCurrent%\db_fill_de.sql
copy ..\sql\db_fill_en.sql %dirCurrent%\db_fill_en.sql
copy ..\sql\db_fill_hu.sql %dirCurrent%\db_fill_hu.sql
copy ..\sql\db_update_%strVersion%.sql %dirCurrent%\*.*
copy ..\sql\db_update_%strVersion%.sql ..\sql\db_update_.sql
copy ..\system.inf %dirCurrent%\system.inf
echo.

echo Shall we continue? Type (y/n)
SET /P cmdVersionOk="> "
IF "%cmdVersionOk%" == "y" ( GOTO :create_update_package )
IF "%cmdVersionOk%" == "n" ( GOTO :end_process )
GOTO :proc_error

:create_update_package
echo ------------------------------------------------------------
echo  Create update package
echo ------------------------------------------------------------
echo.
echo Creating Update package
copy %dirCurrent%\db_update_%strVersion%.sql c:\KiwiSun\PackageCreate\sql\*.*
cd c:\KiwiSun\PackageCreate
%dirCurrent%\pkzip -add -rec -dir %dirCurrent%\Update_%strVersion%.zip
cd %dirCurrent%
echo.

echo Shall we continue? Type (y/n)
SET /P cmdVersionOk="> "
IF "%cmdVersionOk%" == "y" ( GOTO :create_local_update_package )
IF "%cmdVersionOk%" == "n" ( GOTO :end_process )
GOTO :proc_error

:create_local_update_package
echo ------------------------------------------------------------
echo  Create Local update package
echo ------------------------------------------------------------
echo.
echo Copy xml file
copy %dirCurrent%\belenus_loc.xml c:\KiwiSun\LocalPackage\*.*
echo.
echo Copy package file
copy %dirCurrent%\Update_%strVersion%.zip c:\KiwiSun\LocalPackage\*.*
echo.
echo Creating LocalUpdate package
cd c:\KiwiSun\LocalPackage
%dirCurrent%\pkzip -add %dirCurrent%\BelenusUpdate_%strVersion%.zip
cd %dirCurrent%
echo.

echo Shall we continue? Type (y/n)
SET /P cmdVersionOk="> "
IF "%cmdVersionOk%" == "y" ( GOTO :update_master_cd )
IF "%cmdVersionOk%" == "n" ( GOTO :end_process )
GOTO :proc_error

:update_master_cd
echo ------------------------------------------------------------
echo  Update MasterCD
echo ------------------------------------------------------------
echo.
echo Copy language files
copy c:\Install\Belenus\2_Create_Package\lang\*.qm c:\Install\Belenus\4_Update_MasterCD\lang\*.*
echo.
echo Copy package files
copy %dirCurrent%\Update_*.zip c:\Install\Belenus\4_Update_MasterCD\package\*.*
echo.
echo Copy settings files
copy %dirCurrent%\*.xml c:\Install\Belenus\4_Update_MasterCD\settings\*.*
echo. 
echo Copy sql files
copy %dirCurrent%\*.sql c:\Install\Belenus\4_Update_MasterCD\sql\*.*
echo.
echo Copy inf files
copy %dirCurrent%\system.inf c:\Install\Belenus\4_Update_MasterCD\*.*
echo.

echo Shall we continue? Type (y/n)
SET /P cmdVersionOk="> "
IF "%cmdVersionOk%" == "y" ( GOTO :create_master_cd_package )
IF "%cmdVersionOk%" == "n" ( GOTO :end_process )
GOTO :proc_error

:create_master_cd_package
echo ------------------------------------------------------------
echo  Create MasterCD package
echo ------------------------------------------------------------
echo.
echo Creating MasterCD package
cd c:\Install\Belenus\4_Update_MasterCD\
%dirCurrent%\pkzip -add -rec -dir %dirCurrent%\MasterCD_%strVersion%.zip
cd %dirCurrent%
echo.

echo Shall we continue? Type (y/n)
SET /P cmdVersionOk="> "
IF "%cmdVersionOk%" == "y" ( GOTO :update_official_install )
IF "%cmdVersionOk%" == "n" ( GOTO :end_process )
GOTO :proc_error

:update_official_install
echo ------------------------------------------------------------
echo  Update official packages
echo ------------------------------------------------------------
echo.
echo Copy package files
echo.
copy %dirCurrent%\*.zip c:\Install\Belenus\5_Update_Official\*.*
echo.
echo Copy xml file
echo.
copy %dirCurrent%\belenus_web.xml c:\Install\Belenus\5_Update_Official\*.*
echo.

:end_process
echo.
echo Package creation finished
echo.
pause
GOTO:eof

:proc_error
echo.
echo Something went wrong
pause
