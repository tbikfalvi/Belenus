-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_8_0_0.sql
-- AppVersion  : 1.8.0.0
-- DbVersion   : 1.8.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------


-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_8_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_8_0' WHERE identifier='DATABASE_VERSION';
