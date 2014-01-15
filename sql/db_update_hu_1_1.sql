-- -----------------------------------------------------------------------------------
--
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2013
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : db_update_hu_1_1.sql
-- AppVersion  : 1.0.4
-- DbVersion   : 1.1
-- FileVersion : 1.0
-- Author      : Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- 1.1 adatbazis verziot frissito file
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='1_0_5' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_2' WHERE identifier='DATABASE_VERSION';

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazasban mukodtetett panelcsoportokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelgroups` (
  `panelgroupId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(20)             NOT NULL,
  `description`             text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelgroupId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------

ALTER TABLE  `panels` ADD  `panelgroupId` INT( 10 ) NOT NULL AFTER `panelTypeId`;

ALTER TABLE `panels`
  ADD FOREIGN KEY (`panelgroupId`) REFERENCES `panelgroups` (`panelgroupId`) ON UPDATE CASCADE;

-- -----------------------------------------------------------------------------------
