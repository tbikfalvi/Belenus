-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_5_5_4.sql
-- AppVersion  : 1.5.5.4
-- DbVersion   : 1.7.3
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE `ledgertypes` SET `name`='Online bérlet eladás' WHERE `ledgerTypeId` =7;
UPDATE `ledgertypes` SET `name`='Online bérlet feltöltés' WHERE `ledgerTypeId` =8;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_5_5_4' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_7_3' WHERE identifier='DATABASE_VERSION';
