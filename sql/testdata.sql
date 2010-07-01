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

USE `belenus`;

-- -----------------------------------------------------------------------------------

INSERT INTO `licences` (`licenceId`, `serial`, `country`, `region`, `city`, `zip`, `address`, `studio`, `contact`, `active`, `archive`) VALUES
  ( 0, 'BLNS_SERIAL_GLOBAL', 'Magyarorsz�g', 'Pest megye', 'Budapest', '1139', 'ROZSNYAI U. 11', 'Polarium Kft.', NULL, 1, 'ARC');
INSERT INTO `licences` (`licenceId`, `serial`, `country`, `region`, `city`, `zip`, `address`, `studio`, `contact`, `active`, `archive`) VALUES
  ( 1, 'BLNS_SERIAL_DEMO', 'Magyarorsz�g', 'Pest megye', 'Budapest', '1139', 'ROZSNYAI U. 11', 'Polarium Kft.', NULL, 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `groups`, `active`, `comment`) VALUES
  ( 1, 'admin', 'Adminisztr�tor', 'd033e22ae348aeb5660fc2140aec35850c4da997', 'admin,user', 1, 'Gy�gycentrum alap�rtelmezett felhaszn�l�ja rendszergazdai jogosults�gokkal.' );
INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `groups`, `active`, `comment`) VALUES
  ( 1, 'kezelo', 'Kezel�', 'e47c3168137789e3e0df75d23452cabcc9a890f5', 'user', 1, 'Gy�gycentrum alap�rtelmezett felhaszn�l�ja alap felhaszn�l�i jogosults�gokkal' );

-- -----------------------------------------------------------------------------------

INSERT INTO `activateCommand` (`activateCommandId`, `name`) VALUES
 (0, 'Alap �llapot'),
 (1, 'El�k�sz�t�s'),
 (3, 'Kezel�s'),
 (4, 'Ut�h�t�s'),
 (9, 'V�rakoz�s');

-- -----------------------------------------------------------------------------------

INSERT INTO  `patientOrigin` (`patientOriginId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '<Nincs megadva>', 1, 'ARC');
UPDATE `patientOrigin` SET `patientOriginId`=0 WHERE `patientOriginId`=1;
ALTER TABLE `patientOrigin` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `reasonToVisit` (`reasonToVisitId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '<Nincs megadva>', 1, 'ARC');
UPDATE `reasonToVisit` SET `reasonToVisitId`=0 WHERE `reasonToVisitId`=1;
ALTER TABLE `reasonToVisit` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `patients` (`patientId`, `licenceId`, `patientOriginId`, `reasonToVisitId`, `name`, `gender`, `dateBirth`, `uniqueId`, `country`, `region`, `city`, `zip`, `address`, `email`, `phone`, `comment`, `active`, `archive`) VALUES
 (0, 0, 0, 0, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 'ARC');
UPDATE `patients` SET `patientId`=0 WHERE `patientId`=1;
ALTER TABLE `patients` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `attendance` (`attendanceId`, `licenceId`, `patientId`, `date`, `length`, `weight`, `height`, `bloodPressureStart`, `pulseStart`, `bloodPressureStop`, `pulseStop`, `medicineCurrent`, `medicineAllergy`, `comment`, `active`, `archive`) VALUES
 (0, 0, 0, '0000-00-00', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 'ARC');
UPDATE `attendance` SET `attendanceId`=0 WHERE `attendanceId`=1;
ALTER TABLE `attendance` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `vatpercent`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `active`, `archive`) VALUES
 (0, 0, 'Inakt�v k�rty�k', 0, 0, 0, '2010-01-01', '2010-01-01', 0, 0, 0, 'ARC');
UPDATE `patientCardTypes` SET `patientCardTypeId`=0 WHERE `patientCardTypeId`=1;
ALTER TABLE `patientCardTypes` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `panelTypes` ( `licenceId`, `name`, `active`, `archive` ) VALUES
 ( 1, "Sensolite gy�gyter�pi�s g�p", 1, "ARC" );

-- -----------------------------------------------------------------------------------

