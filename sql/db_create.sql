
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
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`licenceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `logs` (
  `date`                    timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `userId`                  int(10) unsigned        DEFAULT NULL,
  `severity`                int(1)                  NOT NULL DEFAULT 5,
  `threadId`                VARCHAR( 20 )           NOT NULL,
  `message`                 text                    DEFAULT NULL,
  INDEX logs_date(`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `settings` (
  `settingId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `identifier`              varchar(50)             NOT NULL,
  `value`                   text                    NOT NULL,
  UNIQUE (`identifier`),
  PRIMARY KEY (`settingId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
  PRIMARY KEY (`userId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `activateCommand` (
  `activateCommandId`       int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  PRIMARY KEY (`activateCommandId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `patientOrigin` (
  `patientOriginId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientOriginId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `reasonToVisit` (
  `reasonToVisitId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`reasonToVisitId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `illnessGroups` (
  `illnessGroupId`          int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`illnessGroupId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `publicPlaces` (
  `publicPlaceId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        DEFAULT 0,
  `name`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`publicPlaceId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `healthInsurances` (
  `healthInsuranceId`       int(10) unsigned        NOT NULL AUTO_INCREMENT,
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
  PRIMARY KEY (`healthInsuranceId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
  PRIMARY KEY (`companyId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `doctorTypes` (
  `doctorTypeId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`doctorTypeId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `doctors` (
  `doctorId`                int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `doctorTypeId`            int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `doctorLicence`           varchar(50)             NOT NULL,
  `data`                    text                    NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`doctorId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`doctorTypeId`) REFERENCES `doctorTypes` (`doctorTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `patients` (
  `patientId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientOriginId`         int(10) unsigned        NOT NULL,
  `reasonToVisitId`         int(10) unsigned        NOT NULL,
  `illnessGroupId`          int(10) unsigned        NOT NULL,
  `healthInsuranceId`       int(10) unsigned        NOT NULL,
  `companyId`               int(10) unsigned        NOT NULL,
  `doctorId`                int(10) unsigned        NOT NULL,
  `created`                 datetime                NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `gender`                  int(11)                 DEFAULT NULL,
  `dateBirth`               date                    DEFAULT NULL,
  `uniqueId`                varchar(20)             DEFAULT NULL,
  `email`                   varchar(100)            DEFAULT NULL,
  `phone`                   varchar(100)            DEFAULT NULL,
  `weight`                  int(11)                 DEFAULT NULL,
  `height`                  int(11)                 DEFAULT NULL,
  `illnesses`               varchar(500)            DEFAULT NULL,
  `symptoms`                varchar(500)            DEFAULT NULL,
  `medicineCurrent`         varchar(500)            DEFAULT NULL,
  `medicineAllergy`         varchar(500)            DEFAULT NULL,
  `regularCustomer`         tinyint(1)              DEFAULT 0,
  `employee`                tinyint(1)              DEFAULT 0,
  `service`                 tinyint(1)              DEFAULT 0,
  `healthInsurance`         tinyint(1)              DEFAULT 0,
  `company`                 tinyint(1)              DEFAULT 0,
  `doctorProposed`          tinyint(1)              DEFAULT 0,
  `discountType`            int(11)                 NOT NULL,
  `comment`                 text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientOriginId`) REFERENCES `patientOrigin` (`patientOriginId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`reasonToVisitId`) REFERENCES `reasonToVisit` (`reasonToVisitId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`illnessGroupId`) REFERENCES `illnessGroups` (`illnessGroupId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`healthInsuranceId`) REFERENCES `healthInsurances` (`healthInsuranceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`companyId`) REFERENCES `companies` (`companyId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`doctorId`) REFERENCES `doctors` (`doctorId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `attendance` (
  `attendanceId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `date`                    date                    NOT NULL,
  `length`                  int(10) unsigned        DEFAULT NULL,
  `bloodPressureStart`      float                   DEFAULT NULL,
  `pulseStart`              float                   DEFAULT NULL,
  `bloodPressureStop`       float                   DEFAULT NULL,
  `pulseStop`               float                   DEFAULT NULL,
  `comment`                 text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`attendanceId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `toBeFilled` (
  `toBeFilledId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `attendanceId`            int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  PRIMARY KEY (`toBeFilledId`),
  FOREIGN KEY (`attendanceId`) REFERENCES `attendance` (`attendanceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientCardTypeId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `patientCards` (
  `patientCardId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardTypeId`       int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `barcode`                 varchar(20)             NOT NULL,
  `comment`                 varchar(50)             DEFAULT NULL,
  `units`                   int(11)                 NOT NULL,
  `timeLeft`                int(10) unsigned        NOT NULL,
  `validDateFrom`           date                    NOT NULL,
  `validDateTo`             date                    NOT NULL,
  `pincode`                 varchar(5)              DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientCardId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `connectPatientWithCard` (
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `modified`                datetime                NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`licenceID`,`patientCardId`,`patientId`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `patientCardHistories` (
  `patientCardHistoryId`    int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  `dateTimeUsed`            timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `units`                   int(11)                 NOT NULL,
  `time`                    time                    NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`patientCardHistoryId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `panelTypes` (
  `panelTypeId`             int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelTypeId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `panelStatuses` (
  `panelStatusId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `panelTypeId`             int(10) unsigned        NOT NULL,
  `seqNumber`               int(10) unsigned        NOT NULL,
  `name`                    varchar(20)             NOT NULL,
  `length`                  int(10) unsigned        NOT NULL DEFAULT 0,
  `activateCmd`             int(10) unsigned        NOT NULL DEFAULT 0,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelStatusId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelTypeId`) REFERENCES `panelTypes` (`panelTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
  PRIMARY KEY (`panelStatusSettingId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelStatusId`) REFERENCES `panelStatuses` (`panelStatusId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `panels` (
  `panelId`                 int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `panelTypeId`             int(10) unsigned        NOT NULL,
  `title`                   varchar(50)             NOT NULL,
  `workTime`                int(10) unsigned        NOT NULL DEFAULT 0,
  `maxWorkTime`             int(10) unsigned        NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelTypeId`) REFERENCES `panelTypes` (`panelTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `panelUses` (
  `panelUseId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `useTime`                 int(11)                 NOT NULL,
  `usePrice`                int(11)                 NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`panelUseId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `attendanceSchedule` (
  `attendanceScheduleId`    int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `dayOfWeek`               int(11)                 NOT NULL,
  `timeFrom`                time                    DEFAULT NULL,
  `timeTo`                  time                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`attendanceScheduleId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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

CREATE TABLE `ledgerTypes` (
  `ledgerTypeId`            int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`ledgerTypeId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `productTypes` (
  `productTypeId`           int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`productTypeId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `products` (
  `productId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `productTypeId`           int(11) unsigned        NOT NULL REFERENCES `productTypes` (`productTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  `name`                    varchar(50)             NOT NULL,
  `netPrice`                int(11)                 NOT NULL,
  `vatpercent`              int(11)                 NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`productId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productTypeId`) REFERENCES `productTypes` (`productTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `discounts` (
  `discountId`              int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `healthInsuranceId`       int(10) unsigned        DEFAULT NULL,
  `companyId`               int(10) unsigned        DEFAULT NULL,
  `doctorId`                int(10) unsigned        DEFAULT NULL,
  `regularCustomer`         tinyint(1)              DEFAULT 0,
  `employee`                tinyint(1)              DEFAULT 0,
  `service`                 tinyint(1)              DEFAULT 0,
  `name`                    varchar(100)            NOT NULL,
  `discountValue`           int(11)                 NOT NULL,
  `discountPercent`         int(11)                 NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`discountId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`healthInsuranceId`) REFERENCES `healthInsurances` (`healthInsuranceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`doctorId`) REFERENCES `doctors` (`doctorId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`companyId`) REFERENCES `companies` (`companyId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `zipRegionCity` (
  `zipRegionCityId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `zip`                     varchar(100)            NOT NULL,
  `region`                  varchar(100)            NOT NULL,
  `city`                    varchar(100)            NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1)              DEFAULT 0,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`zipRegionCityId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `address` (
  `addressId`               int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
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
  PRIMARY KEY (`addressId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`publicPlaceId`) REFERENCES `publicPlaces` (`publicPlaceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `cassa` (
  `cassaId`                 int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `currentBalance`          int(11)                 NOT NULL,
  `startDateTime`           datetime                NOT NULL,
  `stopDateTime`            datetime                NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`cassaId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `cassaHistory` (
  `cassaHistoryId`          int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `cassaId`                 int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `actionValue`             int(11)                 NOT NULL,
  `actionBalance`           int(11)                 NOT NULL,
  `actionTime`              timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment`                 text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`cassaHistoryId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`cassaId`) REFERENCES `cassa` (`cassaId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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

CREATE TABLE `paymentMethods` (
  `paymentMethodId`         int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `name`                    varchar(50)             NOT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`paymentMethodId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `ledgerDevice` (
  `ledgerDeviceId`          int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `patientId`               int(10) unsigned        NOT NULL,
  `paymentMethodId`         int(10) unsigned        NOT NULL,
  `units`                   int(11)                 NOT NULL,
  `cash`                    int(11)                 NOT NULL,
  `timeReal`                int(11)                 NOT NULL,
  `timeLeft`                int(11)                 NOT NULL,
  `timeCard`                int(11)                 NOT NULL,
  `timeCash`                int(11)                 NOT NULL,
  `ledgerTime`              timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment`                 text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`ledgerDeviceId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientId`) REFERENCES `patients` (`patientId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`paymentMethodId`) REFERENCES `paymentMethods` (`paymentMethodId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `ledger` (
  `ledgerId`                int(10) unsigned        NOT NULL AUTO_INCREMENT,
  `licenceId`               int(10) unsigned        NOT NULL,
  `ledgerTypeId`            int(10) unsigned        NOT NULL,
  `ledgerDeviceId`          int(10) unsigned        NOT NULL,
  `paymentMethodId`         int(10) unsigned        NOT NULL,
  `userId`                  int(10) unsigned        NOT NULL,
  `productId`               int(10) unsigned        NOT NULL,
  `patientCardTypeId`       int(10) unsigned        NOT NULL,
  `patientCardId`           int(10) unsigned        NOT NULL,
  `panelId`                 int(10) unsigned        NOT NULL,
  `name`                    varchar(100)            NOT NULL,
  `netPrice`                int(11)                 NOT NULL,
  `discount`                int(11)                 NOT NULL,
  `vatpercent`              int(11)                 NOT NULL,
  `totalPrice`              int(11)                 NOT NULL,
  `ledgerTime`              timestamp               NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment`                 text                    DEFAULT NULL,
  `modified`                datetime                NOT NULL,
  `active`                  tinyint(1) unsigned     NOT NULL,
  `archive`                 varchar(10)             NOT NULL,
  PRIMARY KEY (`ledgerId`,`licenceID`),
  FOREIGN KEY (`licenceId`) REFERENCES `licences` (`licenceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`ledgerTypeId`) REFERENCES `ledgerTypes` (`ledgerTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`ledgerDeviceId`) REFERENCES `ledgerDevice` (`ledgerDeviceId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`paymentMethodId`) REFERENCES `paymentMethods` (`paymentMethodId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`userId`) REFERENCES `users` (`userId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`productId`) REFERENCES `products` (`productId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardTypeId`) REFERENCES `patientCardTypes` (`patientCardTypeId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`patientCardId`) REFERENCES `patientCards` (`patientCardId`) ON UPDATE CASCADE ON DELETE RESTRICT,
  FOREIGN KEY (`panelId`) REFERENCES `panels` (`panelId`) ON UPDATE CASCADE ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

