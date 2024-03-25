-- -----------------------------------------------------------------------------------
-- Filename    : correct_report_cardhistory.sql
-- -----------------------------------------------------------------------------------

USE `belenus`;

-- -----------------------------------------------------------------------------------

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

-- -----------------------------------------------------------------------------------
