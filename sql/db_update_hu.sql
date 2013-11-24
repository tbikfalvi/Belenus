-- -----------------------------------------------------------------------------------
--
-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2013
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : db_update_hu.sql
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
ALTER TABLE `cassahistory` ADD `actionCard` DECIMAL NOT NULL AFTER `actionValue`, ADD `actionCash` DECIMAL NOT NULL AFTER `actionCard`;

-- -----------------------------------------------------------------------------------

INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES
 (NULL, 'APPLICATION_VERSION', '1_0_0'),
 (NULL, 'DATABASE_VERSION', '1_0');

-- -----------------------------------------------------------------------------------

UPDATE `paymentmethods` SET name='Készpénz' WHERE paymentMethodId=1;
UPDATE `paymentmethods` SET name='Bankkártya' WHERE paymentMethodId=2;

-- -----------------------------------------------------------------------------------
