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

INSERT INTO `panels` ( `licenceId`, `panelTypeId`, `title`, `active`, `archive` ) VALUES
  ( 1, 1, "1-es gép", 1, "ARC" ),
  ( 1, 1, "2-es gép", 1, "ARC" ),
  ( 1, 1, "3-as gép", 1, "ARC" ),
  ( 1, 1, "4-es gép", 1, "ARC" ),
  ( 1, 1, "5-ös gép", 1, "ARC" ),
  ( 1, 1, "6-os gép", 1, "ARC" ),
  ( 1, 1, "7-es gép", 1, "ARC" ),
  ( 1, 1, "8-as gép", 1, "ARC" );

INSERT INTO `panelUses` (`licenceId`, `panelId`, `name`, `useTime`, `usePrice`, `archive`) VALUES
  (0, 1, 'Csecsemő A', 5, 1600, 'ARC'),
  (0, 1, 'Csecsemő B', 5, 2400, 'ARC'),
  (0, 1, 'Gyermek B', 10, 3200, 'ARC'),
  (0, 1, 'Gyermek A', 15, 4000, 'ARC'),
  (0, 1, 'Nyugdíjas', 20, 3840, 'ARC'),
  (0, 1, 'Felnőtt', 20, 4800, 'ARC');

INSERT INTO `patients` (`patientId`, `licenceId`, `patientOriginId`, `reasonToVisitId`, `name`, `gender`, `dateBirth`, `uniqueId`, `country`, `region`, `city`, `zip`, `address`, `email`, `phone`, `comment`, `active`, `archive`) VALUES
(1, 1, 0, 0, 'Kovács Géza', 1, '1980-02-11', '1980 0211', '', '', '', '', '', '', '', '', 1, 'NEW'),
(2, 1, 2, 2, 'Herendi Porcelán', 2, '1972-07-25', '1972 0725', '', '', '', '', '', '', '', '', 1, 'NEW'),
(3, 1, 6, 1, 'Mikó Enikő', 2, '1985-09-19', '1985 0919', '', '', '', '', '', '', '', '', 1, 'NEW'),
(4, 1, 0, 0, 'Kalocsai Andrea', 2, '1998-12-07', '1998 1207', '', '', '', '', '', '', '', '', 1, 'NEW'),
(5, 1, 0, 0, 'Szegedi Paprika', 1, '1981-11-17', '1982 1117', '', '', '', '', '', '', '', '', 1, 'NEW');

