-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_5_1_5.sql
-- AppVersion  : 1.5.1.5
-- DbVersion   : 1.7.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------


-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_5_1_5' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_7_0' WHERE identifier='DATABASE_VERSION';
