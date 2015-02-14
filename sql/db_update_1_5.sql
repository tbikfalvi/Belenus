-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_5.sql
-- AppVersion  : 1.5.0
-- DbVersion   : 1.7.0
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

DROP TABLE gibbigmessages;
DROP TABLE gibbigmessagetypes;
ALTER TABLE `patientcardunits` DROP `gibbigId`

-- -----------------------------------------------------------------------------------

CREATE TABLE `httppatientcardinfo` (
  `httpPatientcardInfoId`   int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `barcode`                 varchar(20)             NOT NULL,
  `patientcardInfoText`     text                    NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,  
  PRIMARY KEY (`httpPatientcardInfoId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_5_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_7_0' WHERE identifier='DATABASE_VERSION';
