-- -----------------------------------------------------------------------------------
--
-- Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : belenus.sql
-- AppVersion  : 1.0
-- FileVersion : 1.0
-- Author      : Ballok Peter, Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- Adatbazist letrehozo SQL script
-- -----------------------------------------------------------------------------------

-- -----------------------------------------------------------------------------------
-- Letezo tablak torlese
-- -----------------------------------------------------------------------------------

DROP TABLE IF EXISTS `clients`;
DROP TABLE IF EXISTS `doctors`;
DROP TABLE IF EXISTS `licenses`;
DROP TABLE IF EXISTS `logs`;
DROP TABLE IF EXISTS `panels`;
DROP TABLE IF EXISTS `panelStatuses`;
DROP TABLE IF EXISTS `panelTypes`;
DROP TABLE IF EXISTS `patientCardHistories`;
DROP TABLE IF EXISTS `patientCards`;
DROP TABLE IF EXISTS `patientCardTypes`;
DROP TABLE IF EXISTS `patients`;
DROP TABLE IF EXISTS `productTypes`;
DROP TABLE IF EXISTS `users`;

-- -----------------------------------------------------------------------------------
-- Tablak letrehozasa
-- -----------------------------------------------------------------------------------

-- -----------------------------------------------------------------------------------
-- Kliens tabla. A szerver alkalmazasnal van szerepe. Kliens install-nal nincs ra
-- szukseg. A kliens program nem hasznalja.
-- -----------------------------------------------------------------------------------
CREATE TABLE `clients` (
  `clientId`                int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `code`                    varchar(50)             NOT NULL,
  `dateCreated`             datetime                NOT NULL,
  `lastLogin`               datetime                NOT NULL,
  PRIMARY KEY (`clientId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;

-- -----------------------------------------------------------------------------------
-- Doktor tabla. A studioban rendelo doktorok adatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `doctors` (
  `doctorId`                int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `name`                    varchar(100)            NOT NULL,
  `data`                    text                    NOT NULL,
  PRIMARY KEY (`doctorId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;

-- -----------------------------------------------------------------------------------
-- Liszensz tabla. Ez tartalmazza a kliens alkalmazas liszensz kulcsat es a Studio
-- egyedi adatait.
-- -----------------------------------------------------------------------------------
CREATE TABLE `licenses` (
  `licenceId`               int(10) unsigned        NOT NULL,
  `serial`                  varchar(30)             NOT NULL,
  `country`                 varchar(100)            DEFAULT NULL,
  `region`                  varchar(100)            DEFAULT NULL,
  `city`                    varchar(100)            DEFAULT NULL,
  `zip`                     varchar(10)             DEFAULT NULL,
  `address`                 varchar(100)            DEFAULT NULL,
  `studio`                  varchar(100)            DEFAULT NULL,
  `contact`                 varchar(100)            DEFAULT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  PRIMARY KEY (`licenceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;

-- -----------------------------------------------------------------------------------
-- Log tabla. A program mukodese soran keletkezo log-okat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `logs` (
  `date`                    timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `userId`                  int(10) unsigned        DEFAULT NULL,
  `severity`                int(1)                  NOT NULL DEFAULT 5,
  `message`                 text                    DEFAULT NULL,
  INDEX logs_date(`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Panel tabla. A kliens alkalmazasban mukodtetett panelokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panels` (
  `panelId`                 int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `panelTypeId`             int(10) unsigned        NOT NULL REFERENCES `panelTypes` (`panelTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `title`                   varchar(50)             NOT NULL,
  `workTime`                int(10) unsigned        NOT NULL DEFAULT 0,
  PRIMARY KEY (`panelId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Panel status tabla. A kliens alkalmazasban mukodtetett panelok kulonbozo status-ait
-- tartalmazza, melyek panel tipusokhoz kapcsolodnak.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelStatuses` (
  `panelStatusId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `panelTypeId`             int(10) unsigned        NOT NULL REFERENCES `panelTypes` (`panelTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `seqNumber`               int(10) unsigned        NOT NULL,
  `name`                    varchar(20)             NOT NULL,
  `length`                  int(10) unsigned        NOT NULL DEFAULT 0,
  `activateCmd`             int(10) unsigned        NOT NULL DEFAULT 0,
  PRIMARY KEY (`panelStatusId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Panel tipus tabla. A kliens alkalmazasban mukodtetett panelok tipusait tartalmazza.
-- Paneltipus hatarozza meg az adott panel mukodeset.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelTypes` (
  `panelTypeId`             int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `name`                    varchar(50)             NOT NULL,
  PRIMARY KEY (`panelTypeId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Kartya tortenet tabla. A studioban eladott kartyak hasznalatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCardHistories` (
  `patientCardHistoryId`    int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `patientCardId`           int(10) unsigned        NOT NULL REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `dateTimeUsed`            datetime                NOT NULL,
  `units`                   int(11)                 NOT NULL,
  PRIMARY KEY (`patientCardHistoryId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Kartya tabla. A studioban eladott kartyak adatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCards` (
  `patientCardId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `patientCardTypeId`       int(10) unsigned        NOT NULL REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `patientId`               int(10) unsigned        NOT NULL REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `barcode`                 varchar(20)             NOT NULL,
  `comment`                 varchar(50)             DEFAULT NULL,
  `units`                   int(11)                 NOT NULL,
  `validDate`               date                    NOT NULL,
  `pincode`                 varchar(5)              DEFAULT NULL,
  PRIMARY KEY (`patientCardId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Kartya tipus tabla. A studioban eladott kartyak tipusait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCardTypes` (
  `patientCardTypeId`       int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `name`                    varchar(50)             NOT NULL,
  `price`                   float(2)                NOT NULL DEFAULT 0.00,
  `units`                   int(11)                 NOT NULL DEFAULT 0,
  `validDateFrom`           date                    DEFAULT NULL,
  `validDateTo`             date                    DEFAULT NULL,
  `validDays`               int(11)                 NOT NULL DEFAULT 365,
  `unitTime`                int(11)                 NOT NULL DEFAULT 0,
  PRIMARY KEY (`patientCardTypeId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Paciens tabla. Opcionalis. A studio vendegeinek adatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patients` (
  `patientId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `name`                    varchar(100)            NOT NULL,
  `uniqueId`                varchar(50)             NOT NULL,
  `data`                    text                    NOT NULL,
  PRIMARY KEY (`patientId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;

-- -----------------------------------------------------------------------------------
-- Termek tabla. Opcionalis. A studioban forgalmazott termekeket tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `products` (
  `productId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `productTypeId`           int(11) unsigned        NOT NULL REFERENCES `productTypes` (`productTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `name`                    varchar(50)             NOT NULL,
  PRIMARY KEY (`productId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Termek tipus tabla. Opcionalis. A studioban forgalmazott termekek csoportjait
-- tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `productTypes` (
  `productTypeId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `name`                    varchar(50)             NOT NULL,
  PRIMARY KEY (`productTypeId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Felhasznalo tabla. A kliens alkalmazast uzemelteto felhasznalok adatait tartalmazza
-- -----------------------------------------------------------------------------------
CREATE TABLE `users` (
  `userId`                  int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL REFERENCES `licenses` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `name`                    varchar(20)             NOT NULL,
  `realName`                varchar(50)             NOT NULL,
  `password`                char(40)                NOT NULL,
  `groups`                  varchar(50)             NOT NULL DEFAULT 'user',
  `active`                  tinyint(1) unsigned     NOT NULL DEFAULT '1',
  `comment`                 text                    DEFAULT NULL,
  UNIQUE (`name`),
  PRIMARY KEY (`userId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Tablak feltoltese default adatokkal
-- -----------------------------------------------------------------------------------

INSERT INTO `users` (`name`, `realName`, `password`, `groups`) VALUES
  ('admin', 'Administrator', 'd033e22ae348aeb5660fc2140aec35850c4da997', 'admin,user');

