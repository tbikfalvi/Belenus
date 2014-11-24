-- -----------------------------------------------------------------------------------
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2014
-- -----------------------------------------------------------------------------------
-- Filename    : db_update_1_4_11.sql
-- AppVersion  : 1.4.11
-- DbVersion   : 1.6.1
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `waitlist` ADD `shoppingCartItemId` INT UNSIGNED NOT NULL AFTER `ledgerId`;
ALTER TABLE `waitlist` ADD INDEX `shoppingCartItemId` ( `shoppingCartItemId` );
ALTER TABLE `waitlist` ADD FOREIGN KEY (`shoppingCartItemId`) REFERENCES `shoppingCartItems` (`shoppingCartItemId`) ON UPDATE CASCADE;

ALTER TABLE `shoppingcartitems` auto_increment=1;
INSERT INTO `shoppingcartitems` ( `shoppingCartItemId`, `licenceId`, `patientId`, `productId`, `patientCardId`, `patientCardTypeId`, `panelId`, `ledgerTypeId`, `itemName`, `comment`, `itemCount`, `itemNetPrice`, `card`, `cash`, `voucher`, `itemVAT`, `discountValue`, `itemSumPrice`, `modified`, `archive` ) VALUES
( 0, '0', '0', '0', '0', NULL, '0', '0', '', NULL, '', '', '', '', '', '', '', '', '', 'ARC' );
UPDATE `shoppingcartitems` SET `shoppingCartItemId`=0 WHERE `shoppingCartItemId`=1;
ALTER TABLE `shoppingcartitems` auto_increment=1;

-- -----------------------------------------------------------------------------------

UPDATE settings SET value=NOW() WHERE identifier="GLOBAL_DATA_UPDATED";
UPDATE settings SET value='1_4_11' WHERE identifier='APPLICATION_VERSION';
UPDATE settings SET value='1_6_1' WHERE identifier='DATABASE_VERSION';
