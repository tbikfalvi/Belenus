-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Bikfalvi Tamas
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_3_2_1.sql
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `report_cardhistory` ADD `barcode` varchar(20) NOT NULL AFTER `userName`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_3_2_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_3_2_0' WHERE identifier='WEBSYNC_VERSION';
UPDATE settings SET value='2_3_2_1' WHERE identifier='VIEWER_VERSION';
UPDATE settings SET value='2_3_2_0' WHERE identifier='BACKUP_VERSION';
UPDATE settings SET value='2_3_2_0' WHERE identifier='DATABASE_VERSION';
