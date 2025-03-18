
CREATE DATABASE `belenus` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE USER 'belenus'@'localhost' IDENTIFIED BY 'belenus';

GRANT ALL PRIVILEGES ON `belenus` . * TO 'belenus'@'localhost' WITH GRANT OPTION;
