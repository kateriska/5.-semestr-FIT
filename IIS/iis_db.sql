ALTER DATABASE `iisprojekt_db`
	DEFAULT CHARACTER SET utf8mb4
	COLLATE utf8mb4_unicode_520_ci;
USE iisprojekt_db;
SET foreign_key_checks = 0;
DROP TABLE IF EXISTS `uzivatel`;
DROP TABLE IF EXISTS `kurz`;
DROP TABLE IF EXISTS `mistnost`;
DROP TABLE IF EXISTS `termin`;
SET foreign_key_checks = 1;


CREATE TABLE `uzivatel` (
  `Uzivatel_ID` int NOT NULL AUTO_INCREMENT,
  `jmeno` varchar(25) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `prijmeni` varchar(25) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `heslo` varchar(30) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `role` int NOT NULL,
  `email` varchar(25) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  PRIMARY KEY (`Uzivatel_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;

CREATE TABLE `kurz` (
  `Kurz_ID` varchar(10) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `nazev` varchar(50) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `popis` text COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `typ` varchar(50) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `cena` int NOT NULL,
  `garant_ID` int NOT NULL,
  PRIMARY KEY (`Kurz_ID`),
  CONSTRAINT `kurz_ibfk_1` FOREIGN KEY (`garant_ID`) REFERENCES `uzivatel` (`Uzivatel_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;

CREATE TABLE `mistnost` (
  `Mistnost_ID` varchar(15) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `adresa` varchar(35) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `typ` varchar(25) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `kapacita` int NOT NULL,
  PRIMARY KEY (`Mistnost_ID`),
  KEY `kapacita` (`kapacita`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;

CREATE TABLE `termin` (
  `Kurz_ID` varchar(15) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `datum` date NOT NULL,
  `cas` time(6) NOT NULL,
  `mistnost_ID` varchar(15) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `lektor_ID` int NOT NULL,
  `popis` text COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `typ` varchar(20) COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `kapacita` int NOT NULL,
  `doba trvani` int NOT NULL,
  PRIMARY KEY (`Kurz_ID`,`datum`,`cas`,`mistnost_ID`),
  KEY `lektor_ID` (`lektor_ID`),
  KEY `mistnost_ID` (`mistnost_ID`),
  CONSTRAINT `termin_ibfk_1` FOREIGN KEY (`Kurz_ID`) REFERENCES `kurz` (`Kurz_ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `termin_ibfk_2` FOREIGN KEY (`lektor_ID`) REFERENCES `uzivatel` (`Uzivatel_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;


