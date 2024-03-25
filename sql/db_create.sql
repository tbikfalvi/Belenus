-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2013
-- -----------------------------------------------------------------------------------
-- Filename    : db_create.sql
-- AppVersion  : 2.0.0.0
-- DbVersion   : 2.0.0.0
-- -----------------------------------------------------------------------------------
-- Adatbazist letrehozo SQL script
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazas liszensz kulcsat es a Studio egyedi adatait tartalmazza.
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
  `lastValidated`           DATE                    NULL DEFAULT NULL,
  `type`                    varchar(20)             NOT NULL DEFAULT 'UNREGISTERED',
  `Act` 					VARCHAR( 10 ) 			NOT NULL,
  `Cod` 					VARCHAR( 10 ) 			NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`licenceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A kiwisun web oldalnak kuldendo uzeneteket tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `httppatientcardinfo` (
  `httpPatientcardInfoId`   int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `barcode`                 varchar(20)             NOT NULL,
  `patientcardInfoText`     text                    NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,  
  PRIMARY KEY (`httpPatientcardInfoId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Log tabla. A program mukodese soran keletkezo log-okat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `logs` (
  `date`                    timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `userId`                  int(10) unsigned        DEFAULT NULL,
  `severity`                int(1)                  NOT NULL DEFAULT 5,
  `threadId`                VARCHAR( 20 )           NOT NULL,
  `message`                 text                    DEFAULT NULL,
  INDEX logs_date(`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Settings tabla. A program mukodeset befolyasolo adatokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `settings` (
  `settingId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `identifier`              varchar(50)             NOT NULL,
  `value`                   text                    NOT NULL,
  UNIQUE (`identifier`),
  PRIMARY KEY (`settingId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazast uzemelteto felhasznalok adatait tartalmazza
-- -----------------------------------------------------------------------------------
CREATE TABLE `users` (
  `userId`                  int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(20)             NOT NULL,
  `realName`                varchar(50)             NOT NULL,
  `password`                char(40)                NOT NULL,
  `accGroup`                int(1) unsigned         NOT NULL DEFAULT 1,
  `comment`                 text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL DEFAULT 1,
  `archive`                 varchar(10)             NOT NULL,
  UNIQUE (`name`),
  PRIMARY KEY (`userId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A panel statuszokhoz tartozo parancsokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `activateCommand` (
  `activateCommandId`       int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  PRIMARY KEY (`activateCommandId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A vendegekhez kotheto cegek neveit tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `companies` (
  `companyId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `country`                 varchar(100)            DEFAULT NULL,
  `region`                  varchar(100)            DEFAULT NULL,
  `city`                    varchar(100)            DEFAULT NULL,
  `zip`                     varchar(10)             DEFAULT NULL,
  `address`                 varchar(100)            DEFAULT NULL,
  `contactPerson`           varchar(100)            DEFAULT NULL,
  `contractId`              varchar(100)            DEFAULT NULL,
  `validDateFrom`           date                    NOT NULL,
  `validDateTo`             date                    NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`companyId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Eletkor csoportokat tartalmazza
-- -----------------------------------------------------------------------------------
CREATE TABLE `ageTypes` (
  `ageTypeId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `ageTypeName`             varchar(50)             NOT NULL,
  PRIMARY KEY (`ageTypeId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Nemek leirasat tartalmazza. Vendegekhez.
-- -----------------------------------------------------------------------------------
CREATE TABLE `genders` (
  `genderId`                int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `genderName`              varchar(50)             NOT NULL,
  PRIMARY KEY (`genderId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Bortipusok leirasat tartalmazza. Vendegekhez.
-- -----------------------------------------------------------------------------------
CREATE TABLE `skinTypes` (
  `skinTypeId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `skinTypeName`            varchar(50)             NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`skinTypeId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studio vendegeinek adatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patients` (
  `patientId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `companyId`               int(10) unsigned        NOT NULL,
  `created`                 datetime                NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `gender`                  int(11)                 DEFAULT NULL,
  `ageType`                 int(11)                 DEFAULT 0,
  `isReturning`             tinyint(1)              DEFAULT 0,
  `uniqueId`                varchar(20)             DEFAULT NULL,
  `isCardy`                 tinyint(1)              NOT NULL DEFAULT 0,
  `email`                   varchar(100)            DEFAULT NULL,
  `regularCustomer`         tinyint(1)              DEFAULT 0,
  `employee`                tinyint(1)              DEFAULT 0,
  `service`                 tinyint(1)              DEFAULT 0,
  `company`                 tinyint(1)              DEFAULT 0,
  `discountType`            int(11)                 NOT NULL,
  `membership`				varchar( 30 ) 			NOT NULL,
  `dateBirth` 				date 					NOT NULL,
  `address` 				varchar( 200 ) 			NOT NULL,
  `skinTypeId` 				int(10) unsigned		DEFAULT 0,
  `mobile` 					varchar( 50 ) 			NOT NULL,
  `comment`                 text                    DEFAULT NULL,
  `loyaltyPoints`           int(11)                 DEFAULT 0,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`companyId`) REFERENCES `companies` (`companyId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`skinTypeId`) REFERENCES `skinTypes` (`skinTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studioban hasznalhato kartyak tipusait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCardTypes` (
  `patientCardTypeId`       int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `price`                   decimal(10,0)           NOT NULL DEFAULT 0.00,
  `vatpercent`              int(11)                 NOT NULL,
  `units`                   int(11)                 NOT NULL DEFAULT 0,
  `validDateFrom`           date                    DEFAULT NULL,
  `validDateTo`             date                    DEFAULT NULL,
  `validDays`               int(11)                 NOT NULL DEFAULT 365,
  `unitTime`                int(11)                 NOT NULL DEFAULT 0,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientCardTypeId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studioban hasznalhato kartyak tipusaihoz tartozo idotartamokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCardTypeEnabled` (
  `patientCardTypeEnabledId` int(10) unsigned       NOT NULL AUTO_INCREMENT,
  `licenceId`                int(10) unsigned       NOT NULL,
  `patientCardTypeId`        int(10) unsigned       NOT NULL,
  `validWeekDays`            int(11)                NOT NULL DEFAULT 127,
  `start`                    time                   NOT NULL,
  `stop`                     time                   NOT NULL,
  `modified`                 datetime               NOT NULL,
  `archive`                  varchar(10)            NOT NULL,
  PRIMARY KEY (`patientCardTypeEnabledId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studioban rogzitett, eladott kartyak adatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCards` (
  `patientCardId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardTypeId`       int(10) unsigned        NOT NULL,
  `parentCardId`            int(10) unsigned        DEFAULT 0,
  `patientId`               int(10) unsigned        NOT NULL,
  `servicecard`             BOOLEAN                 NOT NULL DEFAULT FALSE,
  `barcode`                 varchar(20)             NOT NULL,
  `rfId`                    varchar(30)             NOT NULL,
  `comment`                 varchar(50)             DEFAULT NULL,
  `units`                   int(11)                 NOT NULL,
  `amount`                  decimal(10,0)           NOT NULL,
  `timeLeft`                int(10) unsigned        NOT NULL,
  `validDateFrom`           date                    NOT NULL,
  `validDateTo`             date                    NOT NULL,
  `pincode`                 varchar(5)              DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientCardId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Pacienseket es kartyakat osszekoto tabla arra az esetre, ha nem csak a kartya
-- tulajdonosa hasznalja a kartyat
-- -----------------------------------------------------------------------------------
CREATE TABLE `connectPatientWithCard` (
  `patientCardId`           int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `licenceId`               int(10) unsigned        NOT NULL,
  PRIMARY KEY (`patientCardId`,`patientId`,`licenceId`),
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studioban eladott kartyak hasznalatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCardHistories` (
  `patientCardHistoryId`    int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  `dateTimeUsed`            timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `units`                   int(11)                 NOT NULL,
  `unitIds`                 varchar(100)            NOT NULL,
  `time`                    time                    NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientCardHistoryId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazasban mukodtetett panelok tipusait tartalmazza.
-- Paneltipus hatarozza meg az adott panel mukodeset.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelTypes` (
  `panelTypeId`             int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelTypeId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazasban mukodtetett panelok kulonbozo status-ait tartalmazza.
-- A statusok a panel tipusokhoz kapcsolodnak.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelStatuses` (
  `panelStatusId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `panelTypeId`             int(10) unsigned        NOT NULL,
  `seqNumber`               int(10) unsigned        NOT NULL,
  `name`                    varchar(20)             NOT NULL,
  `length`                  int(10) unsigned        NOT NULL DEFAULT 0,
  `activateCmd`             int(10) unsigned        NOT NULL DEFAULT 0,
  `skipLevel`               int(10)                 NOT NULL DEFAULT 0,
  `allowedToSkip`           tinyint(1)              NOT NULL DEFAULT 0,
  `stopLevel`               int(10)                 NOT NULL DEFAULT 0,
  `allowedToStop`           tinyint(1)              NOT NULL DEFAULT 0,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelStatusId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelTypeId`) REFERENCES `panelTypes` (`panelTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazasban mukodtetett panelok statusainak beallitasait tartalmazza
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelStatusSettings` (
  `panelStatusSettingId`    int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `panelStatusId`           int(10) unsigned        NOT NULL,
  `backgroundColor`         varchar(10)             NOT NULL,
  `statusFontName`          varchar(50)             NOT NULL,
  `statusFontSize`          int                     NOT NULL,
  `statusFontColor`         varchar(50)             NOT NULL,
  `timerFontName`           varchar(50)             NOT NULL,
  `timerFontSize`           int                     NOT NULL,
  `timerFontColor`          varchar(50)             NOT NULL,
  `nextFontName`            varchar(50)             NOT NULL,
  `nextFontSize`            int                     NOT NULL,
  `nextFontColor`           varchar(50)             NOT NULL,
  `infoFontName`            varchar(50)             NOT NULL,
  `infoFontSize`            int                     NOT NULL,
  `infoFontColor`           varchar(50)             NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelStatusSettingId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelStatusId`) REFERENCES `panelStatuses` (`panelStatusId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazasban mukodtetett panelcsoportokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelgroups` (
  `panelGroupId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `description`             text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelGroupId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazasban mukodtetett panelokat tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panels` (
  `panelId`                 int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `panelTypeId`             int(10) unsigned        NOT NULL,
  `panelGroupId`            int(10) unsigned        NOT NULL,
  `title`                   varchar(50)             NOT NULL,
  `imagePathFileName` 		varchar( 500 ) 			NOT NULL,
  `workTime`                int(10) unsigned        NOT NULL DEFAULT 0,
  `maxWorkTime`             int(10) unsigned        NOT NULL,
  `cleanTime`               int(10) unsigned        NOT NULL DEFAULT 0,
  `maxCleanTime`            int(10) unsigned        NOT NULL DEFAULT 0,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelTypeId`) REFERENCES `panelTypes` (`panelTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelGroupId`) REFERENCES `panelgroups` (`panelGroupId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studioban eladott kartyak hasznalatait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientCardUnits` (
  `patientCardUnitId`       int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  `patientCardTypeId`       int(10) unsigned        NOT NULL DEFAULT 0,
  `panelGroupID`       		int(10) unsigned        NOT NULL DEFAULT 0,
  `ledgerId`                int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `unitTime`                int(11)                 NOT NULL DEFAULT 0,
  `unitPrice`               int(11)                 NOT NULL DEFAULT 0,
  `validDateFrom`           date                    DEFAULT NULL,
  `validDateTo`             date                    DEFAULT NULL,
  `dateTimeUsed`            timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `prepared`                tinyint(1)              NOT NULL DEFAULT '0',
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientCardUnitId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A panelok (gepek) hasznalatait tartalmazo tabla.
-- Minden egyes hasznalat egy rekordban van rogzitve.
-- -----------------------------------------------------------------------------------
CREATE TABLE `panelUses` (
  `panelUseId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `useTime`                 int(11)                 NOT NULL,
  `usePrice`                decimal(10,0)           NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelUseId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Penz cimleteket tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `denominations` (
  `denominationId`          int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `denomination`            int(10) unsigned        NOT NULL DEFAULT 0,
  `comment`                 text                    DEFAULT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`denominationId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Konyveles tipusok tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `ledgerTypes` (
  `ledgerTypeId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`ledgerTypeId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studioban forgalmazott termekek csoportjait tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `productTypes` (
  `productTypeId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`productTypeId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studioban forgalmazott termekeket tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `products` (
  `productId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `barcode`                 varchar(20)             NOT NULL,
  `netPriceBuy`             decimal(10,0)           NOT NULL DEFAULT 0,
  `vatpercentBuy`           int(11)                 NOT NULL DEFAULT 0,
  `netPriceSell`            decimal(10,0)           NOT NULL DEFAULT 0,
  `vatpercentSell`          int(11)                 NOT NULL DEFAULT 0,
  `productCount`            int(11)                 NOT NULL DEFAULT 0,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`productId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studioban forgalmazott termekeket és csoportokat köti össze.
-- -----------------------------------------------------------------------------------
CREATE TABLE `connectProductWithType` (
  `productTypeId`           int(10) unsigned        NOT NULL,
  `productId`               int(10) unsigned        NOT NULL,
  `licenceId`               int(10) unsigned        NOT NULL,
  PRIMARY KEY (`productTypeId`,`productId`,`licenceId`),
  FOREIGN KEY (`productTypeId`) REFERENCES `productTypes` (`productTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productId`) REFERENCES `products` (`productId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studioban forgalmazott termekekkel kapcsolatos esemenyek (vetel, eladas stb.).
-- -----------------------------------------------------------------------------------
CREATE TABLE `productActionType` (
  `productActionTypeId`     int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `increaseProductCount`    tinyint(1) unsigned     NOT NULL,
  `decreaseProductCount`    tinyint(1) unsigned     NOT NULL,
  `cassaActionIndication`   varchar(20)             NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`productActionTypeId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
--
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientHistoryType` (
  `patientHistoryTypeId`    int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientHistoryTypeId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
--
-- -----------------------------------------------------------------------------------
CREATE TABLE `patientHistory` (
  `patientHistoryId`        int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `patientHistoryTypeId`    int(10) unsigned        NOT NULL,
  `productId`               int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `patientCardTypeId`       int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  PRIMARY KEY (`patientHistoryId`,`licenceId`),
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientHistoryTypeId`) REFERENCES `patientHistoryType` (`patientHistoryTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productId`) REFERENCES `products` (`productId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Cimekhez elore feltoltott megye, telepules es iranyitoszam rekordokat tartalmaz.
-- -----------------------------------------------------------------------------------
CREATE TABLE `zipRegionCity` (
  `zipRegionCityId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `zip`                     varchar(100)            NOT NULL,
  `region`                  varchar(100)            NOT NULL,
  `city`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`zipRegionCityId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A cimekhez a kozteruletek neveit tartalmazza
-- -----------------------------------------------------------------------------------
CREATE TABLE `publicPlaces` (
  `publicPlaceId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        DEFAULT 0,
  `name`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`publicPlaceId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A cimeket tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `address` (
  `addressId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `publicPlaceId`           int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `country`                 varchar(100)            DEFAULT NULL,
  `region`                  varchar(100)            DEFAULT NULL,
  `city`                    varchar(100)            DEFAULT NULL,
  `zip`                     varchar(10)             DEFAULT NULL,
  `street`                  varchar(50)             DEFAULT NULL,
  `streetNumber`            varchar(10)             DEFAULT NULL,
  `floor`                   varchar(10)             DEFAULT NULL,
  `door`                    varchar(10)             DEFAULT NULL,
  `primaryAddress`          tinyint(1)              DEFAULT 0,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`addressId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`publicPlaceId`) REFERENCES `publicPlaces` (`publicPlaceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazas napi penzforgalmanak egyenleget tartalmazza.
-- Minden rekord egy felhasznalohoz kotheto.
-- -----------------------------------------------------------------------------------
CREATE TABLE `cassa` (
  `cassaId`                 int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `currentBalance`          decimal(10,0)           NOT NULL,
  `startDateTime`           datetime                NOT NULL,
  `stopDateTime`            datetime                NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`cassaId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A kulonbozo penztarak zarasaihoz tartalmazo cimleteket tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `cassaDenominations` (
  `denominationId`          int(10) unsigned        NOT NULL,
  `cassaId`                 int(10) unsigned        NOT NULL,
  `licenceId`               int(10) unsigned        NOT NULL,
  `value`                   int(10) unsigned        NOT NULL DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`denominationId`,`cassaId`,`licenceId`),
  FOREIGN KEY (`denominationId`) REFERENCES `denominations` (`denominationId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`cassaId`) REFERENCES `cassa` (`cassaId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Fizetesi modokat tartalmazo tabla
-- -----------------------------------------------------------------------------------
CREATE TABLE `paymentMethods` (
  `paymentMethodId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`paymentMethodId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Kulonbozo a rendszerbe felvett kedvezmenyeket tartalmazza.
-- -----------------------------------------------------------------------------------
CREATE TABLE `discounts` (
  `discountId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        DEFAULT NULL,
  `companyId`               int(10) unsigned        DEFAULT NULL,
  `paymentMethodId`         int(10) unsigned        DEFAULT NULL,
  `productId`               int(10) unsigned        DEFAULT NULL,
  `regularCustomer`         tinyint(1)              DEFAULT 0,
  `employee`                tinyint(1)              DEFAULT 0,
  `service`                 tinyint(1)              DEFAULT 0,
  `timezoneStart`           time                    NOT NULL,
  `timezoneStop`            time                    NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `discountValue`           decimal(10,0)           NOT NULL,
  `discountPercent`         int(11)                 NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`discountId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`companyId`) REFERENCES `companies` (`companyId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`paymentMethodId`) REFERENCES `paymentmethods` (`paymentMethodId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productId`) REFERENCES `products` (`productId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Bevasarlo kosar elemeit tartalmazo tabla
-- -----------------------------------------------------------------------------------
CREATE TABLE `shoppingCartItems` (
  `shoppingCartItemId`      int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        DEFAULT NULL,
  `productId`               int(10) unsigned        DEFAULT NULL,
  `patientCardId`           int(10) unsigned        DEFAULT NULL,
  `patientCardTypeId`       int(10) unsigned        DEFAULT NULL,
  `panelId`                 int(10) unsigned        DEFAULT NULL,
  `ledgerTypeId`            int(10) unsigned        NOT NULL,
  `itemName`                varchar(100)            NOT NULL,
  `comment`                 text                    DEFAULT NULL,
  `itemCount`               int(11)                 NOT NULL,
  `itemNetPrice`            decimal(10,0)           NOT NULL,
  `card`                    decimal(10,0)           NOT NULL,
  `cash`                    decimal(10,0)           NOT NULL,
  `voucher`                 decimal(10,0)           NOT NULL,
  `itemVAT`                 int(11)                 NOT NULL,
  `discountValue`           decimal(10,0)           NOT NULL,
  `itemSumPrice`            decimal(10,0)           NOT NULL,
  `modified`                datetime                NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`shoppingCartItemId`,`licenceId`),
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productId`) REFERENCES `products` (`productId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`ledgerTypeId`) REFERENCES `ledgerTypes` (`ledgerTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
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
  `paymentMethodId`         int(10) unsigned        NOT NULL,
  `units`                   int(11)                 NOT NULL,
  `cash`                    decimal(10,0)           NOT NULL,
  `timeReal`                int(11)                 NOT NULL,
  `timeLeft`                int(11)                 NOT NULL,
  `timeCard`                int(11)                 NOT NULL,
  `timeCash`                int(11)                 NOT NULL,
  `ledgerTime`              timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment`                 text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`ledgerDeviceId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`paymentMethodId`) REFERENCES `paymentMethods` (`paymentMethodId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Konyveles tabla.
-- -----------------------------------------------------------------------------------
CREATE TABLE `ledger` (
  `ledgerId`                int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `parentId`                int(10) unsigned        NOT NULL DEFAULT '0',
  `ledgerTypeId`            int(10) unsigned        NOT NULL,
  `ledgerDeviceId`          int(10) unsigned        NOT NULL,
  `paymentMethodId`         int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `productId`               int(10) unsigned        NOT NULL,
  `patientCardTypeId`       int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `itemCount`               int(11)                 NOT NULL DEFAULT '1',
  `netPrice`                decimal(10,0)           NOT NULL,
  `card`                    decimal(10,0)           NOT NULL,
  `cash`                    decimal(10,0)           NOT NULL,
  `voucher`                 decimal(10,0)           NOT NULL,
  `discount`                decimal(10,0)           NOT NULL,
  `vatpercent`              int(11)                 NOT NULL,
  `totalPrice`              decimal(10,0)           NOT NULL,
  `ledgerTime`              timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment`                 text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`ledgerId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`ledgerTypeId`) REFERENCES `ledgerTypes` (`ledgerTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`ledgerDeviceId`) REFERENCES `ledgerDevice` (`ledgerDeviceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`paymentMethodId`) REFERENCES `paymentMethods` (`paymentMethodId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productId`) REFERENCES `products` (`productId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A studioban forgalmazott termekek esemeny tortenete.
-- -----------------------------------------------------------------------------------
CREATE TABLE `productHistory` (
  `productHistoryId`        int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `productId`               int(10) unsigned        NOT NULL,
  `ledgerId`                int(10) unsigned        NOT NULL,
  `productActionTypeId`     int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `productItemCount`        int(11)                 NOT NULL,
  `netPrice`                decimal(10,0)           NOT NULL,
  `vatpercent`              int(11)                 NOT NULL,
  `actionDateTime`          timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`productHistoryId`),
  FOREIGN KEY (`productId`) REFERENCES `products` (`productId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`ledgerId`) REFERENCES `ledger` (`ledgerId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productActionTypeId`) REFERENCES `productActionType` (`productActionTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- A kliens alkalmazas penzforgalmat tartalmazza.
-- Minden egyes penzforgalom egy kulon rekord.
-- -----------------------------------------------------------------------------------
CREATE TABLE `cassaHistory` (
  `cassaHistoryId`          int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `parentId`                int(10) unsigned        NOT NULL DEFAULT '0',
  `cassaId`                 int(10) unsigned        NOT NULL,
  `ledgerId`                int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `actionValue`             int(11)                 NOT NULL,
  `actionCard`              decimal(10,0)           NOT NULL,
  `actionCash`              decimal(10,0)           NOT NULL,
  `actionBalance`           decimal(10,0)           NOT NULL,
  `actionTime`              timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment`                 text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`cassaHistoryId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`cassaId`) REFERENCES `cassa` (`cassaId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`ledgerId`) REFERENCES `ledger` (`ledgerId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Masodlagos monitoron megjeleno, reklamokat - slide show - es fix kepeket tartalmazo
-- nem modal ablakok tartalmat meghatarozo tabla
-- Minden egyes bejegyzes egy ablakot ir le
-- -----------------------------------------------------------------------------------
CREATE TABLE `advertisements` (
  `advertisementId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `caption`                 varchar(100)            NOT NULL,
  `backgroundColor`         varchar(10)             NOT NULL,
  `path`                    text                    NOT NULL,
  `fileNames`               text                    NOT NULL,
  `timer`                   int(10) unsigned        NOT NULL DEFAULT 0,
  `transparent`             tinyint(1)              DEFAULT 0,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`advertisementId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Varakozo lista
-- -----------------------------------------------------------------------------------
CREATE TABLE `waitlist` (
  `waitlistId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  `ledgerId`                int(10) unsigned        NOT NULL,
  `shoppingCartItemId`      int(10) unsigned        NOT NULL,
  `panelTypeId`             int(10) unsigned        NOT NULL,
  `payType`                 int(10) unsigned        NOT NULL,
  `barcode`                 varchar(20)             NOT NULL,
  `unitIds`                 text                    NOT NULL,
  `lengthCash`              decimal(10,0)           NOT NULL,
  `lengthCard`              decimal(10,0)           NOT NULL,
  `useTime`                 int(11)                 NOT NULL,
  `usePrice`                decimal(10,0)           NOT NULL,
  `comment`                 text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  PRIMARY KEY (`waitlistId`,`licenceId`),
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`ledgerId`) REFERENCES `ledger` (`ledgerId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`shoppingCartItemId`) REFERENCES `shoppingCartItems` (`shoppingCartItemId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelTypeId`) REFERENCES `panelTypes` (`panelTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Email cimlista
-- -----------------------------------------------------------------------------------
CREATE TABLE `distlist` (
  `distlistId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `description`             varchar(500)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`distlistId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Pacienseket es email levelezo listakat osszekoto tabla, levelezo listak kezelesehez
-- -----------------------------------------------------------------------------------
CREATE TABLE `connectPatientWithDistList` (
  `distlistId`              int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `licenceId`               int(10) unsigned        NOT NULL,
  PRIMARY KEY (`distlistId`,`patientId`,`licenceId`),
  FOREIGN KEY (`distlistId`) REFERENCES `distlist` (`distlistId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Email kuldes ideiglenes tablaja. Minden sor egy kikuldendo level.
-- -----------------------------------------------------------------------------------
CREATE TABLE `httpsendmail` (
  `httpSendMailId`          int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `mailTypeId`              int(10) unsigned        NOT NULL,
  `mailDestination`         int(10) unsigned        NOT NULL,
  `dateOfSending`           date                    NOT NULL,
  `recipients`              text                    NOT NULL,
  `subject`                 varchar(500)            NOT NULL,
  `mailbody`                text                    NOT NULL,
  `var_name`                varchar(500)            NOT NULL,
  `var_barcode`             varchar(20)             NOT NULL,
  `var_cardinfo`            text                    NOT NULL,
  `var_unitcount`           varchar(10)             NOT NULL,
  `var_datetime`            varchar(50)             NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,  
  PRIMARY KEY (`httpSendMailId`,`licenceId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- -----------------------------------------------------------------------------------
-- Ideiglenes tabla berlet elettortenethez.
-- -----------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `report_cardhistory` (
  `dateCardAction` 			datetime 				NOT NULL,
  `cardAction` 				varchar(500)            NOT NULL,
  `countUnits`	 			int(11) 				NOT NULL,
  `unitTime`	 			int(11) 				NOT NULL,
  `unitType` 				varchar(500)            NOT NULL,
  `priceAction` 			int(11) 				NOT NULL,
  `userName` 				varchar(500)            NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

