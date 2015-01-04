-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_4_14.sql
-- AppVersion  : 1.4.14
-- DbVersion   : 1.6.1
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_4_14' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_6_1' WHERE identifier='DATABASE_VERSION';
