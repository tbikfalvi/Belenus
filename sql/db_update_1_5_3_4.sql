-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_5_3_4.sql
-- AppVersion  : 1.5.3.4
-- DbVersion   : 1.7.1
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------


-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_5_3_4' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_7_1' WHERE identifier='DATABASE_VERSION';
