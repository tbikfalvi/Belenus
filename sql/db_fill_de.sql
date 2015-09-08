-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2013
-- -----------------------------------------------------------------------------------
-- Filename    : db_fill_de.sql
-- AppVersion  : 1.5.2.1
-- DbVersion   : 1.7.0
-- Author      : Bikfalvi Tamas
-- -----------------------------------------------------------------------------------
-- Adatbazist default adatokkal feltolto SQL script
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

INSERT INTO `licences` (`licenceId`, `serial`, `country`, `region`, `city`, `zip`, `address`, `studio`, `contact`, `lastValidated`, `active`, `archive`) VALUES
 ( 0, 'BLNS_SERIAL_GLOBAL', 'Hungary', 'Budapest', 'Budapest', '', '', 'KiwiSun Franchise', NULL, '2013-10-01', 1, 'ARC');
INSERT INTO `licences` (`licenceId`, `serial`, `country`, `region`, `city`, `zip`, `address`, `studio`, `contact`, `lastValidated`, `active`, `archive`) VALUES
 ( 1, 'BLNS_SERIAL_DEMO', 'Hungary', 'Budapest', 'Budapest', '', '', 'KiwiSun Franchise', NULL, '2013-10-01', 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `accgroup`, `active`, `comment`, `archive`) VALUES
 ( 0, 'guest', 'guest', '', 1, 0, '', 'ARC' );
UPDATE `users` SET `userId`=0 WHERE `userId`=1;
ALTER TABLE `users` auto_increment=1;

INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `accgroup`, `active`, `comment`, `archive`) VALUES
 ( 1, 'system', 'System Administrator', 'a382329cfe97ae74677649d1f7fc03986b27cf3f', 3, 1, 'Default system administrator with extended administrator rights.', 'ARC' );
INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `accgroup`, `active`, `comment`, `archive`) VALUES
 ( 1, 'admin', 'Administrator', 'd033e22ae348aeb5660fc2140aec35850c4da997', 2, 1, 'Default administrator with basic administrator rights.', 'ARC' );
INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `accgroup`, `active`, `comment`, `archive`) VALUES
 ( 1, 'operator', 'Operator', 'e47c3168137789e3e0df75d23452cabcc9a890f5', 1, 1, 'Default user with operator rights.', 'ARC' );

-- -----------------------------------------------------------------------------------

INSERT INTO `activateCommand` (`activateCommandId`, `name`) VALUES
 (0, 'Default status'),
 (1, 'Preparation'),
 (2, 'Sauna'),
 (3, 'Browning'),
 (4, 'Cooling'),
 (9, 'Waiting');

-- -----------------------------------------------------------------------------------

