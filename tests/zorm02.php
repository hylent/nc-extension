<?php

$mf = Nc\Kernel::newInstance(array(
    'class' => 'Nc\Sql\Orm\DirectoryModelFactory',
    'arguments' => array(
        __DIR__ . '/metadata',
    ),
    'injections' => array(
        'adapter' => array(
            'class' => 'Nc\Sql\Mysql',
            'injections' => array(
                'connection' => array(
                    'class' => 'Nc\Sql\PdoConnection',
                    'arguments' => array(
                        'mysql:host=127.0.0.1;port=3306;dbname=bbs;charset=utf8',
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
        ),
    ),
));

$topicModel = $mf->topic;
$topicEntity = $topicModel->findEntity('1=1');

print_R($topicEntity);
