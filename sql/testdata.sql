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
-- Az itt lathato INSERT INTO `licences` ... parancs csak fejlesztes celjara kell
--
-- Az INSERT INTO `users` ... parancsot a telepito program fogja lefuttatni
--
-- -----------------------------------------------------------------------------------

INSERT INTO `licences` (`licenceId`, `serial`, `country`, `region`, `city`, `zip`, `address`, `studio`, `contact`, `active`) VALUES
  ( 0, 'BLNS_SERIAL_DEMO', 'Budapest', NULL, NULL, NULL, NULL, NULL, NULL, 1);

INSERT INTO `users` (`name`, `licenceId`, `realName`, `password`, `groups`) VALUES
  ( 'admin', 0, 'Administrator', 'd033e22ae348aeb5660fc2140aec35850c4da997', 'admin,user');

INSERT INTO `panelTypes` ( `licenceId`, `name` ) VALUES
  ( 0, "Panel Type 1" );

INSERT INTO `panels` ( `licenceId`, `panelTypeId`, `title` ) VALUES
  ( 0, 1, "Panel 1" ),
  ( 0, 1, "Panel 2" ),
  ( 0, 1, "Panel 3" ),
  ( 0, 1, "Panel 4" ),
  ( 0, 1, "Panel 5" ),
  ( 0, 1, "Panel 6" ),
  ( 0, 1, "Panel 7" ),
  ( 0, 1, "Panel 8" );
