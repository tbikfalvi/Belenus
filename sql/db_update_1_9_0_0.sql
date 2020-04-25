-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_9_0_0.sql
-- AppVersion  : 1.9.0.0
-- DbVersion   : 1.9.0.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE `settings` SET value = DATE_ADD( NOW(), INTERVAL 60 DAY) WHERE identifier = "LICENCE_LAST_VALIDATED";

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcards` ADD `rfId` varchar(30) NOT NULL AFTER `barcode`;
ALTER TABLE `patientcards` ADD `servicecard` BOOLEAN NOT NULL DEFAULT FALSE AFTER `patientId`;

UPDATE `patientcards` SET `servicecard` = '1' WHERE `patientCardId` = 1;
UPDATE `patientcardtypes` SET `units` = '9' WHERE `patientCardTypeId` = 1;

INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES
 (NULL, "HW_RFIDEnabled", 0 ),
 (NULL, "HW_RFIDComPort", 1 );

UPDATE `licences` SET `lastValidated` = DATE_ADD( NOW( ) , INTERVAL 60 DAY ) WHERE `licenceId` > 1;

INSERT INTO `users` (`userId`, `licenceId`, `name`, `realName`, `password`, `accGroup`, `comment`, `modified`, `active`, `archive`) VALUES 
 (NULL, '0', 'kiwiadmin', 'Kiwisun Adminisztrátor', '1180796965429c0903c2ef9194121493a5127e44', '2', 'Kiwisun Adminisztrátor', '0000-00-00 00:00:00', '1', 'ARC');

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_9_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_9_0_0' WHERE identifier='DATABASE_VERSION';
