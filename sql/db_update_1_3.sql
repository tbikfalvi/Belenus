-- -----------------------------------------------------------------------------------
--
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : db_update_1_3.sql
-- AppVersion  : 1.3.0
-- DbVersion   : 1.4
-- FileVersion : 1.0
-- Author      : Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- 1.2 adatbazis verziot frissito file
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcardunits` ADD `gibbigId` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `panelId`;

-- -----------------------------------------------------------------------------------

CREATE TABLE `gibbigMessageTypes` (
  `gibbigMessageTypeId`     int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `gibbigMessageType`       varchar(20)             NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`gibbigMessageTypeId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `gibbigMessages` (
  `gibbigMessageId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `gibbigMessageTypeId`     int(10) unsigned        NOT NULL,
  `gibbigMessage`           text                    NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`gibbigMessageId`,`licenceId`),
  FOREIGN KEY (`gibbigMessageTypeId`) REFERENCES `gibbigMessageTypes` (`gibbigMessageTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `gibbigmessagetypes` ( `licenceId`, `gibbigMessageType`, `active`, `archive` ) VALUES
 ( '0', 'PCT_CREATE', '1', 'ARC' ),
 ( '0', 'PCT_MODIFY', '1', 'ARC' ),
 ( '0', 'PCT_DELETE', '1', 'ARC' ),
 ( '0', 'PC_SELL', '1', 'ARC' ),
 ( '0', 'PC_REFILL', '1', 'ARC' ),
 ( '0', 'PC_USE', '1', 'ARC' ),
 ( '0', 'PC_DELETE', '1', 'ARC' );

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_3_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_4' WHERE identifier='DATABASE_VERSION';
