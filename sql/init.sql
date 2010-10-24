-- -----------------------------------------------------------------------------------
--
-- Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
--
-- -----------------------------------------------------------------------------------
--
-- Filename : init.sql
-- AppVersion : 1.0
-- FileVersion : 1.0
-- Author : Ballok Peter, Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- Tablak feltoltese default adatokkal
--
-- Ezeket az INSERT-eket a telepito futtatja le azutan, hogy a szerver programtol
-- megkapja a kliens serial-hoz tartozo licenceId erteket
--
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

INSERT INTO `licences` (`licenceId`, `serial`, `country`, `region`, `city`, `zip`, `address`, `studio`, `contact`, `lastValidated`, `active`, `archive`) VALUES
  ( 0, 'BLNS_SERIAL_GLOBAL', 'Magyarország', 'Pest megye', 'Budapest', '1139', 'ROZSNYAI U. 11', 'Polarium Kft.', NULL, '0000-00-00', 1, 'ARC');
INSERT INTO `licences` (`licenceId`, `serial`, `country`, `region`, `city`, `zip`, `address`, `studio`, `contact`, `lastValidated`, `active`, `archive`) VALUES
  ( 1, 'BLNS_SERIAL_DEMO', 'Magyarország', 'Pest megye', 'Budapest', '1139', 'ROZSNYAI U. 11', 'Polarium Kft.', NULL, '0000-00-00', 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `accgroup`, `active`, `comment`) VALUES
  ( 1, 'admin', 'Adminisztrátor', 'd033e22ae348aeb5660fc2140aec35850c4da997', 2, 1, 'Gyógycentrum alapértelmezett felhasználója rendszergazdai jogosultságokkal.' );
INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `accgroup`, `active`, `comment`) VALUES
  ( 1, 'kezelo', 'Kezelõ', 'e47c3168137789e3e0df75d23452cabcc9a890f5', 1, 1, 'Gyógycentrum alapértelmezett felhasználója alap felhasználói jogosultságokkal' );

-- -----------------------------------------------------------------------------------

INSERT INTO `activateCommand` (`activateCommandId`, `name`) VALUES
 (0, 'Alap állapot'),
 (1, 'Elõkészítés'),
 (3, 'Kezelés'),
 (4, 'Utóhûtés'),
 (9, 'Várakozás');

-- -----------------------------------------------------------------------------------

