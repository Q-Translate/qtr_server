DROP TABLE IF EXISTS `qtr_chapters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qtr_chapters` (
  `cid` tinyint(4) NOT NULL COMMENT 'Number of chapter (from 1 up to 114).',
  `verses` smallint(6) NOT NULL COMMENT 'Number of verses.',
  `start` smallint(6) NOT NULL COMMENT 'Index in the table of verses.',
  `name` varchar(20) NOT NULL COMMENT 'The name of the chapter.',
  `tname` varchar(20) NOT NULL COMMENT 'Transliterated name.',
  PRIMARY KEY (`cid`),
  UNIQUE KEY `name` (`name`),
  KEY `tname` (`tname`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `qtr_translations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qtr_translations` (
  `tguid` char(40) CHARACTER SET ascii NOT NULL COMMENT 'Globally Unique ID of the translation, defined as hash: SHA1(CONCAT(translation,lng,vid))',
  `vid` smallint(6) NOT NULL,
  `lng` varchar(5) COLLATE utf8_bin NOT NULL COMMENT 'Language code (en, fr, sq_AL, etc.)',
  `translation` varchar(2500) COLLATE utf8_bin NOT NULL COMMENT 'The translation of the verse.',
  `count` smallint(6) DEFAULT '1' COMMENT 'Count of likes received so far. This can be counted on the table qtr_likes, but for convenience is stored here as well.',
  `umail` varchar(250) CHARACTER SET utf8 NOT NULL COMMENT 'The email of the user that submitted this translation.',
  `ulng` varchar(5) CHARACTER SET utf8 NOT NULL COMMENT 'The translation language of the user that suggested this translation.',
  `time` datetime DEFAULT NULL COMMENT 'Time when the translation was entered into the database.',
  `active` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'The active/deleted status of the record.',
  PRIMARY KEY (`tguid`),
  KEY `time` (`time`),
  KEY `umail` (`umail`),
  KEY `vid` (`vid`),
  FULLTEXT KEY `translation` (`translation`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='Translations of the verses.';
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `qtr_translations_trash`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qtr_translations_trash` (
  `tguid` char(40) CHARACTER SET ascii NOT NULL,
  `vid` smallint(6) NOT NULL,
  `lng` varchar(5) COLLATE utf8_bin NOT NULL COMMENT 'Language code (en, fr, sq_AL, etc.)',
  `translation` varchar(2500) COLLATE utf8_bin NOT NULL COMMENT 'The translation of the verse.',
  `count` smallint(6) DEFAULT '1' COMMENT 'Count of likes received so far. This can be counted on the table qtr_likes, but for convenience is stored here as well.',
  `umail` varchar(250) CHARACTER SET utf8 NOT NULL COMMENT 'The email of the user that submitted this translation.',
  `ulng` varchar(5) CHARACTER SET utf8 NOT NULL COMMENT 'The translation language of the user that suggested this translation.',
  `time` datetime DEFAULT NULL COMMENT 'Time when the translation was entered into the database.',
  `active` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'The active/deleted status of the record.',
  `d_umail` varchar(250) CHARACTER SET utf8 NOT NULL COMMENT 'The email of the user that deleted this translation.',
  `d_ulng` varchar(5) CHARACTER SET utf8 NOT NULL COMMENT 'The language of the user that deleted this translation.',
  `d_time` datetime NOT NULL COMMENT 'Timestamp of the deletion time.',
  KEY `time` (`time`),
  KEY `umail` (`umail`),
  KEY `d_time` (`d_time`),
  KEY `d_umail` (`d_umail`),
  KEY `tguid` (`tguid`),
  FULLTEXT KEY `translation_text` (`translation`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='Translations that are deleted are saved on the trash table.';
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `qtr_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qtr_users` (
  `umail` varchar(250) NOT NULL COMMENT 'Email of the user.',
  `ulng` varchar(5) NOT NULL COMMENT 'Translation language of the user.',
  `uid` int(11) NOT NULL COMMENT 'The numeric identifier of the user.',
  `name` varchar(60) NOT NULL COMMENT 'Username',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'Disabled (0) or active (1).',
  `points` int(11) DEFAULT '0' COMMENT 'Number of points rewarded for his activity.',
  `config` varchar(250) DEFAULT NULL COMMENT 'Serialized configuration variables.',
  PRIMARY KEY (`umail`,`ulng`,`uid`),
  KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Users that make translations/likes.';
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `qtr_verses`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qtr_verses` (
  `vid` smallint(6) NOT NULL AUTO_INCREMENT COMMENT 'Verse id.',
  `cid` tinyint(4) NOT NULL,
  `nr` smallint(6) NOT NULL COMMENT 'Number of the verse.',
  `verse` varchar(2500) NOT NULL COMMENT 'The text of the verse.',
  PRIMARY KEY (`vid`),
  KEY `cid` (`cid`),
  FULLTEXT KEY `verse` (`verse`)
) ENGINE=MyISAM AUTO_INCREMENT=6237 DEFAULT CHARSET=utf8 COMMENT='Verses of the Quran.';
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `qtr_likes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qtr_likes` (
  `lid` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Internal numeric identifier for a like.',
  `tguid` char(40) CHARACTER SET ascii NOT NULL COMMENT 'Reference to the id of the translation which is liked.',
  `umail` varchar(250) NOT NULL COMMENT 'Email of the user that submitted the like.',
  `ulng` varchar(5) NOT NULL COMMENT 'Translation language of the user that submitted the like.',
  `time` datetime DEFAULT NULL COMMENT 'Timestamp of the like time.',
  `active` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'The active/deleted status of the record.',
  PRIMARY KEY (`lid`),
  UNIQUE KEY `umail_ulng_tguid` (`umail`,`ulng`,`tguid`),
  KEY `time` (`time`),
  KEY `tguid` (`tguid`),
  KEY `umail` (`umail`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Likes for each translation.';
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `qtr_likes_trash`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qtr_likes_trash` (
  `lid` int(11) NOT NULL COMMENT 'Internal numeric identifier for a like.',
  `tguid` char(40) CHARACTER SET ascii NOT NULL COMMENT 'Reference to the id of the translation which is liked.',
  `umail` varchar(250) NOT NULL COMMENT 'Email of the user that submitted the like.',
  `ulng` varchar(5) NOT NULL COMMENT 'Translation language of the user that submitted the like.',
  `time` datetime DEFAULT NULL COMMENT 'Timestamp of the like time.',
  `active` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'The active/deleted status of the record.',
  `d_time` datetime NOT NULL COMMENT 'Timestamp of the deletion time.',
  KEY `time` (`time`),
  KEY `tguid` (`tguid`),
  KEY `d_time` (`d_time`),
  KEY `umail_ulng_tguid` (`umail`,`ulng`,`tguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Likes that are deleted are saved on the trash table.';
/*!40101 SET character_set_client = @saved_cs_client */;
