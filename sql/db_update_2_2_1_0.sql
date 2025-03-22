-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_2_1_0.sql
-- DbVersion   : 2.2.0.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------


-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_2_1_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_2_0_0' WHERE identifier='WEBSYNC_VERSION';
UPDATE settings SET value='2_2_0_0' WHERE identifier='VIEWER_VERSION';
UPDATE settings SET value='2_2_0_0' WHERE identifier='DATABASE_VERSION';
