SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
CREATE DATABASE `rdm` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `rdm`;

CREATE TABLE IF NOT EXISTS `plates` (
  `registration` text NOT NULL COMMENT 'Matrícula',
  `owner` text NOT NULL COMMENT 'Nome do proprietário',
  `color` text NOT NULL COMMENT 'Cor do automóvel',
  `year` year(4) NOT NULL DEFAULT '2011' COMMENT 'Ano de compra do automóvel',
  `brand` text NOT NULL COMMENT 'Marca do automóvel',
  `model` text NOT NULL COMMENT 'Modelo do automóvel',
  `registryTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Data de registo na DB',
  `warnings` int(11) NOT NULL DEFAULT '0' COMMENT 'Avisos (seguros, etc.)'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `seen` (
  `registration` text NOT NULL COMMENT 'Matrícula',
  `color` text NOT NULL COMMENT 'Cor do automóvel',
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Data de detecção'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
