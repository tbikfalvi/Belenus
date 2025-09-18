-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Bikfalvi Tamas
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_3_2_0.sql
-- DbVersion   : 2.3.2.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

INSERT INTO `ledgertypes` (`ledgerTypeId`, `licenceId`, `name`, `active`, `archive`) VALUES (NULL, '0', 'Bérlet egység növelés', '1', 'ARC');
INSERT INTO `ledgertypes` (`ledgerTypeId`, `licenceId`, `name`, `active`, `archive`) VALUES (NULL, '0', 'Bérlet egység csökkentés', '1', 'ARC');

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_3_2_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_3_2_0' WHERE identifier='WEBSYNC_VERSION';
UPDATE settings SET value='2_3_2_0' WHERE identifier='VIEWER_VERSION';
UPDATE settings SET value='2_3_2_0' WHERE identifier='BACKUP_VERSION';
UPDATE settings SET value='2_3_2_0' WHERE identifier='DATABASE_VERSION';