INSERT INTO  `publicPlaces` (`publicPlaceId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '', 0, 'ARC');
UPDATE `publicPlaces` SET `publicPlaceId`=0 WHERE `publicPlaceId`=1;
ALTER TABLE `publicPlaces` auto_increment=1;
INSERT INTO `publicPlaces` (`publicPlaceId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, 0, 'street', 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO  `companies` (`companyId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '', 0, 'ARC');
UPDATE `companies` SET `companyId`=0 WHERE `companyId`=1;
ALTER TABLE `companies` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `agetypes` (`ageTypeId`, `licenceId`, `ageTypeName` ) VALUES
 ('0', '0', 'Not defined');
UPDATE `agetypes` SET `ageTypeId`=0 WHERE `ageTypeId`=1;
ALTER TABLE `agetypes` auto_increment=1;

INSERT INTO `agetypes` (`ageTypeId`, `licenceId`, `ageTypeName` ) VALUES
 ('1', '0', 'Younger than 18'),
 ('2', '0', '18 - 20'),
 ('3', '0', '21 - 30'),
 ('4', '0', '31 - 40'),
 ('5', '0', '41 - 50'),
 ('6', '0', '51 - 60'),
 ('7', '0', 'Above 60');

-- -----------------------------------------------------------------------------------

INSERT INTO `genders` (`genderId`, `licenceId`, `genderName` ) VALUES
 ('0', '0', 'Not defined');
UPDATE `genders` SET `genderId`=0 WHERE `genderId`=1;
ALTER TABLE `genders` auto_increment=1;

INSERT INTO `genders` (`genderId`, `licenceId`, `genderName` ) VALUES
 ('1', '0', 'Male'),
 ('2', '0', 'Female');

-- -----------------------------------------------------------------------------------

INSERT INTO `skinTypes` (`skinTypeId`, `licenceId`, `skinTypeName`, `active`, `archive` ) VALUES
 ('0', '0', 'Not defined', 1, 'ARC');
UPDATE `skinTypes` SET `skinTypeId`=0 WHERE `skinTypeId`=1;
ALTER TABLE `skinTypes` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `patients` (`patientId`, `licenceId`, `companyId`, `created`, `name`, `gender`, `ageType`, `isReturning`, `uniqueId`, `email`, `regularCustomer`, `employee`, `service`, `company`, `discountType`, `comment`, `loyaltyPoints`, `modified`, `active`, `archive`) VALUES
 ('0', '0', '0', '', '', '0', '0', '0', NULL, NULL, '0', '0', '0', '0', '', NULL, 0, '', '0', 'ARC');
UPDATE `patients` SET `patientId`=0 WHERE `patientId`=1;
ALTER TABLE `patients` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `vatpercent`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `active`, `archive`) VALUES
 (0, 0, '', 0, 0, 0, '2012-01-01', '2012-01-01', 0, 0, 0, 'ARC');
UPDATE `patientCardTypes` SET `patientCardTypeId`=0 WHERE `patientCardTypeId`=1;
ALTER TABLE `patientCardTypes` auto_increment=1;

INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `vatpercent`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `active`, `archive`) VALUES
 (1, 0, 'Service cards', 0, 0, 999, '2012-01-01', '2100-12-31', 0, 1, 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `patientCards` (`patientCardId`, `licenceId`, `patientCardTypeId`, `patientId`, `barcode`, `comment`, `units`, `timeLeft`, `validDateFrom`, `validDateTo`, `pincode`, `active`, `archive`) VALUES
 (0, 0, 0, 0, '', NULL, 0, 0, '0000-00-00', '0000-00-00', NULL, 0, 'ARC');
UPDATE `patientCards` SET `patientCardId`=0 WHERE `patientCardId`=1;
ALTER TABLE `patientCards` auto_increment=1;

INSERT INTO `patientCards` (`patientCardId`, `licenceId`, `patientCardTypeId`, `patientId`, `barcode`, `comment`, `units`, `timeLeft`, `validDateFrom`, `validDateTo`, `pincode`, `active`, `archive`) VALUES
 (1, 0, 1, 0, '000000', 'Szerviz kártya. Csak szerviz használatra', 999, 43200, '2012-01-01', '2100-12-31', NULL, 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `panelTypes` ( `panelTypeId`, `licenceId`, `name`, `active`, `archive` ) VALUES
  ( 0, 0, "", 0, "ARC" );
UPDATE `panelTypes` SET `panelTypeId`='0' WHERE `panelTypeId`=1;
ALTER TABLE `panelTypes` auto_increment=1;

INSERT INTO `panelTypes` ( `panelTypeId`, `licenceId`, `name`, `active`, `archive` ) VALUES
 ( 1, 0, "KiwiSun Solarium", 1, "ARC" ),
 ( 2, 0, "KiwiSun Infrasolarium", 1, "ARC" );

-- -----------------------------------------------------------------------------------

INSERT INTO `panelStatuses` ( `licenceId`, `panelTypeId`, `seqNumber`, `name`, `length`, `activateCmd`, `skipLevel`, `allowedToSkip`, `active`, `archive` ) VALUES
 ( 0, 1, 1, "Base status", 0, 0, 4, 0, 1, "ARC" ),
 ( 0, 1, 2, "Preparation/Undressing", 60, 1, 1, 1, 1, "ARC" ),
 ( 0, 1, 3, "Browning", 0, 3, 4, 0, 1, "ARC" ),
 ( 0, 1, 4, "Cooling", 300, 4, 4, 0, 1, "ARC" ),
 ( 0, 2, 1, "Base status", 0, 0, 4, 0, 1, "ARC" ),
 ( 0, 2, 2, "Preparation/Undressing", 60, 1, 1, 1, 1, "ARC" ),
 ( 0, 2, 3, "Sauna", 120, 2, 4, 0, 1, "ARC" ),
 ( 0, 2, 4, "Waiting", 3, 9, 4, 0, 1, "ARC" ),
 ( 0, 2, 5, "Browning", 0, 3, 4, 0, 1, "ARC" ),
 ( 0, 2, 6, "Cooling", 300, 4, 4, 0, 1, "ARC" );

-- -----------------------------------------------------------------------------------

INSERT INTO `panelstatussettings` (`panelStatusSettingId`, `licenceId`, `panelStatusId`, `backgroundColor`, `statusFontName`, `statusFontSize`, `statusFontColor`, `timerFontName`, `timerFontSize`, `timerFontColor`, `nextFontName`, `nextFontSize`, `nextFontColor`, `infoFontName`, `infoFontSize`, `infoFontColor`, `modified`, `active`, `archive`) VALUES
 (1, 1, 1, '#00ff00', 'Arial', 18, '#000000', 'Book Antiqua', 30, '#000000', 'Arial', 18, '#000000', 'Arial', 10, '#000000', '2013-01-01 12:00:00', 1, 'NEW'),
 (2, 1, 2, '#ffff00', 'Arial', 18, '#000000', 'Book Antiqua', 30, '#000000', 'Arial', 18, '#000000', 'Arial', 10, '#000000', '2013-01-01 12:00:00', 1, 'NEW'),
 (3, 1, 3, '#ff0000', 'Arial', 18, '#000000', 'Book Antiqua', 30, '#000000', 'Arial', 18, '#000000', 'Arial', 10, '#000000', '2013-01-01 12:00:00', 1, 'NEW'),
 (4, 1, 4, '#ffff7f', 'Arial', 18, '#000000', 'Book Antiqua', 30, '#000000', 'Arial', 18, '#000000', 'Arial', 10, '#000000', '2013-01-01 12:00:00', 1, 'NEW'),
 (5, 1, 5, '#00aa00', 'Arial', 18, '#000000', 'Book Antiqua', 30, '#000000', 'Arial', 18, '#000000', 'Arial', 10, '#000000', '2013-01-01 12:00:00', 1, 'NEW'),
 (6, 1, 6, '#ffff00', 'Arial', 18, '#000000', 'Book Antiqua', 30, '#000000', 'Arial', 18, '#000000', 'Arial', 10, '#000000', '2013-01-01 12:00:00', 1, 'NEW'),
 (7, 1, 7, '#aa0000', 'Arial', 18, '#000000', 'Book Antiqua', 30, '#000000', 'Arial', 18, '#000000', 'Arial', 10, '#000000', '2013-01-01 12:00:00', 1, 'NEW'),
 (8, 1, 8, '#ffff00', 'Arial', 18, '#000000', 'Book Antiqua', 30, '#000000', 'Arial', 18, '#000000', 'Arial', 10, '#000000', '2013-01-01 12:00:00', 1, 'NEW'),
 (9, 1, 9, '#ff0000', 'Arial', 18, '#000000', 'Book Antiqua', 30, '#000000', 'Arial', 18, '#000000', 'Arial', 10, '#000000', '2013-01-01 12:00:00', 1, 'NEW'),
 (10, 1, 10, '#ffff7f', 'Arial', 18, '#000000', 'Book Antiqua', 30, '#000000', 'Arial', 18, '#000000', 'Arial', 10, '#000000', '2013-01-01 12:00:00', 1, 'NEW');

-- -----------------------------------------------------------------------------------

INSERT INTO `panelgroups` (`panelGroupId`, `licenceId`, `name`, `description`, `modified`, `active`, `archive`) VALUES
(0, 0, '<No group associated>', NULL, '0000-00-00 00:00:00', 1, 'ARC');
UPDATE `panelgroups` SET `panelGroupId`='0' WHERE `panelGroupId`=1;
ALTER TABLE `panelgroups` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `panels` ( `panelId`, `licenceId`, `panelTypeId`, `panelGroupId`, `title`, `workTime`, `maxWorkTime`, `active`, `archive` ) VALUES
  ( 0, 0, 0, 0, "", 0, 0, 0, "ARC" );
UPDATE `panels` SET `panelId`='0' WHERE `panelId`=1;
ALTER TABLE `panels` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `panelUses` (`licenceId`, `panelId`, `name`, `useTime`, `usePrice`, `active`, `archive`) VALUES
  ('0', '0', '', '0', '0', '0', 'ARC');
UPDATE `panelUses` SET `panelUseId`='0' WHERE `panelUseId`=1;
ALTER TABLE `panelUses` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `patientcardunits` (`patientCardUnitId` ,`licenceId` ,`patientCardId` ,`ledgerId` ,`panelId` ,`unitTime` ,`unitPrice` ,`validDateFrom` ,`validDateTo` ,`dateTimeUsed` ,`active` ,`archive` ) VALUES
 ('0',  '0', '1', '0', '0', '5', '0', '2013-01-01', '2100-12-31', CURRENT_TIMESTAMP , '1', 'ARC');
UPDATE `patientcardunits` SET `patientCardUnitId`='0' WHERE `patientCardUnitId`=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `patientcardtypeenabled` (`patientCardTypeEnabledId`, `licenceId`, `patientCardTypeId`, `validWeekDays`, `start`, `stop`, `modified`, `archive`) VALUES
 ('0', '0', '1', '127', '00:00:00', '23:59:59', '2013-01-01 00:00:00', 'ARC');
UPDATE `patientcardtypeenabled` SET `patientCardTypeEnabledId`='0' WHERE `patientCardTypeEnabledId`=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `denominations` (`denominationId`, `licenceId`, `denomination`, `comment`, `active`, `archive`) VALUES
 (1, 0, 5, "", 1, 'ARC'),
 (2, 0, 10, "", 1, 'ARC'),
 (3, 0, 20, "", 1, 'ARC'),
 (4, 0, 50, "", 1, 'ARC'),
 (5, 0, 100, "", 1, 'ARC'),
 (6, 0, 200, "", 1, 'ARC'),
 (7, 0, 500, "", 1, 'ARC'),
 (8, 0, 1000, "", 1, 'ARC'),
 (9, 0, 2000, "", 1, 'ARC'),
 (10, 0, 5000, "", 1, 'ARC'),
 (11, 0, 10000, "", 1, 'ARC'),
 (12, 0, 20000, "", 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `ledgerTypes` (`ledgerTypeId` ,`licenceId` ,`name` ,`active` ,`archive`) VALUES
 (0 , '0', '', '0', 'ARC');
UPDATE `ledgerTypes` SET `ledgerTypeId`=0 WHERE `ledgerTypeId`=1;
ALTER TABLE `ledgerTypes` auto_increment=1;

INSERT INTO `ledgerTypes` (`ledgerTypeId` ,`licenceId` ,`name` ,`active` ,`archive`) VALUES
 (1 , '0', 'Using solarium', '1', 'ARC'),
 (2 , '0', 'Selling patientcard', '1', 'ARC'),
 (3 , '0', 'Filling patientcard', '1', 'ARC'),
 (4 , '0', 'Selling product', '1', 'ARC'),
 (5 , '0', 'Replacing lost patientcard', '1', 'ARC'),
 (6 , '0', 'Assign patientcard', '1', 'ARC'),
 (7 , '0', 'Other', '1', 'ARC'),
 (8 , '0', 'Other', '1', 'ARC'),
 (9 , '0', 'Storage action', '1', 'ARC'),
 (10, '0', 'Casssa expense', '1', 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `productTypes` (`productTypeId`, `licenceId`, `name`, `active`, `archive`) VALUES
 ('0', '0', '', '0', 'ARC');
UPDATE `productTypes` SET `productTypeId`=0 WHERE `productTypeId`=1;
ALTER TABLE `productTypes` auto_increment=1;

-- -----------------------------------------------------------------------------------
INSERT INTO `products` (`productId`, `licenceId`, `name`, `barcode`, `netPriceBuy`, `vatpercentBuy`, `netPriceSell`, `vatpercentSell`, `productCount`, `modified`, `active`, `archive`) VALUES
 ('0', '0', '', '', '0', '0', '0', '0', '0', '', '0', 'ARC');
UPDATE `products` SET `productId`='0' WHERE `productId`=1;
ALTER TABLE `products` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `productActionType` (`productActionTypeId`, `name`, `increaseProductCount`, `decreaseProductCount`, `licenceId`, `active`, `archive` ) VALUES
 ('0', '', '0', '0', '0', 0, "ARC" );
UPDATE `productActionType` SET `productActionTypeId`='0' WHERE `productActionTypeId`=1;
ALTER TABLE `productActionType` auto_increment=1;

INSERT INTO `productactiontype` (`productActionTypeId`, `licenceId`, `name`, `increaseProductCount`, `decreaseProductCount`, `cassaActionIndication`, `modified`, `active`, `archive`) VALUES
 (1, 0, 'Add product to storage', 1, 0, 'Negative', '2013-09-08 17:38:16', 1, 'NEW'),
 (2, 0, 'Product disposal', 0, 1, 'Positive', '2013-09-08 17:38:33', 1, 'NEW'),
 (3, 0, 'Product donate', 0, 1, 'Positive', '2013-09-08 17:38:46', 1, 'NEW');

-- -----------------------------------------------------------------------------------

INSERT INTO `discounts` (`discountId`, `licenceId`, `patientId`, `companyId`, `paymentMethodId`, `productId`, `regularCustomer`, `employee`, `service`, `name`, `discountValue`, `discountPercent`, `modified`, `active`, `archive`) VALUES
 (NULL, '0', NULL, NULL, NULL, NULL, '0', '0', '0', '', '', '', '', '0', 'ARC');
UPDATE `discounts` SET `discountId`='0' WHERE `discountId`=1;
ALTER TABLE `discounts` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `address` (`addressId`, `licenceId`, `publicPlaceId`, `name`, `country`, `region`, `city`, `zip`, `street`, `streetNumber`, `floor`, `door`, `primaryAddress`, `modified`, `active`, `archive`) VALUES
 (NULL, '0', '0', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '', '0', 'ARC');
UPDATE `address` SET `addressId`='0' WHERE `addressId`=1;
ALTER TABLE `address` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `cassa` (`cassaId`, `licenceId`, `userId`, `currentBalance`, `startDateTime`, `stopDateTime`, `modified`, `active`, `archive`) VALUES
 ('0', '0', '0', '0', '', '', '', '1', 'NEW');
UPDATE `cassa` SET `cassaId`='0' WHERE `cassaId`=1;
ALTER TABLE `cassa` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `paymentMethods` (`paymentMethodId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (0, 0, '', 0, 'ARC');
UPDATE `paymentMethods` SET `paymentMethodId`=0 WHERE `paymentMethodId`=1;
ALTER TABLE `paymentMethods` auto_increment=1;

INSERT INTO `paymentMethods` (`paymentMethodId`, `licenceId`, `name`, `active`, `archive`) VALUES
 (1, 0, 'Cash', 1, 'ARC'),
 (2, 0, 'Creditcard', 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `ledgerDevice` (`ledgerDeviceId`, `licenceId`, `userId`, `panelId`, `patientId`, `paymentMethodId`, `units`, `cash`, `timeReal`, `timeLeft`, `timeCard`, `timeCash`, `ledgerTime`, `comment`, `active`, `archive`) VALUES
 (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '2012-11-02 08:00:00', NULL, 0, 'ARC');
UPDATE `ledgerDevice` SET `ledgerDeviceId`=0 WHERE `ledgerDeviceId`=1;
ALTER TABLE `ledgerDevice` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `ledger` (`ledgerId`, `licenceId`, `parentId` , `ledgerTypeId` , `ledgerDeviceId` , `paymentMethodId` , `userId` , `productId` , `patientCardTypeId` , `patientCardId` , `panelId` , `patientId` , `name` , `netPrice` , `voucher` , `discount` , `vatpercent` , `totalPrice` , `ledgerTime` , `comment` , `modified` , `active` , `archive` ) VALUES
 ( 0 ,  '0',  '0',  '0',  '0',  '0',  '0',  '0',  '0',  '0',  '0',  '0',  '',  '0', '0', '0',  '0',  '0', CURRENT_TIMESTAMP , NULL ,  '',  '0',  'ARC' );
UPDATE `ledger` SET `ledgerId`=0 WHERE `ledgerId`=1;
ALTER TABLE `ledger` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `patienthistorytype` (`patientHistoryTypeId`, `licenceId`, `name`, `modified`, `active`, `archive`) VALUES
 (1, 0, 'Guest entered into database', '0000-00-00 00:00:00', 1, 'ARC'),
 (2, 0, 'Purchase patientcard', '0000-00-00 00:00:00', 1, 'ARC'),
 (3, 0, 'Refill patientcard', '0000-00-00 00:00:00', 1, 'ARC'),
 (4, 0, 'Purchase product', '0000-00-00 00:00:00', 1, 'ARC'),
 (5, 0, 'Using device with card', '0000-00-00 00:00:00', 1, 'ARC'),
 (6, 0, 'Using device with cash', '0000-00-00 00:00:00', 1, 'ARC');

-- -----------------------------------------------------------------------------------

INSERT INTO `shoppingcartitems` ( `shoppingCartItemId`, `licenceId`, `patientId`, `productId`, `patientCardId`, `patientCardTypeId`, `panelId`, `ledgerTypeId`, `itemName`, `comment`, `itemCount`, `itemNetPrice`, `card`, `cash`, `voucher`, `itemVAT`, `discountValue`, `itemSumPrice`, `modified`, `archive` ) VALUES
( 0, '0', '0', '0', '0', NULL, '0', '0', '', NULL, '', '', '', '', '', '', '', '', '', 'ARC' );
UPDATE `shoppingcartitems` SET `shoppingCartItemId`=0 WHERE `shoppingCartItemId`=1;
ALTER TABLE `shoppingcartitems` auto_increment=1;

-- -----------------------------------------------------------------------------------

INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES
 (NULL, 'GLOBAL_DATA_UPDATED', '2015-01-01 08:00:00'),
 (NULL, 'ABOUT_INFO_LINK', 'http://www.kiwisun.eu/elerhetosegeink.html'),
 (NULL, 'APPLICATION_VERSION', '1_5_2_1')
 (NULL, 'DATABASE_VERSION', '1_7_0');

-- -----------------------------------------------------------------------------------
