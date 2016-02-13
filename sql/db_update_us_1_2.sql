-- -----------------------------------------------------------------------------------
--
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : db_update_us_1_2.sql
-- AppVersion  : 1.2.0
-- DbVersion   : 1.3
-- FileVersion : 1.0
-- Author      : Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- 1.2 adatbazis verziot frissito file
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `patientcardunits`
 ADD `prepared` 	tinyint(1) 			NOT NULL DEFAULT '0'	AFTER `dateTimeUsed`;

-- -----------------------------------------------------------------------------------

CREATE TABLE `skinTypes` (
  `skinTypeId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `skinTypeName`            varchar(50)             NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`skinTypeId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `skinTypes` (`skinTypeId`, `licenceId`, `skinTypeName`, `active`, `archive` ) VALUES
 ('0', '0', 'Not defined', 1, 'ARC');
UPDATE `skinTypes` SET `skinTypeId`=0 WHERE `skinTypeId`=1;
ALTER TABLE `skinTypes` auto_increment=1;

-- -----------------------------------------------------------------------------------

ALTER TABLE  `patients` 
 ADD `membership`	varchar( 30 ) 		NOT NULL 				AFTER `discountType` ,
 ADD `dateBirth` 	date 				NOT NULL 				AFTER `membership` ,
 ADD `address` 		varchar( 200 ) 		NOT NULL 				AFTER `dateBirth` ,
 ADD `skinTypeId` 	int(10) unsigned	DEFAULT 0				AFTER `address` ,
 ADD `mobile` 		varchar( 50 ) 		NOT NULL 				AFTER `skinTypeId`;

ALTER TABLE `patients` ADD INDEX `skinTypeId` ( `skinTypeId` );
ALTER TABLE `patients` ADD FOREIGN KEY (`skinTypeId`) REFERENCES `skinTypes` (`skinTypeId`) ON UPDATE CASCADE;
 
-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_2_0' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_3' WHERE identifier='DATABASE_VERSION';
