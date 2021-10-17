-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_9_0_1.sql
-- AppVersion  : 1.9.0.1
-- DbVersion   : 1.9.0.1
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE `settings` SET value = DATE_ADD( NOW(), INTERVAL 60 DAY) WHERE identifier = "LICENCE_LAST_VALIDATED";

-- -----------------------------------------------------------------------------------


-- -----------------------------------------------------------------------------------

UPDATE settings SET value='1_9_0_1' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_9_0_1' WHERE identifier='DATABASE_VERSION';
