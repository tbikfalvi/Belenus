-- -----------------------------------------------------------------------------------
--
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : db_update_hu_1_2.sql
-- AppVersion  : 1.1.1
-- DbVersion   : 1.2
-- FileVersion : 1.0
-- Author      : Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- 1.2 adatbazis verziot frissito file
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_1_1' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_2' WHERE identifier='DATABASE_VERSION';
