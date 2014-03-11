-- -----------------------------------------------------------------------------------
--
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2013
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : db_update_hu_1_1.sql
-- AppVersion  : 1.1.0
-- DbVersion   : 1.2
-- FileVersion : 1.0
-- Author      : Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- 1.1 adatbazis verziot frissito file
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='1_1_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_2' WHERE identifier='DATABASE_VERSION';

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazasban mukodtetett panelcsoportokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelgroups` (
  `panelGroupId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `description`             text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelGroupId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `panelgroups` (`panelGroupId`, `licenceId`, `name`, `description`, `modified`, `active`, `archive`) VALUES
 (0, 0, '<Nincs csoporthoz rendelve>', NULL, '0000-00-00 00:00:00', 1, 'ARC');
UPDATE `panelgroups` SET `panelGroupId`='0' WHERE `panelGroupId`=1;
ALTER TABLE `panelgroups` auto_increment=1;

-- -----------------------------------------------------------------------------------

ALTER TABLE `panels` ADD `panelGroupId` INT( 10 ) unsigned NOT NULL AFTER `panelTypeId`;
ALTER TABLE `panels` ADD INDEX `panelGroupId` ( `panelGroupId` );
ALTER TABLE `panels` ADD FOREIGN KEY (`panelGroupId`) REFERENCES `panelGroups` (`panelGroupId`) ON UPDATE CASCADE;

-- -----------------------------------------------------------------------------------

ALTER TABLE `panelstatuses` ADD `skipLevel` INT( 10 ) NOT NULL DEFAULT '4' AFTER
            `activateCmd` , ADD `allowedToSkip` TINYINT NOT NULL DEFAULT '0' AFTER `skipLevel`;

