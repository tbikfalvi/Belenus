-- -----------------------------------------------------------------------------------
--
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2013
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : db_update_us.sql
-- AppVersion  : 1.0.0
-- DbVersion   : 1.0
-- FileVersion : 1.0
-- Author      : Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- 0. adatbazis verziot frissito file
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

ALTER TABLE `ledger` ADD `card` DECIMAL NOT NULL AFTER `netPrice`, ADD `cash` DECIMAL NOT NULL AFTER `card`;
ALTER TABLE `shoppingCartItems` ADD `card` DECIMAL NOT NULL AFTER `itemNetPrice`, ADD `cash` DECIMAL NOT NULL AFTER `card`;

-- -----------------------------------------------------------------------------------

INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES
 (NULL, 'APPLICATION_VERSION', '1_0_0'),
 (NULL, 'DATABASE_VERSION', '1_0');

-- -----------------------------------------------------------------------------------

UPDATE `paymentmethods` SET name='Cash' WHERE paymentMethodId=1;
UPDATE `paymentmethods` SET name='Credit card' WHERE paymentMethodId=2;

-- -----------------------------------------------------------------------------------
