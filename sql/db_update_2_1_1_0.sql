﻿-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_2_1_1_0.sql
-- AppVersion  : 2.1.1.0
-- DbVersion   : 2.1.1.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcardtypes` ADD `panelGroupID` int(10) unsigned NOT NULL DEFAULT 0 AFTER `licenceId`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='2_1_1_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='2_1_1_0' WHERE identifier='DATABASE_VERSION';
