-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_6_0_0.sql
-- AppVersion  : 1.6.0.0
-- DbVersion   : 1.7.5
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE `ledgertypes` SET `name`='Online bérlet eladás' WHERE `ledgerTypeId`=7;
UPDATE `ledgertypes` SET `name`='Online bérlet feltöltés' WHERE `ledgerTypeId`=8;

ALTER TABLE `panels`
 ADD `cleanTime`    INT UNSIGNED NOT NULL DEFAULT '0' AFTER `maxWorkTime`,
 ADD `maxCleanTime` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `cleanTime`;
 
-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_6_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_7_5' WHERE identifier='DATABASE_VERSION';