INSERT INTO  `patientOrigin` (`patientOriginId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '', 0, 'ARC');
UPDATE `patientOrigin` SET `patientOriginId`=0 WHERE `patientOriginId`=1;
ALTER TABLE `patientOrigin` auto_increment=1;

INSERT INTO `patientOrigin` (`patientOriginId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, 0, 'Ismerõs/barát ajánlotta', 1, 'ARC'),
 (2, 0, 'Szórólap', 1, 'ARC'),
 (3, 0, 'Internet', 1, 'ARC'),
 (4, 0, 'TV', 1, 'ARC'),
 (5, 0, 'Újság', 1, 'ARC'),
 (6, 0, 'Egyéb', 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `reasonToVisit` (`reasonToVisitId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '', 0, 'ARC');
UPDATE `reasonToVisit` SET `reasonToVisitId`=0 WHERE `reasonToVisitId`=1;
ALTER TABLE `reasonToVisit` auto_increment=1;

INSERT INTO `reasonToVisit` (`reasonToVisitId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, 0, 'Gyógyulás betegségbõl', 1, 'ARC'),
 (2, 0, 'Immunerõsítés / regenerálódás', 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `illnessGroups` (`illnessGroupId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '', 0, 'ARC');
UPDATE `illnessGroups` SET `illnessGroupId`=0 WHERE `illnessGroupId`=1;
ALTER TABLE `illnessGroups` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO  `publicPlaces` (`publicPlaceId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '', 0, 'ARC');
UPDATE `publicPlaces` SET `publicPlaceId`=0 WHERE `publicPlaceId`=1;
ALTER TABLE `publicPlaces` auto_increment=1;
INSERT INTO `publicPlaces` (`publicPlaceId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, 0, 'út', 1, 'ARC'),
 (2, 0, 'utca', 1, 'ARC'),
 (3, 0, 'körút', 1, 'ARC'),
 (4, 0, 'köz', 1, 'ARC'),
 (5, 0, 'tér', 1, 'ARC'),
 (6, 0, 'dûlõ', 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO  `healthInsurances` (`healthInsuranceId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '', 0, 'ARC');
UPDATE `healthInsurances` SET `healthInsuranceId`=0 WHERE `healthInsuranceId`=1;
ALTER TABLE `healthInsurances` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO  `companies` (`companyId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '', 0, 'ARC');
UPDATE `companies` SET `companyId`=0 WHERE `companyId`=1;
ALTER TABLE `companies` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO  `doctorTypes` (`doctorTypeId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '', 0, 'ARC');
UPDATE `doctorTypes` SET `doctorTypeId`=0 WHERE `doctorTypeId`=1;
ALTER TABLE `doctorTypes` auto_increment=1;

INSERT INTO `belenus`.`doctorTypes` (`doctorTypeId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, '0', 'Orvos', '1', 'ARC'),
 (2, '0', 'Gyógyszerész', '1', 'ARC'),
 (3, '0', 'Gyógytornász', '1', 'ARC'),
 (4, '0', 'Védõnõ', '1', 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO  `doctors` (`doctorId`, `licenceId`, `doctorTypeId`, `name`, `active`, `archive`) VALUES
 (0, 0, 0, '', 0, 'ARC');
UPDATE `doctors` SET `doctorId`=0 WHERE `doctorId`=1;
ALTER TABLE `doctors` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `patients` (`patientId`, `licenceId`, `patientOriginId`, `reasonToVisitId`, `illnessGroupId`, `healthInsuranceId`, `companyId`, `doctorId`, `name`, `gender`, `dateBirth`, `uniqueId`, `email`, `phone`, `weight`, `height`, `illnesses`, `symptoms`, `medicineCurrent`, `medicineAllergy`, `regularCustomer`, `employee`, `service`, `healthInsurance`, `company`, `doctorProposed`, `comment`, `active`, `archive`) VALUES
 ('0', '0', '0', '0', '0', '0', '0', '0', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '0', '0', '0', '0', NULL, '0', 'ARC');
UPDATE `patients` SET `patientId`=0 WHERE `patientId`=1;
ALTER TABLE `patients` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `attendance` (`attendanceId`, `licenceId`, `patientId`, `date`, `length`, `bloodPressureStart`, `pulseStart`, `bloodPressureStop`, `pulseStop`, `comment`, `active`, `archive`) VALUES
 (0, 0, 0, '0000-00-00', NULL, NULL, NULL, NULL, NULL, NULL, 0, 'ARC');
UPDATE `attendance` SET `attendanceId`=0 WHERE `attendanceId`=1;
ALTER TABLE `attendance` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `vatpercent`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `active`, `archive`) VALUES
 (0, 0, '', 0, 0, 0, '2010-01-01', '2010-01-01', 0, 0, 0, 'ARC');
UPDATE `patientCardTypes` SET `patientCardTypeId`=0 WHERE `patientCardTypeId`=1;
ALTER TABLE `patientCardTypes` auto_increment=1;

INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `vatpercent`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `active`, `archive`) VALUES
 (1, 0, 'Szervíz kártyák', 0, 0, 999, '2010-01-01', '2100-12-31', 0, 1, 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `patientCards` (`patientCardId`, `licenceId`, `patientCardTypeId`, `patientId`, `barcode`, `comment`, `units`, `timeLeft`, `validDateFrom`, `validDateTo`, `pincode`, `active`, `archive`) VALUES
 (0, 0, 0, 0, '', NULL, 0, 0, '0000-00-00', '0000-00-00', NULL, 0, 'ARC');
UPDATE `patientCards` SET `patientCardId`=0 WHERE `patientCardId`=1;
ALTER TABLE `patientCards` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `panelTypes` ( `panelTypeId`, `licenceId`, `name`, `active`, `archive` ) VALUES
  ( 0, 0, "", 0, "ARC" );
UPDATE `panelTypes` SET `panelTypeId`='0' WHERE `panelTypeId`=1;
ALTER TABLE `panelTypes` auto_increment=1;

INSERT INTO `panelTypes` ( `panelTypeId`, `licenceId`, `name`, `active`, `archive` ) VALUES
 ( 1, 0, "Sensolite gyógyterápiás gép", 1, "ARC" );

-- -----------------------------------------------------------------------------------

INSERT INTO `panelStatuses` ( `licenceId`, `panelTypeId`, `seqNumber`, `name`, `length`, `activateCmd`, `active`, `archive` ) VALUES
 ( 0, 1, 1, "Alap állapot", 0, 0, 1, "ARC" ),
 ( 0, 1, 2, "Elõkészület/Vetkõzés", 60, 1, 1, "ARC" ),
 ( 0, 1, 3, "Kezelés", 0, 3, 1, "ARC" ),
 ( 0, 1, 4, "Utóhûtés", 60, 4, 1, "ARC" );

-- -----------------------------------------------------------------------------------

INSERT INTO `panels` ( `panelId`, `licenceId`, `panelTypeId`, `title`, `workTime`, `maxWorkTime`, `active`, `archive` ) VALUES
  ( 0, 0, 0, "", 0, 0, 0, "ARC" );
UPDATE `panels` SET `panelId`='0' WHERE `panelId`=1;
ALTER TABLE `panels` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `panelUses` (`licenceId`, `panelId`, `name`, `useTime`, `usePrice`, `active`, `archive`) VALUES
  ('0', '0', '', '0', '0', '0', 'ARC');
UPDATE `panelUses` SET `panelUseId`='0' WHERE `panelUseId`=1;
ALTER TABLE `panelUses` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `denominations` (`denominationId`, `licenceId`, `denomination`, `comment`, `active`, `archive`) VALUES
 (1, 0, 5, "", 1, 'ARC'),
 (2, 0, 10, "", 1, 'ARC'),
 (3, 0, 20, "", 1, 'ARC'),
 (4, 0, 50, "", 1, 'ARC'),
 (5, 0, 100, "", 1, 'ARC'),
 (6, 0, 200, "", 1, 'ARC'),
 (7, 0, 500, "", 1, 'ARC'),
 (8, 0, 1000, "", 1, 'ARC'),
 (9, 0, 2000, "", 1, 'ARC'),
 (10, 0, 5000, "", 1, 'ARC'),
 (11, 0, 10000, "", 1, 'ARC'),
 (12, 0, 20000, "", 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `ledgerDevice` (`ledgerDeviceId`, `licenceId`, `userId`, `panelId`, `patientId`, `units`, `cash`, `timeReal`, `timeLeft`, `timeCard`, `timeCash`, `ledgerTime`, `comment`, `active`, `archive`) VALUES
 (0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, '2010-10-23 11:45:07', NULL, 0, 'ARC');
UPDATE `ledgerDevice` SET `ledgerDeviceId`=0 WHERE `ledgerDeviceId`=1;
ALTER TABLE `ledgerDevice` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `paymentMethods` (`paymentMethodId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, 0, 'Készpénzes fizetés', 1, 'ARC'),
 (2, 0, 'Fizetés utalvánnyal', 1, 'ARC'),
 (3, 0, 'Fizetés bankkártyával', 1, 'ARC');
-- -----------------------------------------------------------------------------------

INSERT INTO `ledgerTypes` (`ledgerTypeId` ,`licenceId` ,`name` ,`active` ,`archive`) VALUES
 (1 , '0', 'Géphasználat', '1', 'ARC'),
 (2 , '0', 'Bérlet eladás', '1', 'ARC'),
 (3 , '0', 'Bérlet feltöltés', '1', 'ARC'),
 (4 , '0', 'Termék eladás', '1', 'ARC'),
 (5 , '0', 'Egyéb', '1', 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `productTypes` (`productTypeId`, `licenceId`, `name`, `active`, `archive`) VALUES
 ('0', '0', '', '0', 'ARC');
UPDATE `productTypes` SET `productTypeId`=0 WHERE `productTypeId`=1;
ALTER TABLE `productTypes` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `products` (`productId`, `licenceId`, `productTypeId`, `name`, `netPrice`, `vatpercent`, `active`, `archive`) VALUES
 ('0', '0', '0', '', '0', '0', '0', 'ARC');
UPDATE `products` SET `productId`='0' WHERE `productId`=1;
ALTER TABLE `products` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `discounts` (`discountId`, `licenceId`, `healthInsuranceId`, `companyId`, `regularCustomer`, `employee`, `service`, `name`, `discountValue`, `discountPercent`, `active`, `archive`) VALUES
 ('0', '0', NULL, NULL, '0', '0', '0', '', '', '', 0, "ARC" );
UPDATE `discounts` SET `discountId`='0' WHERE `discountId`=1;
ALTER TABLE `discounts` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `address` (`addressId`, `licenceId`, `patientId`, `publicPlaceId`, `name`, `country`, `region`, `city`, `zip`, `street`, `streetNumber`, `floor`, `door`, `primaryAddress`, `active`, `archive`) VALUES
 ('0', '0', '0', '0', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', 'ARC');
UPDATE `address` SET `addressId`='0' WHERE `addressId`=1;
ALTER TABLE `address` auto_increment=1;
