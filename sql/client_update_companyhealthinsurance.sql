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

