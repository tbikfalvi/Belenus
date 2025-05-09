-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Bikfalvi Tamas
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_3_0_0.sql
-- DbVersion   : 2.3.0.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `waitlist` ADD `guestId` int(10) unsigned NOT NULL DEFAULT 0 AFTER `licenceId`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_3_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_3_0_0' WHERE identifier='WEBSYNC_VERSION';
UPDATE settings SET value='2_3_0_0' WHERE identifier='VIEWER_VERSION';
UPDATE settings SET value='2_3_0_0' WHERE identifier='BACKUP_VERSION';
UPDATE settings SET value='2_3_0_0' WHERE identifier='DATABASE_VERSION';
