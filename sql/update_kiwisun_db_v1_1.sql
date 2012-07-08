-- -----------------------------------------------------------------------------------
--
-- Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2012
--
-- -----------------------------------------------------------------------------------
--
-- Filename    : update_kiwisun_db_v1_1.sql
-- AppVersion  : 1.0
-- FileVersion : 1.0
-- Author      : Bikfalvi Tamas
--
-- -----------------------------------------------------------------------------------
-- Adatbazist frissito SQL script
-- -----------------------------------------------------------------------------------

USE `belenus`; 

-- -----------------------------------------------------------------------------------
-- Letezo tablak frissitese, kiegeszitese
-- -----------------------------------------------------------------------------------

ALTER TABLE ledger ADD parentId INT UNSIGNED NOT NULL DEFAULT '0' AFTER licenceId;
ALTER TABLE cassahistory ADD ledgerId INT UNSIGNED NOT NULL AFTER cassaId;

INSERT INTO ledger ( `ledgerId`, `licenceId`, `parentId` , `ledgerTypeId` , `ledgerDeviceId` , `paymentMethodId` , `userId` , `productId` , `patientCardTypeId` , `patientCardId` , `panelId` , `patientId` , `name` , `netPrice` , `discount` , `vatpercent` , `totalPrice` , `ledgerTime` , `comment` , `modified` , `active` , `archive` ) VALUES 
 ( 999 ,  '1',  '0',  '5',  '0',  '0',  '0',  '0',  '0',  '0',  '0',  '0',  '',  '0',  '0',  '0',  '0', CURRENT_TIMESTAMP , NULL ,  '',  '0',  'ARC' );
UPDATE `ledger` SET `ledgerId`='0' WHERE `ledgerId`=999;

ALTER TABLE cassahistory ADD FOREIGN KEY (ledgerId) REFERENCES ledger(ledgerId);
