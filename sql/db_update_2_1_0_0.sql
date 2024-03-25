-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_1_0_0.sql
-- AppVersion  : 2.1.0.0
-- DbVersion   : 2.1.0.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcardunits` ADD `panelGroupID` int(10) unsigned NOT NULL DEFAULT 0 AFTER `patientCardTypeId`;

UPDATE `panelgroups` SET `name` = '* Osszes *' WHERE `panelGroupId` = 0;

ALTER TABLE `panels` ADD `imagePathFileName` VARCHAR( 500 ) NOT NULL AFTER `title`

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_1_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_1_0_0' WHERE identifier='DATABASE_VERSION';
