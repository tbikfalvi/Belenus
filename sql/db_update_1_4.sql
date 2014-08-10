-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_4.sql
-- AppVersion  : 1.4.0
-- DbVersion   : 1.5
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

CREATE TABLE `advertisements` (
  `advertisementId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `caption`                 varchar(100)            NOT NULL,
  `backgroundColor`         varchar(10)             NOT NULL,
  `path`                    text                    NOT NULL,
  `fileNames`               text                    NOT NULL,
  `timer`                   int(10) unsigned        NOT NULL DEFAULT 0,
  `transparent`             tinyint(1)              DEFAULT 0,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`advertisementId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_4_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_5' WHERE identifier='DATABASE_VERSION';
