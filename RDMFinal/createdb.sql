--
-- Database: `rdm`
--

-- --------------------------------------------------------

--
-- Table structure for table `seen`
--

CREATE TABLE IF NOT EXISTS `seen` (
  `registration` text NOT NULL COMMENT 'Matrícula',
  `color` text NOT NULL COMMENT 'Cor do automóvel',
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Data de detecção'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Table structure for table `plates`
--

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

--
-- Dumping data for table `plates`
--

INSERT INTO `plates` (`registration`, `owner`, `color`, `year`, `brand`, `model`, `registryTime`, `warnings`) VALUES
('6828PD', 'Joaquim Ferreira Duarte', 'Cinzento', 2001, 'Hyundai', 'Ascent', '2011-01-25 11:20:41', 0),
('51GM51', 'Joaquim Ferreira Duarte', 'Preto', 2008, 'Citroen', 'C4', '2011-01-25 11:27:37', 0),
('83JH29', 'João Silva', 'Amarelo', 2011, 'Laborghini', '?', '2011-02-25 16:52:07', 0),
('6969IS', 'Ricardo Estêvão', 'Preto', 2011, 'Ferrari', 'Enzo', '2011-02-11 16:40:34', 0),
('3444TT', 'Manuel Joaquim', 'Cinzento', 2004, 'Mercedes', 'E270', '2011-02-07 14:29:12', 1),
('111111', '1111sss', '111', 2011, 'Alfa Romeo', '', '2011-02-04 17:14:48', 0),
('88IH08', 'Dário Barroca', 'Vermelha', 2009, 'Honda', '(Mota)', '2011-02-04 15:27:54', 0),
('64MJ16', 'Alfredo', 'Cinzento', 2008, 'Dodge', 'Viper', '2011-02-01 17:40:58', 0),
('FR2211', 'Francisco Robalo', 'Azul', 2011, 'Redbull', 'X2010 Sebastian Vattel', '2011-03-04 16:27:38', 0),
('2892EO', 'Duarte Duarte', 'Branco', 2004, 'Peugeot', 'xyz', '2011-03-11 16:59:02', 0),
('2892E0', 'Duarte Duarte', 'Branco', 2004, 'Peugeot', 'xyz', '2011-03-11 16:59:32', 0),
('28-92-E0', 'test', 'test', 2011, 'test', 'test', '2011-03-11 17:52:06', 0);

-- 28 de Março de 2011