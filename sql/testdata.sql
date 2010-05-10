-- -----------------------------------------------------------------------------------
--
-- Belenus Server alkalmazas ? Pagony Multimedia Studio Bt - 2010
--
-- -----------------------------------------------------------------------------------
--
-- Filename : testdata.sql
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

INSERT INTO `licences` (`licenceId`, `serial`, `country`, `region`, `city`, `zip`, `address`, `studio`, `contact`, `active`) VALUES
  ( 0, 'BLNS_SERIAL_GLOBAL', 'Magyarország', 'Pest megye', 'Budapest', '1139', 'ROZSNYAI U. 11', 'Polarium Kft.', NULL, 1);
INSERT INTO `licences` (`licenceId`, `serial`, `country`, `region`, `city`, `zip`, `address`, `studio`, `contact`, `active`) VALUES
  ( 1, 'BLNS_SERIAL_DEMO', 'Magyarország', 'Pest megye', 'Budapest', '1139', 'ROZSNYAI U. 11', 'Polarium Kft.', NULL, 1);

INSERT INTO `users` (`name`, `licenceId`, `realName`, `password`, `groups`) VALUES
  ( 'admin', 1, 'Administrator', 'd033e22ae348aeb5660fc2140aec35850c4da997', 'admin,user');

INSERT INTO `panelTypes` ( `licenceId`, `name` ) VALUES
  ( 1, "Panel Type 1" );

INSERT INTO `panels` ( `licenceId`, `panelTypeId`, `title` ) VALUES
  ( 1, 1, "Panel 1" ),
  ( 1, 1, "Panel 2" ),
  ( 1, 1, "Panel 3" ),
  ( 1, 1, "Panel 4" ),
  ( 1, 1, "Panel 5" ),
  ( 1, 1, "Panel 6" ),
  ( 1, 1, "Panel 7" ),
  ( 1, 1, "Panel 8" );

INSERT INTO  `patientOrigin` (`patientOriginId`, `licenceId`, `name`, `archive`) VALUES
 (0, 0, '<Nincs megadva>', 'ARC');
UPDATE `patientOrigin` SET `patientOriginId`=0 WHERE `patientOriginId`=1;
ALTER TABLE `patientOrigin` auto_increment=1;

INSERT INTO `reasonToVisit` (`reasonToVisitId`, `licenceId`, `name`, `archive`) VALUES
 (0, 0, '<Nincs megadva>', 'ARC');
UPDATE `reasonToVisit` SET `reasonToVisitId`=0 WHERE `reasonToVisitId`=1;
ALTER TABLE `reasonToVisit` auto_increment=1;

INSERT INTO `patients` (`patientId`, `licenceId`, `patientOriginId`, `reasonToVisitId`, `name`, `gender`, `dateBirth`, `uniqueId`, `country`, `region`, `city`, `zip`, `address`, `email`, `phone`, `comment`, `archive`) VALUES
 (0, 0, 0, 0, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '');
 UPDATE `patients` SET `patientId`=0 WHERE `patientId`=1;
 ALTER TABLE `patients` auto_increment=1;

INSERT INTO `attendance` (`attendanceId`, `licenceId`, `patientId`, `date`, `length`, `weight`, `height`, `bloodPressureStart`, `pulseStart`, `bloodPressureStop`, `pulseStop`, `medicineCurrent`, `medicineAllergy`, `comment`, `archive`) VALUES
 (0, 0, 0, '0000-00-00', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '');
 UPDATE `attendance` SET `attendanceId`=0 WHERE `attendanceId`=1;
 ALTER TABLE `attendance` auto_increment=1;
