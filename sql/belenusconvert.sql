DROP DATABASE IF EXISTS `belenusconvert`;

CREATE DATABASE `belenusconvert` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

USE `belenusconvert`;

CREATE TABLE IF NOT EXISTS `berlet` (
  `strVonalkod` varchar(20) NOT NULL,
  `strMegjegyzes` varchar(50) NOT NULL,
  `nBerletTipus` int(11) NOT NULL,
  `nEgyseg` int(11) NOT NULL,
  `nErvEv` int(11) NOT NULL,
  `nErvHo` int(11) NOT NULL,
  `nErvNap` int(11) NOT NULL,
  `nPin` int(11) NOT NULL,
  PRIMARY KEY (`strVonalkod`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `berlethasznalat` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `strVonalkod` varchar(20) NOT NULL,
  `nEv` int(11) NOT NULL,
  `nHo` int(11) NOT NULL,
  `nNap` int(11) NOT NULL,
  `nOra` int(11) NOT NULL,
  `nPerc` int(11) NOT NULL,
  `nEgyseg` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;


CREATE TABLE IF NOT EXISTS `berlettipus` (
  `nId` int(11) NOT NULL,
  `nAr` int(11) NOT NULL,
  `nEgyseg` int(11) NOT NULL,
  `strNev` varchar(50) NOT NULL,
  `nErvTolEv` int(11) NOT NULL,
  `nErvTolHo` int(11) NOT NULL,
  `nErvTolNap` int(11) NOT NULL,
  `nErvIgEv` int(11) NOT NULL,
  `nErvIgHo` int(11) NOT NULL,
  `nErvIgNap` int(11) NOT NULL,
  `nErvNapok` int(11) NOT NULL,
  `bSzolariumHaszn` tinyint(1) NOT NULL,
  `nEgysegIdo` int(11) NOT NULL,
  PRIMARY KEY (`nId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

