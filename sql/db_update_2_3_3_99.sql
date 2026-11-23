-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Bikfalvi Tamas
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_3_3_99.sql
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

INSERT INTO `settings` ( `settingId`, `identifier`, `value` ) VALUES ( NULL , 'LICENCE_CHECK_ENABLED', '0' );

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_3_3_99' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_3_3_0' WHERE identifier='WEBSYNC_VERSION';
UPDATE settings SET value='2_3_3_0' WHERE identifier='VIEWER_VERSION';
UPDATE settings SET value='2_3_3_0' WHERE identifier='BACKUP_VERSION';
UPDATE settings SET value='2_3_3_99' WHERE identifier='DATABASE_VERSION';
