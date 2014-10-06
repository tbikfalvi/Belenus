-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_4_4.sql
-- AppVersion  : 1.4.4
-- DbVersion   : 1.5.1
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcardunits` ADD `patientCardTypeId` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `patientCardId`;
ALTER TABLE `patientcardunits` ADD INDEX `patientCardTypeId` ( `patientCardTypeId` );
ALTER TABLE `patientcardunits` ADD FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_4_4' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_5_1' WHERE identifier='DATABASE_VERSION';
