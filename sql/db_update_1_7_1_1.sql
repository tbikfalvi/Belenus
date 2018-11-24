-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_7_1_1.sql
-- AppVersion  : 1.7.1.1
-- DbVersion   : 1.7.7
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------


-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_7_1_1' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_7_7' WHERE identifier='DATABASE_VERSION';
