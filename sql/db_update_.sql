-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_9_0_0.sql
-- AppVersion  : 1.9.1.0
-- DbVersion   : 1.9.1.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE `settings` SET value = DATE_ADD( NOW(), INTERVAL 60 DAY) WHERE identifier = "LICENCE_LAST_VALIDATED";

-- -----------------------------------------------------------------------------------


 -- -----------------------------------------------------------------------------------

UPDATE settings SET value='1_9_1_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_9_1_0' WHERE identifier='DATABASE_VERSION';
