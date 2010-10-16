-- -----------------------------------------------------------------------------------
--
-- Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
--
-- -----------------------------------------------------------------------------------
--
-- Filename : client_update_companyhealthinsurance.sql
-- AppVersion : 1.0
-- FileVersion : 1.0
-- Author : Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- Tablak upgrade-je a kedvezmenyekhez
--
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------
ALTER TABLE `companies`
 ADD `country` VARCHAR( 100 ) NOT NULL AFTER `name` ,
 ADD `region` VARCHAR( 100 ) NOT NULL AFTER `country` ,
 ADD `city` VARCHAR( 100 ) NOT NULL AFTER `region` ,
 ADD `zip` VARCHAR( 10 ) NOT NULL AFTER `city` ,
 ADD `address` VARCHAR( 100 ) NOT NULL AFTER `zip` ,
 ADD `contactPerson` VARCHAR( 100 ) NOT NULL AFTER `address` ,
 ADD `contractId` VARCHAR( 100 ) NOT NULL AFTER `contactPerson` ,
 ADD `validDateFrom` DATE NOT NULL AFTER `contractId` ,
 ADD `validDateTo` DATE NOT NULL AFTER `validDateFrom` ;

ALTER TABLE `healthInsurances`
 ADD `country` VARCHAR( 100 ) NOT NULL AFTER `name` ,
 ADD `region` VARCHAR( 100 ) NOT NULL AFTER `country` ,
 ADD `city` VARCHAR( 100 ) NOT NULL AFTER `region` ,
 ADD `zip` VARCHAR( 10 ) NOT NULL AFTER `city` ,
 ADD `address` VARCHAR( 100 ) NOT NULL AFTER `zip` ,
 ADD `contactPerson` VARCHAR( 100 ) NOT NULL AFTER `address` ,
 ADD `contractId` VARCHAR( 100 ) NOT NULL AFTER `contactPerson` ,
 ADD `validDateFrom` DATE NOT NULL AFTER `contractId` ,
 ADD `validDateTo` DATE NOT NULL AFTER `validDateFrom` ;

ALTER TABLE `patients`
 ADD `discountType` INT NOT NULL AFTER `doctorProposed`;

ALTER TABLE `discounts`
 ADD `doctorId` INT UNSIGNED NOT NULL AFTER `companyId`;
