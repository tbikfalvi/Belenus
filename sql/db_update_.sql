-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_3_0_0_0.sql
-- DbVersion   : 3.0.0.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------


-- -----------------------------------------------------------------------------------

UPDATE settings SET value='3_0_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='3_0_0_0' WHERE identifier='BACKUP_VERSION';
UPDATE settings SET value='3_0_0_0' WHERE identifier='WEBSYNC_VERSION';
UPDATE settings SET value='3_0_0_0' WHERE identifier='VIEWER_VERSION';

UPDATE settings SET value='3_0_0_0' WHERE identifier='DATABASE_VERSION';
