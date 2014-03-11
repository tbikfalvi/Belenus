-- -----------------------------------------------------------------------------------
--
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2013
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : db_update_hu_1_0.sql
-- AppVersion  : 1.0.0
-- DbVersion   : 1.0
-- FileVersion : 1.0
-- Author      : Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- 1.0 adatbazis verziot frissito file
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value='1_0_4' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_1' WHERE identifier='DATABASE_VERSION';

INSERT INTO `ledgerTypes` (`ledgerTypeId` ,`licenceId` ,`name` ,`active` ,`archive`) VALUES
 (10, '0', 'Pénztár kiadás', '1', 'ARC');

-- -----------------------------------------------------------------------------------
