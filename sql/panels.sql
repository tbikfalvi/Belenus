-- -----------------------------------------------------------------------------------
--
-- Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
--
-- -----------------------------------------------------------------------------------
--
-- Filename : panels.sql
-- AppVersion : 1.0
-- FileVersion : 1.0
-- Author : Bikfalvi Tamas
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
