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

ALTER TABLE `licences` ADD `type` VARCHAR( 20 ) NOT NULL DEFAULT 'UNREGISTERED' AFTER `lastValidated`;

UPDATE `licences` SET `type` = 'DEMO' WHERE `licenceId` < 2;

ALTER TABLE `patients` ADD `isCardy` TINYINT NOT NULL DEFAULT '0' AFTER `uniqueId`;

INSERT INTO `settings` ( `settingId`, `identifier`, `value` ) VALUES ( NULL , 'LICENCE_CHECK', '56' );
INSERT INTO `settings` ( `settingId`, `identifier`, `value` ) VALUES ( NULL , 'LICENCE_CHECK_COUNTER', '56' );
INSERT INTO `settings` ( `settingId`, `identifier`, `value` ) VALUES ( NULL , 'LICENCE_WORKTIME', '336' );
INSERT INTO `settings` ( `settingId`, `identifier`, `value` ) VALUES ( NULL , 'LICENCE_WORKTIME_COUNTER', '336' );

INSERT INTO `settings` ( `settingId`, `identifier`, `value` ) VALUES ( NULL , 'AUTOMAIL_OnUnitChange', 0 );
INSERT INTO `settings` ( `settingId`, `identifier`, `value` ) VALUES ( NULL , 'AUTOMAIL_OnCardyGo', 0 );

ALTER TABLE `httpsendmail` ADD `mailDestination` INT UNSIGNED NOT NULL AFTER `mailTypeId`;

ALTER TABLE `panels` ADD `imagePathFileName` VARCHAR( 500 ) NOT NULL AFTER `title`

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_0_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_0_0_0' WHERE identifier='DATABASE_VERSION';
