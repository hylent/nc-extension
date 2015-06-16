<?php

(new Nc\Loader\NamespaceDirectoryLoader())
    ->add('Nc', __DIR__ . '/../lib', false)
    ->add('Bbs', __DIR__ . '/../app');


$mf = Nc\Kernel::newInstance(array(
    'class' => 'Nc\Sql\Orm\NamespacedModelFactory',
    'properties' => array(
        'namespace' => 'Bbs\Model',
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
        'entity' => array(
            'class' => 'Nc\Sql\Orm\Entity',
            'namedAs' => 'modelFactory',
            'alwaysNew' => true,
        ),
    ),
));

$topic = $mf->topic;

$topicEntity = $mf['entity'];
$topicEntity->inject('model', $topic);

$topicEntity->load(range(1,10));

//print_R($topicEntity);
//print_R($topicEntity('user')->preload());
print_R($topicEntity('reply')->preload()->grouped('topic_id'));
