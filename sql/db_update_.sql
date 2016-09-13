-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_5_5_0.sql
-- AppVersion  : 1.5.5.0
-- DbVersion   : 1.7.2
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `discounts` ADD `timezoneStart` TIME NOT NULL AFTER `service`, ADD `timezoneStop` TIME NOT NULL AFTER `timezoneStart`;

INSERT INTO settings (settingId, identifier, value)
 VALUES (NULL, "LICENCE_LAST_VALIDATED", CONCAT((SELECT lastValidated FROM licences WHERE licenceId>1 ORDER BY licenceId DESC LIMIT 1), " 00:00:00") );

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_5_5_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_7_2' WHERE identifier='DATABASE_VERSION';
