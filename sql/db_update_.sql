-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_0_0_0.sql
-- AppVersion  : 2.0.0.0
-- DbVersion   : 2.0.0.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE `settings` SET value = DATE_ADD( NOW(), INTERVAL 60 DAY) WHERE identifier = "LICENCE_LAST_VALIDATED";

-- -----------------------------------------------------------------------------------

ALTER TABLE `licences` ADD `type` VARCHAR( 10 ) NOT NULL DEFAULT 'UNVERIFIED' AFTER `lastValidated`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_0_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_0_0_0' WHERE identifier='DATABASE_VERSION';
