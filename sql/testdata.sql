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

INSERT INTO `activatecommand` (`activateCommandId`, `name`) VALUES
 (0, 'Alap állapot'),
 (1, 'Elõkészítés'),
 (3, 'Kezelés'),
 (4, 'Utóhûtés'),
 (9, 'Várakozás');

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

INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `archive`) VALUES
 ('0', '0', 'System Administrator / Service', '0', '999', '2010-01-01', '2110-01-01', '0', '10', 'ARC');
UPDATE `patientCardTypes` SET `patientCardTypeId`=0 WHERE `patientCardTypeId`=1;
ALTER TABLE `patientCardTypes` auto_increment=1;

INSERT INTO `patientCards` (`patientCardId`, `licenceId`, `patientCardTypeId`, `patientId`, `barcode`, `comment`, `units`, `timeLeft`, `validDate`, `pincode`, `archive`) VALUES
 ('0', '0', '0', '0', 'BELENUS_SERVICECARD', 'Szervíz kártya', '999', '99:99:99', '2110-01-01', NULL, 'ARC');
UPDATE `patientCards` SET `patientCardId`=0 WHERE `patientCardId`=1;
ALTER TABLE `patientCards` auto_increment=1;

-- -----------------------------------------------------------------------------------
-- Tablak feltoltese teszt adatokkal
--
-- Ezek az INSERT-ek a teszteles miatt kellenek
--
-- -----------------------------------------------------------------------------------

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

INSERT INTO `patientOrigin` (`patientOriginId`, `licenceId`, `name`, `archive`) VALUES
(1, 1, 'Ismerõs/barát ajánlotta', 'ARC'),
(2, 1, 'Szórólap', 'ARC'),
(3, 1, 'Internet', 'ARC'),
(4, 1, 'TV', 'ARC'),
(5, 1, 'Újság', 'ARC'),
(6, 1, 'Egyéb', 'ARC');

INSERT INTO `reasonToVisit` (`reasonToVisitId`, `licenceId`, `name`, `archive`) VALUES
(1, 1, 'Gyógyulás betegségbõl', 'ARC'),
(2, 1, 'Immunerõsítés / regenerálódás', 'ARC');

INSERT INTO `patients` (`patientId`, `licenceId`, `patientOriginId`, `reasonToVisitId`, `name`, `gender`, `dateBirth`, `uniqueId`, `country`, `region`, `city`, `zip`, `address`, `email`, `phone`, `comment`, `archive`) VALUES
(1, 1, 0, 0, 'Kovács Géza', 1, '1980-02-11', '1980 0211', '', '', '', '', '', '', '', '', 'NEW'),
(2, 1, 2, 2, 'Herendi Porcelán', 2, '1972-07-25', '1972 0725', '', '', '', '', '', '', '', '', 'NEW'),
(3, 1, 6, 1, 'Mikó Enikõ', 2, '1985-09-19', '1985 0919', '', '', '', '', '', '', '', '', 'NEW'),
(4, 1, 0, 0, 'Kalocsai Andrea', 2, '1998-12-07', '1998 1207', '', '', '', '', '', '', '', '', 'NEW'),
(5, 1, 0, 0, 'Szegedi Paprika', 1, '1981-11-17', '1982 1117', '', '', '', '', '', '', '', '', 'NEW');

