-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_6_0_0.sql
-- AppVersion  : 1.6.0.0
-- DbVersion   : 1.7.5
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcards` ADD `rfId` varchar(30) NOT NULL AFTER `barcode`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_6_0_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_7_5' WHERE identifier='DATABASE_VERSION';
