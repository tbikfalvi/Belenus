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
  ( 0, 'BLNS_SERIAL_GLOBAL', 'Magyarország', 'Pest megye', 'Budapest', '1139', 'ROZSNYAI U. 11', 'Polarium Kft.', NULL, 1, 'ARC');
INSERT INTO `licences` (`licenceId`, `serial`, `country`, `region`, `city`, `zip`, `address`, `studio`, `contact`, `active`, `archive`) VALUES
  ( 1, 'BLNS_SERIAL_DEMO', 'Magyarország', 'Pest megye', 'Budapest', '1139', 'ROZSNYAI U. 11', 'Polarium Kft.', NULL, 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `groups`, `active`, `comment`) VALUES
  ( 1, 'admin', 'Adminisztrátor', 'd033e22ae348aeb5660fc2140aec35850c4da997', 'admin,user', 1, 'Gyógycentrum alapértelmezett felhasználója rendszergazdai jogosultságokkal.' );
INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `groups`, `active`, `comment`) VALUES
  ( 1, 'kezelo', 'Kezelõ', 'e47c3168137789e3e0df75d23452cabcc9a890f5', 'user', 1, 'Gyógycentrum alapértelmezett felhasználója alap felhasználói jogosultságokkal' );

-- -----------------------------------------------------------------------------------

INSERT INTO `activateCommand` (`activateCommandId`, `name`) VALUES
 (0, 'Alap állapot'),
 (1, 'Elõkészítés'),
 (3, 'Kezelés'),
 (4, 'Utóhûtés'),
 (9, 'Várakozás');

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
 (0, 0, 'Inaktív kártyák', 0, 0, 0, '2010-01-01', '2010-01-01', 0, 0, 0, 'ARC');
UPDATE `patientCardTypes` SET `patientCardTypeId`=0 WHERE `patientCardTypeId`=1;
ALTER TABLE `patientCardTypes` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `panelTypes` ( `licenceId`, `name`, `active`, `archive` ) VALUES
 ( 1, "Sensolite gyógyterápiás gép", 1, "ARC" );

-- -----------------------------------------------------------------------------------

INSERT INTO `panelStatuses` ( `licenceId`, `panelTypeId`, `seqNumber`, `name`, `length`, `activateCmd`, `active`, `archive` ) VALUES
 ( 1, 1, 1, "Alap állapot", 0, 0, 1, "ARC" ),
 ( 1, 1, 2, "Elõkészület/Vetkõzés", 60, 1, 1, "ARC" ),
 ( 1, 1, 3, "Várakozás", 3, 9, 1, "ARC" ),
 ( 1, 1, 4, "Kezelés", 0, 3, 1, "ARC" ),
 ( 1, 1, 5, "Utóhûtés", 60, 4, 1, "ARC" );

-- -----------------------------------------------------------------------------------

INSERT INTO `patientOrigin` (`patientOriginId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, 1, 'Ismerõs/barát ajánlotta', 1, 'ARC'),
 (2, 1, 'Szórólap', 1, 'ARC'),
 (3, 1, 'Internet', 1, 'ARC'),
 (4, 1, 'TV', 1, 'ARC'),
 (5, 1, 'Újság', 1, 'ARC'),
 (6, 1, 'Egyéb', 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `reasonToVisit` (`reasonToVisitId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, 1, 'Gyógyulás betegségbõl', 1, 'ARC'),
 (2, 1, 'Immunerõsítés / regenerálódás', 1, 'ARC');

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
 (1 , '0', 'Géphasználat', '1', 'ARC'),
 (2 , '0', 'Bérlet eladás', '1', 'ARC'),
 (3 , '0', 'Bérlet feltöltés', '1', 'ARC'),
 (4 , '0', 'Termék eladás', '1', 'ARC'),
 (5 , '0', 'Egyéb', '1', 'ARC');

-- -----------------------------------------------------------------------------------
-- Tablak feltoltese teszt adatokkal
--
-- Ezek az INSERT-ek a teszteles miatt kellenek
--
-- -----------------------------------------------------------------------------------

INSERT INTO `panels` ( `licenceId`, `panelTypeId`, `title`, `active`, `archive` ) VALUES
  ( 1, 1, "1-es gép", 1, "ARC" ),
  ( 1, 1, "2-es gép", 1, "ARC" ),
  ( 1, 1, "3-as gép", 1, "ARC" ),
  ( 1, 1, "4-es gép", 1, "ARC" ),
  ( 1, 1, "5-ös gép", 1, "ARC" ),
  ( 1, 1, "6-os gép", 1, "ARC" ),
  ( 1, 1, "7-es gép", 1, "ARC" ),
  ( 1, 1, "8-as gép", 1, "ARC" );

INSERT INTO `patients` (`patientId`, `licenceId`, `patientOriginId`, `reasonToVisitId`, `name`, `gender`, `dateBirth`, `uniqueId`, `country`, `region`, `city`, `zip`, `address`, `email`, `phone`, `comment`, `active`, `archive`) VALUES
(1, 1, 0, 0, 'Kovács Géza', 1, '1980-02-11', '1980 0211', '', '', '', '', '', '', '', '', 1, 'NEW'),
(2, 1, 2, 2, 'Herendi Porcelán', 2, '1972-07-25', '1972 0725', '', '', '', '', '', '', '', '', 1, 'NEW'),
(3, 1, 6, 1, 'Mikó Enikõ', 2, '1985-09-19', '1985 0919', '', '', '', '', '', '', '', '', 1, 'NEW'),
(4, 1, 0, 0, 'Kalocsai Andrea', 2, '1998-12-07', '1998 1207', '', '', '', '', '', '', '', '', 1, 'NEW'),
(5, 1, 0, 0, 'Szegedi Paprika', 1, '1981-11-17', '1982 1117', '', '', '', '', '', '', '', '', 1, 'NEW');

