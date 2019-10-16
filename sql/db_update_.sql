-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_8_0_1.sql
-- AppVersion  : 1.8.0.1
-- DbVersion   : 1.8.0.1
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcards` ADD `rfId` varchar(30) NOT NULL AFTER `barcode`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_8_0_1' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_8_0_1' WHERE identifier='DATABASE_VERSION';
