<?php

(new Nc\Loader\NamespaceDirectoryLoader)->add('Nc', __DIR__ . '/../lib');

$adapter = Nc\Kernel::newInstance(array(
    'class' => 'Nc\Sql\Mysql',
    'injections' => array(
        'connection' => array(
            'class' => 'Nc\Sql\PdoConnection',
            'arguments' => array(
                'mysql:host=192.168.1.13;port=3306;dbname=bbs;charset=utf8',
                'bbs',
                'bbs',
            ),
            'namedAs' => 'adapter',
        ),
        'predicate' => array(
            'class' => 'Nc\Sql\Predicate',
            'namedAs' => 'adapter',
            'alwaysNew' => true,
        ),
        'select' => array(
            'class' => 'Nc\Sql\Select',
            'namedAs' => 'adapter',
            'alwaysNew' => true,
        ),
    ),
));

$tag = new Nc\Sql\TableGateway('tag');
$tag['adapter'] = $adapter;

$topic = new Nc\Sql\TableGateway('topic');
$topic['adapter'] = $adapter;

$topicTag = new Nc\Sql\TableGateway('topicTag', 'topic_tag', ['topic_id', 'tag_id']);
$topicTag['adapter'] = $adapter;

print_R($topicTag->find(1, 1));
print_R($topicTag->findMany(null, [1, 2]));
