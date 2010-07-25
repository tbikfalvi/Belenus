-- -----------------------------------------------------------------------------------
--
-- Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
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

DROP DATABASE IF EXISTS `belenus`;

CREATE DATABASE `belenus` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

USE `belenus`;

-- -----------------------------------------------------------------------------------
-- Tablak letrehozasa
-- -----------------------------------------------------------------------------------

-- -----------------------------------------------------------------------------------
-- Liszensz tabla. Ez tartalmazza a kliens alkalmazas liszensz kulcsat es a Studio
-- egyedi adatait.
-- -----------------------------------------------------------------------------------
CREATE TABLE `licences` (
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
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`licenceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Kliens tabla. A szerver alkalmazasnal van szerepe. Kliens install-nal nincs ra
-- szukseg. A kliens program nem hasznalja.
-- -----------------------------------------------------------------------------------
CREATE TABLE `clients` (
  `clientId`                int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `code1`                   varchar(50)             NOT NULL,
  `code2`                   varchar(50)             NOT NULL,
  `dateCreated`             datetime                NOT NULL,
  `lastLogin`               datetime                NOT NULL,
  PRIMARY KEY (`clientId`),
  UNIQUE (`code1`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;

-- -----------------------------------------------------------------------------------
-- Doktor tabla. A studioban rendelo doktorok adatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `doctors` (
  `doctorId`                int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `doctorLicence`           varchar(50)             NOT NULL,
  `data`                    text                    NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`doctorId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Doktorok rendelesi idejet tartalmazo tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `doctorSchedule` (
  `doctorScheduleId`        int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `doctorId`                int(10) unsigned        NOT NULL,
  `dayOfWeek`               int(11)                 NOT NULL,
  `timeFrom`                time                    DEFAULT NULL,
  `timeTo`                  time                    DEFAULT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`doctorScheduleId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`doctorId`) REFERENCES `doctors` (`doctorId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
-- Panel tipus tabla. A kliens alkalmazasban mukodtetett panelok tipusait tartalmazza.
-- Paneltipus hatarozza meg az adott panel mukodeset.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelTypes` (
  `panelTypeId`             int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelTypeId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Panel tabla. A kliens alkalmazasban mukodtetett panelokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panels` (
  `panelId`                 int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `panelTypeId`             int(10) unsigned        NOT NULL,
  `title`                   varchar(50)             NOT NULL,
  `workTime`                int(10) unsigned        NOT NULL DEFAULT 0,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelTypeId`) REFERENCES `panelTypes` (`panelTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Panel tabla. A kliens alkalmazasban mukodtetett panelokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelUses` (
  `panelUseId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `useTime`                 int(11)                 NOT NULL,
  `usePrice`                int(11)                 NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelUseId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- activateCommand tabla. A statuszokhoz tartozo parancsokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `activateCommand` (
  `activateCommandId`       int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  PRIMARY KEY (`activateCommandId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Panel status tabla. A kliens alkalmazasban mukodtetett panelok kulonbozo status-ait
-- tartalmazza, melyek panel tipusokhoz kapcsolodnak.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelStatuses` (
  `panelStatusId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `panelTypeId`             int(10) unsigned        NOT NULL,
  `seqNumber`               int(10) unsigned        NOT NULL,
  `name`                    varchar(20)             NOT NULL,
  `length`                  int(10) unsigned        NOT NULL DEFAULT 0,
  `activateCmd`             int(10) unsigned        NOT NULL DEFAULT 0,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelStatusId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelTypeId`) REFERENCES `panelTypes` (`panelTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Kartya tipus tabla. A studioban eladott kartyak tipusait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCardTypes` (
  `patientCardTypeId`       int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `price`                   float(2)                NOT NULL DEFAULT 0.00,
  `vatpercent`              int(11)                 NOT NULL,
  `units`                   int(11)                 NOT NULL DEFAULT 0,
  `validDateFrom`           date                    DEFAULT NULL,
  `validDateTo`             date                    DEFAULT NULL,
  `validDays`               int(11)                 NOT NULL DEFAULT 365,
  `unitTime`                int(11)                 NOT NULL DEFAULT 0,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientCardTypeId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- patientOrigin tabla. Opcionalis. Azokat a helyeket tartalmazza, ahonnan a paciens
-- ertesulhetett a studio-rol.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientOrigin` (
  `patientOriginId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientOriginId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- reasonToVisit tabla. Opcionalis. Az indokokat tartalmazza, amiert jott a paciens
-- -----------------------------------------------------------------------------------
CREATE TABLE `reasonToVisit` (
  `reasonToVisitId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`reasonToVisitId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Paciens tabla. Opcionalis. A studio vendegeinek adatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patients` (
  `patientId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientOriginId`         int(10) unsigned        NOT NULL,
  `reasonToVisitId`         int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `gender`                  int(11)                 DEFAULT NULL,
  `dateBirth`               date                    DEFAULT NULL,
  `uniqueId`                varchar(20)             DEFAULT NULL,
  `country`                 varchar(100)            DEFAULT NULL,
  `region`                  varchar(100)            DEFAULT NULL,
  `city`                    varchar(100)            DEFAULT NULL,
  `zip`                     varchar(10)             DEFAULT NULL,
  `address`                 varchar(100)            DEFAULT NULL,
  `email`                   varchar(100)            DEFAULT NULL,
  `phone`                   varchar(100)            DEFAULT NULL,
  `comment`                 text                    DEFAULT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientOriginId`) REFERENCES `patientOrigin` (`patientOriginId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`reasonToVisitId`) REFERENCES `reasonToVisit` (`reasonToVisitId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Pacienshez kotheto egyeb cimek: pl. szamlazas miatt
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientAddress` (
  `patientAddressId`        int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `patientId`               int(10) unsigned        NOT NULL,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `country`                 varchar(100)            DEFAULT NULL,
  `region`                  varchar(100)            DEFAULT NULL,
  `city`                    varchar(100)            DEFAULT NULL,
  `zip`                     varchar(10)             DEFAULT NULL,
  `address`                 varchar(100)            DEFAULT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientAddressId`,`licenceID`),
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Paciensek altal lefoglalt idopontokat tartalmazo tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `attendanceSchedule` (
  `attendanceScheduleId`    int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `dayOfWeek`               int(11)                 NOT NULL,
  `timeFrom`                time                    DEFAULT NULL,
  `timeTo`                  time                    DEFAULT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`attendanceScheduleId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- attendance tabla. Opcionalis. A paciensek kezeleseit nyilvantarto tabla
-- -----------------------------------------------------------------------------------
CREATE TABLE `attendance` (
  `attendanceId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `date`                    date                    NOT NULL,
  `length`                  time                    DEFAULT NULL,
  `weight`                  int(11)                 DEFAULT NULL,
  `height`                  int(11)                 DEFAULT NULL,
  `bloodPressureStart`      float                   DEFAULT NULL,
  `pulseStart`              float                   DEFAULT NULL,
  `bloodPressureStop`       float                   DEFAULT NULL,
  `pulseStop`               float                   DEFAULT NULL,
  `medicineCurrent`         varchar(500)            DEFAULT NULL,
  `medicineAllergy`         varchar(500)            DEFAULT NULL,
  `comment`                 text                    DEFAULT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`attendanceId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- toBeFilled tabla. Opcionalis. A nem kitoltott paciensek illetve kezelesekre
-- mutato rekordok
-- -----------------------------------------------------------------------------------
CREATE TABLE `toBeFilled` (
  `toBeFilledId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `attendanceId`            int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  PRIMARY KEY (`toBeFilledId`),
  FOREIGN KEY (`attendanceId`) REFERENCES `attendance` (`attendanceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Kartya tabla. A studioban eladott kartyak adatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCards` (
  `patientCardId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardTypeId`       int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `barcode`                 varchar(20)             NOT NULL,
  `comment`                 varchar(50)             DEFAULT NULL,
  `units`                   int(11)                 NOT NULL,
  `timeLeft`                time                    NOT NULL,
  `validDate`               date                    NOT NULL,
  `pincode`                 varchar(5)              DEFAULT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientCardId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Pacienseket es kartyakat osszekoto tabla arra az esetre, ha nem a kartyatulajdonos
-- hasznalja a kartyat
-- -----------------------------------------------------------------------------------
CREATE TABLE `connectPatientWithCard` (
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`licenceID`,`patientCardId`,`patientId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Kartya tortenet tabla. A studioban eladott kartyak hasznalatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCardHistories` (
  `patientCardHistoryId`    int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  `dateTimeUsed`            timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `units`                   int(11)                 NOT NULL,
  `time`                    time                    NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientCardHistoryId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Termek tipus tabla. Opcionalis. A studioban forgalmazott termekek csoportjait
-- tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `productTypes` (
  `productTypeId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`productTypeId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Termek tabla. Opcionalis. A studioban forgalmazott termekeket tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `products` (
  `productId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `productTypeId`           int(11) unsigned        NOT NULL REFERENCES `productTypes` (`productTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `name`                    varchar(50)             NOT NULL,
  `netPrice`                int(11)                 NOT NULL,
  `vatpercent`              int(11)                 NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`productId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productTypeId`) REFERENCES `productTypes` (`productTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Felhasznalo tabla. A kliens alkalmazast uzemelteto felhasznalok adatait tartalmazza
-- -----------------------------------------------------------------------------------
CREATE TABLE `users` (
  `userId`                  int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(20)             NOT NULL,
  `realName`                varchar(50)             NOT NULL,
  `password`                char(40)                NOT NULL,
  `accGroup`                int(1) unsigned         NOT NULL DEFAULT 1,
  `active`                  tinyint(1) unsigned     NOT NULL DEFAULT 1,
  `comment`                 text                    DEFAULT NULL,
  `archive`                 varchar(10)             NOT NULL,
  UNIQUE (`name`),
  PRIMARY KEY (`userId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Penztar tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `cassa` (
  `cassaId`                 int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `currentBalance`          int(11)                 NOT NULL,
  `startDateTime`           datetime                NOT NULL,
  `stopDateTime`            datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`cassaId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Penztar tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `cassaHistory` (
  `cassaHistoryId`          int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `cassaId`                 int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `actionValue`             int(11)                 NOT NULL,
  `actionBalance`           int(11)                 NOT NULL,
  `actionTime`              timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment`                 text                    DEFAULT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`cassaHistoryId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`cassaId`) REFERENCES `cassa` (`cassaId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Penztar tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `denominations` (
  `denominationId`          int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `denomination`            int(10) unsigned        NOT NULL DEFAULT 0,
  `comment`                 text                    DEFAULT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`denominationId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Penztar tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `cassaDenominations` (
  `denominationId`          int(10) unsigned        NOT NULL,
  `cassaId`                 int(10) unsigned        NOT NULL,
  `licenceId`               int(10) unsigned        NOT NULL,
  `value`                   int(10) unsigned        NOT NULL DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`denominationId`,`cassaId`,`licenceID`),
  FOREIGN KEY (`denominationId`) REFERENCES `denominations` (`denominationId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`cassaId`) REFERENCES `cassa` (`cassaId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Szamla tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `invoices` (
  `invoiceId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `name`                    varchar(100)            NOT NULL,
  `country`                 varchar(100)            DEFAULT NULL,
  `region`                  varchar(100)            NOT NULL,
  `city`                    varchar(100)            NOT NULL,
  `zip`                     varchar(10)             NOT NULL,
  `address`                 varchar(100)            NOT NULL,
  `product`                 varchar(100)            NOT NULL,
  `netPrice`                int(11)                 NOT NULL,
  `vatpercent`              int(11)                 NOT NULL,
  `totalPrice`              int(11)                 NOT NULL,
  `invoiceReady`            tinyint(1)              DEFAULT 0,
  PRIMARY KEY (`invoiceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Kosar tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `carts` (
  `cartId`                  int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `panelId`                 int(10) unsigned        NOT NULL,
  `productId`               int(10) unsigned        NOT NULL,
  `patientCardTypeId`       int(10) unsigned        NOT NULL,
  PRIMARY KEY (`cartId`),
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productId`) REFERENCES `products` (`productId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Konyveles tipusok tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `ledgerTypes` (
  `ledgerTypeId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`ledgerTypeId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Konyveles tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `ledger` (
  `ledgerId`                int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `ledgerTypeId`            int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `productId`               int(10) unsigned        NOT NULL,
  `patientCardTypeId`       int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `netPrice`                int(11)                 NOT NULL,
  `vatpercent`              int(11)                 NOT NULL,
  `ledgerTime`              timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment`                 text                    DEFAULT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`ledgerId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`ledgerTypeId`) REFERENCES `ledgerTypes` (`ledgerTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productId`) REFERENCES `products` (`productId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- GepKonyveles tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `ledgerDevice` (
  `ledgerDeviceId`          int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `units`                   int(11)                 NOT NULL,
  `cash`                    int(11)                 NOT NULL,
  `timeReal`                int(11)                 NOT NULL,
  `timeLeft`                int(11)                 NOT NULL,
  `timeCard`                int(11)                 NOT NULL,
  `timeCash`                int(11)                 NOT NULL,
  `ledgerTime`              timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment`                 text                    DEFAULT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`ledgerDeviceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
