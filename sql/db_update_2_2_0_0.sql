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

ALTER TABLE `paneluses` ADD `panelIds` varchar(100) DEFAULT "" AFTER `licenceId`;
DELETE FROM `paneluses` WHERE active = 0;
UPDATE `paneluses` SET panelIds = panelId;

ALTER TABLE `patienthistorytype` DROP PRIMARY KEY, ADD PRIMARY KEY(`patientHistoryTypeId`,`licenceId`);
ALTER TABLE `productactiontype` DROP PRIMARY KEY, ADD PRIMARY KEY(`productActionTypeId`,`licenceId`);
ALTER TABLE `producthistory` DROP PRIMARY KEY, ADD PRIMARY KEY(`productHistoryId`,`licenceId`);

INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, 'BACKUP_VERSION', '2_2_0_0');
INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, 'DB_AllowDeleteObsoleteUnits', 0);
INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, 'DB_ObsolateUnitsDays', 30 );
INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, 'FILE_AllowDeleteLogFiles', 0);
INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, 'FILE_DeleteLogFilesMonts', 3 );

INSERT INTO `genders` (`genderId`, `licenceId`, `genderName` ) VALUES ('3', '0', 'Prefer not to say');

UPDATE patients SET isNewsletter=1, isCardMail=1 WHERE email<>"";

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_2_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_2_0_0' WHERE identifier='WEBSYNC_VERSION';
UPDATE settings SET value='2_2_0_0' WHERE identifier='VIEWER_VERSION';
UPDATE settings SET value='2_2_0_0' WHERE identifier='DATABASE_VERSION';
