-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Počítač: 127.0.0.1:3306
-- Vytvořeno: Stř 16. říj 2019, 12:46
-- Verze serveru: 8.0.18
-- Verze PHP: 7.2.18

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Databáze: `IISprojekt`
--

-- --------------------------------------------------------

--
-- Struktura tabulky `kurz`
--

DROP TABLE IF EXISTS `kurz`;
CREATE TABLE IF NOT EXISTS `kurz` (
  `Kurz_ID` varchar(10) COLLATE utf8_czech_ci NOT NULL,
  `nazev` varchar(50) COLLATE utf8_czech_ci NOT NULL,
  `popis` text COLLATE utf8_czech_ci NOT NULL,
  `typ` varchar(50) COLLATE utf8_czech_ci NOT NULL,
  `cena` int(20) NOT NULL,
  `garant_ID` int(11) NOT NULL,
  PRIMARY KEY (`Kurz_ID`),
  KEY `garant_ID` (`garant_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;

--
-- Vypisuji data pro tabulku `kurz`
--

INSERT INTO `kurz` (`Kurz_ID`, `nazev`, `popis`, `typ`, `cena`, `garant_ID`) VALUES
('CADZ', 'Zaklady programu CAD', 'Seznamuje se zaklady modelovani pomoci CAD technologii.', 'Strojirenstvi', 2900, 9),
('IMD', 'Uvod do medicinskych databazi', 'Seznamuje se zakladem tvorby databazi v medicinskem prostredi, vhodne pro studenty informatiky nebo bionformatiky', 'Medicinska informatika', 5900, 3),
('VID', 'Zaklady tvorby video-obsahu', 'Vhodne pro zajemce o nataceni videi, kratkych filmu, strihani videa atd.', 'Multimedia', 5000, 3);

-- --------------------------------------------------------

--
-- Struktura tabulky `termin`
--

DROP TABLE IF EXISTS `termin`;
CREATE TABLE IF NOT EXISTS `termin` (
  `Kurz_ID` varchar(15) COLLATE utf8_czech_ci NOT NULL,
  `datum` date NOT NULL,
  `cas` time(6) NOT NULL,
  `mistnost_ID` varchar(15) COLLATE utf8_czech_ci NOT NULL,
  `lektor_ID` int(15) NOT NULL,
  `popis` text COLLATE utf8_czech_ci NOT NULL,
  `typ` varchar(20) COLLATE utf8_czech_ci NOT NULL,
  `kapacita` int(10) NOT NULL,
  `doba trvani` int(10) NOT NULL,
  PRIMARY KEY (`Kurz_ID`,`datum`,`cas`,`mistnost_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;

-- --------------------------------------------------------

--
-- Struktura tabulky `uzivatel`
--

DROP TABLE IF EXISTS `uzivatel`;
CREATE TABLE IF NOT EXISTS `uzivatel` (
  `Uzivatel_ID` int(11) NOT NULL AUTO_INCREMENT,
  `jmeno` varchar(25) COLLATE utf8_czech_ci NOT NULL,
  `prijmeni` varchar(25) COLLATE utf8_czech_ci NOT NULL,
  `heslo` varchar(30) COLLATE utf8_czech_ci NOT NULL,
  `role` int(11) NOT NULL,
  `email` varchar(25) COLLATE utf8_czech_ci NOT NULL,
  PRIMARY KEY (`Uzivatel_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;

--
-- Vypisuji data pro tabulku `uzivatel`
--

INSERT INTO `uzivatel` (`Uzivatel_ID`, `jmeno`, `prijmeni`, `heslo`, `role`, `email`) VALUES
(1, 'Marek', 'Prokop', '', 1, 'marek.prokop@email.cz'),
(2, 'Eleanora', 'Palkova', '', 2, 'palkova@seznam.cz'),
(3, 'Andrej', 'Maly', '', 3, 'maly.andrej@seznam.cz'),
(4, 'Iveta', 'Stara', '', 4, 'stara@gmail.com'),
(5, 'Katerina', 'Humpolcova', '', 5, 'humpolcova@gmail.com'),
(6, 'Anna', 'Novotna', '', 5, 'novotna.anna@centrum.cz'),
(7, 'Adam', 'Hajek', '', 4, 'hajek@email.cz'),
(8, 'Lucie', 'Otahalova', '', 4, 'lucie.otahalova@post.cz'),
(9, 'Zaneta', 'Formankova', '', 3, 'formankova@email.cz'),
(10, 'Zuzana', 'Stivinova', '', 5, 'stivinova@seznam.cz');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
