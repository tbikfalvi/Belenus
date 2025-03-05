-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_1_3_0.sql
-- AppVersion  : 2.1.3.0
-- DbVersion   : 2.1.3.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, 'WEBSYNC_VERSION', '2_1_3_0');
INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, 'VIEWER_VERSION', '2_1_3_0');

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_1_3_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_1_3_0' WHERE identifier='DATABASE_VERSION';
