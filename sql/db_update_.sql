-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_9_0_0.sql
-- AppVersion  : 1.9.0.0
-- DbVersion   : 1.9.0.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcards` ADD `rfId` varchar(30) NOT NULL AFTER `barcode`;
ALTER TABLE `patientcards` ADD `servicecard` BOOLEAN NOT NULL DEFAULT FALSE AFTER `patientId`;

UPDATE `patientcards` SET `servicecard` = '1' WHERE `patientCardId` = 1;
UPDATE `patientcardtypes` SET `units` = '9' WHERE `patientCardTypeId` = 1;

INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES
 (NULL, "HW_RFIDEnabled", 0 ),
 (NULL, "HW_RFIDComPort", 1 );

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_9_0_0_beta' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_9_0_0_beta' WHERE identifier='DATABASE_VERSION';