INSERT INTO `panelStatuses` ( `licenceId`, `panelTypeId`, `seqNumber`, `name`, `length`, `activateCmd`, `active`, `archive` ) VALUES
 ( 1, 1, 1, "Alap �llapot", 0, 0, 1, "ARC" ),
 ( 1, 1, 2, "El�k�sz�let/Vetk�z�s", 60, 1, 1, "ARC" ),
 ( 1, 1, 3, "V�rakoz�s", 3, 9, 1, "ARC" ),
 ( 1, 1, 4, "Kezel�s", 0, 3, 1, "ARC" ),
 ( 1, 1, 5, "Ut�h�t�s", 60, 4, 1, "ARC" );

-- -----------------------------------------------------------------------------------

INSERT INTO `patientOrigin` (`patientOriginId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, 1, 'Ismer�s/bar�t aj�nlotta', 1, 'ARC'),
 (2, 1, 'Sz�r�lap', 1, 'ARC'),
 (3, 1, 'Internet', 1, 'ARC'),
 (4, 1, 'TV', 1, 'ARC'),
 (5, 1, '�js�g', 1, 'ARC'),
 (6, 1, 'Egy�b', 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `reasonToVisit` (`reasonToVisitId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, 1, 'Gy�gyul�s betegs�gb�l', 1, 'ARC'),
 (2, 1, 'Immuner�s�t�s / regener�l�d�s', 1, 'ARC');

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

INSERT INTO `belenus`.`ledgertypes` (`ledgerTypeId` ,`licenceId` ,`name` ,`active` ,`archive`) VALUES
 (1 , '0', 'G�phaszn�lat', '1', 'ARC'),
 (2 , '0', 'B�rlet elad�s', '1', 'ARC'),
 (3 , '0', 'B�rlet felt�lt�s', '1', 'ARC'),
 (4 , '0', 'Term�k elad�s', '1', 'ARC'),
 (5 , '0', 'Egy�b', '1', 'ARC');

-- -----------------------------------------------------------------------------------
-- Tablak feltoltese teszt adatokkal
--
-- Ezek az INSERT-ek a teszteles miatt kellenek
--
-- -----------------------------------------------------------------------------------

INSERT INTO `panels` ( `licenceId`, `panelTypeId`, `title`, `active`, `archive` ) VALUES
  ( 1, 1, "1-es g�p", 1, "ARC" ),
  ( 1, 1, "2-es g�p", 1, "ARC" ),
  ( 1, 1, "3-as g�p", 1, "ARC" ),
  ( 1, 1, "4-es g�p", 1, "ARC" ),
  ( 1, 1, "5-�s g�p", 1, "ARC" ),
  ( 1, 1, "6-os g�p", 1, "ARC" ),
  ( 1, 1, "7-es g�p", 1, "ARC" ),
  ( 1, 1, "8-as g�p", 1, "ARC" );

INSERT INTO `patients` (`patientId`, `licenceId`, `patientOriginId`, `reasonToVisitId`, `name`, `gender`, `dateBirth`, `uniqueId`, `country`, `region`, `city`, `zip`, `address`, `email`, `phone`, `comment`, `active`, `archive`) VALUES
(1, 1, 0, 0, 'Kov�cs G�za', 1, '1980-02-11', '1980 0211', '', '', '', '', '', '', '', '', 1, 'NEW'),
(2, 1, 2, 2, 'Herendi Porcel�n', 2, '1972-07-25', '1972 0725', '', '', '', '', '', '', '', '', 1, 'NEW'),
(3, 1, 6, 1, 'Mik� Enik�', 2, '1985-09-19', '1985 0919', '', '', '', '', '', '', '', '', 1, 'NEW'),
(4, 1, 0, 0, 'Kalocsai Andrea', 2, '1998-12-07', '1998 1207', '', '', '', '', '', '', '', '', 1, 'NEW'),
(5, 1, 0, 0, 'Szegedi Paprika', 1, '1981-11-17', '1982 1117', '', '', '', '', '', '', '', '', 1, 'NEW');

