-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_2_0_0.sql
-- DbVersion   : 2.2.0.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patients` ADD `isNewsletter` TINYINT NOT NULL DEFAULT '0' AFTER `isCardy`;
ALTER TABLE `patients` ADD `isCardMail` TINYINT NOT NULL DEFAULT '0' AFTER `isNewsletter`;

ALTER TABLE `patienthistory` ADD `patientHistoryTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP;

INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, 'BACKUP_VERSION', '2_2_0_0');
INSERT INTO `genders` (`genderId`, `licenceId`, `genderName` ) VALUES ('3', '0', 'Nem kívánja megadni');

UPDATE patients SET isNewsletter=1, isCardMail=1 WHERE email<>"";

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_2_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_2_0_0' WHERE identifier='WEBSYNC_VERSION';
UPDATE settings SET value='2_2_0_0' WHERE identifier='VIEWER_VERSION';
UPDATE settings SET value='2_2_0_0' WHERE identifier='DATABASE_VERSION';
