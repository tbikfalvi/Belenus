-- -----------------------------------------------------------------------------------
--
-- Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : user.sql
-- AppVersion  : 1.0
-- FileVersion : 1.0
-- Author      : Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- Program felhasznalot letrehozo SQL script
-- -----------------------------------------------------------------------------------

CREATE USER 'belenus'@'localhost' IDENTIFIED BY 'belenus';

GRANT ALL PRIVILEGES ON belenus.* TO 'belenus'@'localhost';

