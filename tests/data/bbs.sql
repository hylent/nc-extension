-- category
DROP TABLE IF EXISTS `category`;
CREATE TABLE `category` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `name` varchar(50) NOT NULL COMMENT '名称',
  `pid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '父级id',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='分类';

-- reply
DROP TABLE IF EXISTS `reply`;
CREATE TABLE `reply` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `content` text NOT NULL COMMENT '内容',
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '时间',
  `topic_id` int(10) unsigned NOT NULL COMMENT '话题id',
  `user_id` int(10) unsigned NOT NULL COMMENT '用户id',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='回复';

-- tag
DROP TABLE IF EXISTS `tag`;
CREATE TABLE `tag` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `name` varchar(50) NOT NULL COMMENT '名称',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='标签';

-- topic
DROP TABLE IF EXISTS `topic`;
CREATE TABLE `topic` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `title` varchar(200) NOT NULL COMMENT '标题',
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '时间',
  `user_id` int(10) unsigned NOT NULL COMMENT '用户id',
  `category_id` int(10) unsigned NOT NULL COMMENT '分类id',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='话题';

-- topic_tag
DROP TABLE IF EXISTS `topic_tag`;
CREATE TABLE `topic_tag` (
  `topic_id` int(10) unsigned NOT NULL COMMENT '话题id',
  `tag_id` int(10) unsigned NOT NULL COMMENT '标签id',
  PRIMARY KEY (`topic_id`,`tag_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='话题标签';

-- user
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `name` varchar(50) NOT NULL COMMENT '用户名',
  `passwd` char(32) NOT NULL COMMENT '密码',
  `realname` varchar(10) NOT NULL DEFAULT '' COMMENT '真实名称',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='用户';
