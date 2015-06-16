<?php

$db = Nc\Kernel::newInstance(array(
    'class' => 'Nc\Sql\Pgsql',
    'injections' => array(
        'connection' => array(
            'class' => 'Nc\Sql\PdoConnection',
            'arguments' => array(
                'pgsql:host=192.168.1.85;port=5432;dbname=nc;user=nc;password=nc',
            ),
            'namedAs' => 'adapter',
        ),
        'predicate' => array(
            'class' => 'Nc\Sql\Predicate',
            'alwaysNew' => true,
            'namedAs' => 'adapter',
        ),
        'select' => array(
            'class' => 'Nc\Sql\Select',
            'alwaysNew' => true,
            'namedAs' => 'adapter',
        ),
    ),
));

$db['connection']->begin();
$db->update('kv', '1=1', ['v' => 'vvv',]);
$db['connection']->rollback();

$r = $db['select']->table('kv')->__invoke();
var_dump($r);
