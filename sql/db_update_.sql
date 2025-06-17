-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Bikfalvi Tamas
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_3_1_0.sql
-- DbVersion   : 2.3.0.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

DELETE FROM `genders` WHERE genderId=3;
ALTER TABLE `genders` auto_increment=3;
INSERT INTO `genders` (`genderId`, `licenceId`, `genderName` ) VALUES ('3', '0', 'Nem kívánja megadni');

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_3_1_0' WHERE identifier='APPLICATION_VERSION';
-- UPDATE settings SET value='2_3_0_0' WHERE identifier='WEBSYNC_VERSION';
-- UPDATE settings SET value='2_3_0_0' WHERE identifier='VIEWER_VERSION';
-- UPDATE settings SET value='2_3_0_0' WHERE identifier='BACKUP_VERSION';
-- UPDATE settings SET value='2_3_0_0' WHERE identifier='DATABASE_VERSION';
