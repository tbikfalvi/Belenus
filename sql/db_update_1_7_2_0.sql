﻿-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_7_2_0.sql
-- AppVersion  : 1.7.2.0
-- DbVersion   : 1.7.8
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------


-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_7_2_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_7_8' WHERE identifier='DATABASE_VERSION';
