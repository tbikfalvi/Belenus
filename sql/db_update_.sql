-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_6_1_0.sql
-- AppVersion  : 1.6.1.0
-- DbVersion   : 1.7.6
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

-- -----------------------------------------------------------------------------------
-- Email cimlista
-- -----------------------------------------------------------------------------------
CREATE TABLE `distlist` (
  `distlistId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `description`             varchar(500)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`distlistId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Pacienseket es email levelezo listakat osszekoto tabla, levelezo listak kezelesehez
-- -----------------------------------------------------------------------------------
CREATE TABLE `connectPatientWithDistList` (
  `distlistId`              int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `licenceId`               int(10) unsigned        NOT NULL,
  PRIMARY KEY (`distlistId`,`patientId`,`licenceId`),
  FOREIGN KEY (`distlistId`) REFERENCES `distlist` (`distlistId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Email kuldes ideiglenes tablaja. Minden sor egy kikuldendo level.
-- -----------------------------------------------------------------------------------
CREATE TABLE `httpsendmail` (
  `httpSendMailId`          int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `mailTypeId`              int(10) unsigned        NOT NULL,
  `dateOfSending`           date                    NOT NULL,
  `recipients`              text                    NOT NULL,
  `subject`                 varchar(500)            NOT NULL,
  `mailbody`                text                    NOT NULL,
  `var_name`                varchar(500)            NOT NULL,
  `var_barcode`             varchar(20)             NOT NULL,
  `var_cardinfo`            text                    NOT NULL,
  `var_unitcount`           varchar(10)             NOT NULL,
  `var_datetime`            varchar(50)             NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,  
  PRIMARY KEY (`httpSendMailId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Varakozok tablaja kiegeszitve datummal, hogy lehessen tudni, mikor kerult be a rekord.
-- -----------------------------------------------------------------------------------
ALTER TABLE `waitlist` ADD `modified` DATETIME NOT NULL;

-- -----------------------------------------------------------------------------------
-- Liszensz tabla kiegeszitve kiegeszito adatokkal.
-- -----------------------------------------------------------------------------------
ALTER TABLE  `licences`
 ADD  `Act` VARCHAR( 10 ) NOT NULL AFTER  `lastValidated`,
 ADD  `Cod` VARCHAR( 10 ) NOT NULL AFTER  `Act`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_6_1_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_7_6' WHERE identifier='DATABASE_VERSION';
