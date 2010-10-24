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

INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `vatpercent`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `active`, `archive`) VALUES
(2, 1, '10 alkalmas feln�tt b�rlet', 60000, 0, 13, '2010-08-26', '2010-08-26', 365, 20, 1, 'NEW'),
(3, 1, '10 alkalmas nyugd�jas b�rlet', 37000, 0, 10, '2010-08-26', '2010-08-26', 365, 20, 1, 'NEW');

INSERT INTO `panelUses` (`licenceId`, `panelId`, `name`, `useTime`, `usePrice`, `archive`) VALUES
  (0, 1, 'Csecsem� A', 5, 1600, 'ARC'),
  (0, 1, 'Csecsem� B', 5, 2400, 'ARC'),
  (0, 1, 'Gyermek B', 10, 3200, 'ARC'),
  (0, 1, 'Gyermek A', 15, 4000, 'ARC'),
  (0, 1, 'Nyugd�jas', 20, 3840, 'ARC'),
  (0, 1, 'Feln�tt', 20, 4800, 'ARC');

