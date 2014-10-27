-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_4_5.sql
-- AppVersion  : 1.4.5
-- DbVersion   : 1.6.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcardunits` ADD `patientCardTypeId` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `patientCardId`;
ALTER TABLE `patientcardunits` ADD INDEX `patientCardTypeId` ( `patientCardTypeId` );
ALTER TABLE `patientcardunits` ADD FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE;

-- -----------------------------------------------------------------------------------

CREATE TABLE `waitlist` (
  `waitlistId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  `ledgerId`                int(10) unsigned        NOT NULL,
  `panelTypeId`             int(10) unsigned        NOT NULL,
  `payType`                 int(10) unsigned        NOT NULL,
  `barcode`                 varchar(20)             NOT NULL,
  `unitIds`                 varchar(100)            NOT NULL,
  `lengthCash`              decimal(10,0)           NOT NULL,
  `lengthCard`              decimal(10,0)           NOT NULL,
  `useTime`                 int(11)                 NOT NULL,
  `usePrice`                decimal(10,0)           NOT NULL,
  `comment`                 text                    DEFAULT NULL,
  PRIMARY KEY (`waitlistId`,`licenceId`),
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`ledgerId`) REFERENCES `ledger` (`ledgerId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelTypeId`) REFERENCES `panelTypes` (`panelTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_4_5' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_6_0' WHERE identifier='DATABASE_VERSION';
