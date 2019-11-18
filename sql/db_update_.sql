-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_8_0_2.sql
-- AppVersion  : 1.8.0.2
-- DbVersion   : 1.8.0.2
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcards` ADD `rfId` varchar(30) NOT NULL AFTER `barcode`;
ALTER TABLE `patientcards` ADD `servicecard` BOOLEAN NOT NULL DEFAULT FALSE AFTER `patientId`;
UPDATE `patientcards` SET `servicecard` = '1' WHERE `patientCardId` = 1;

INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES
 (NULL, "HW_RFIDEnabled", 0 ),
 (NULL, "HW_RFIDComPort", 1 );

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_8_0_2' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_8_0_2' WHERE identifier='DATABASE_VERSION';
