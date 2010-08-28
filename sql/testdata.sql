-- -----------------------------------------------------------------------------------
--
-- Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
--
-- -----------------------------------------------------------------------------------
--
-- Filename : testdata.sql
-- AppVersion : 1.0
-- FileVersion : 1.0
-- Author : Ballok Peter, Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- Tablak feltoltese teszt adatokkal
-- -----------------------------------------------------------------------------------

USE `belenus`;

INSERT INTO `panels` ( `licenceId`, `panelTypeId`, `title`, `workTime`, `maxWorkTime`, `active`, `archive` ) VALUES
  ( 1, 1, "1-es gép", 0, 0, 1, "ARC" ),
  ( 1, 1, "2-es gép", 0, 0, 1, "ARC" ),
  ( 1, 1, "3-as gép", 0, 0, 1, "ARC" ),
  ( 1, 1, "4-es gép", 0, 0, 1, "ARC" ),
  ( 1, 1, "5-ös gép", 0, 0, 1, "ARC" ),
  ( 1, 1, "6-os gép", 0, 0, 1, "ARC" ),
  ( 1, 1, "7-es gép", 0, 0, 1, "ARC" ),
  ( 1, 1, "8-as gép", 0, 0, 1, "ARC" );

INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `vatpercent`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `active`, `archive`) VALUES
(2, 1, '10 alkalmas felnőtt bérlet', 60000, 0, 13, '2010-08-26', '2010-08-26', 365, 20, 1, 'NEW'),
(3, 1, '10 alkalmas nyugdíjas bérlet', 37000, 0, 10, '2010-08-26', '2010-08-26', 365, 20, 1, 'NEW');

INSERT INTO `patients` (`patientId`, `licenceId`, `patientOriginId`, `reasonToVisitId`, `illnessGroupId`, `healthInsuranceId`, `companyId`, `doctorId`, `name`, `gender`, `dateBirth`, `uniqueId`, `email`, `phone`, `weight`, `height`, `medicineCurrent`, `medicineAllergy`, `regularCustomer`, `employee`, `service`, `healthInsurance`, `company`, `doctorProposed`, `comment`, `active`, `archive`) VALUES
 (NULL, '1', '0', '0', '0', '0', '0', '0', 'Teszt Vásárló', '1', '1988-07-11', '123 456 789', NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '0', '0', '0', '0', NULL, '1', 'ARC');

INSERT INTO `patientCards` (`patientCardId`, `licenceId`, `patientCardTypeId`, `patientId`, `barcode`, `comment`, `units`, `timeLeft`, `validDate`, `pincode`, `active`, `archive`) VALUES
(1, 1, 2, 1, '11111', '', 13, 15600, '2011-08-26', '', 1, 'NEW');

INSERT INTO `panelUses` (`licenceId`, `panelId`, `name`, `useTime`, `usePrice`, `archive`) VALUES
  (0, 1, 'Csecsemő A', 5, 1600, 'ARC'),
  (0, 1, 'Csecsemő B', 5, 2400, 'ARC'),
  (0, 1, 'Gyermek B', 10, 3200, 'ARC'),
  (0, 1, 'Gyermek A', 15, 4000, 'ARC'),
  (0, 1, 'Nyugdíjas', 20, 3840, 'ARC'),
  (0, 1, 'Felnőtt', 20, 4800, 'ARC');

INSERT INTO `address` (`addressId`, `licenceId`, `patientId`, `publicPlaceId`, `name`, `country`, `region`, `city`, `zip`, `street`, `streetNumber`, `floor`, `door`, `primaryAddress`, `active`, `archive`) VALUES
 (1, 1, 1, 4, 'Teszt Vásárló', 'Magyarország', 'Pest', 'Budapest', '1234', 'Próba', '1', NULL, NULL, 1, 1, 'ARC');
