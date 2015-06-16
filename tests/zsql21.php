<?php

$connectionOptions = array(
    array(
        'class' => 'Nc\Sql\PdoConnection',
        'arguments' => array(
            'mysql:host=192.168.1.13;port=3306;dbname=nc;charset=utf8',
            'nc',
            'nc',
        ),
        'namedAs' => 'adapter',
    ),
    array(
        'class' => 'Nc\Sql\MysqliConnection',
        'arguments' => array(
            '192.168.1.13',
            'nc',
            'nc',
            'nc',
            3306,
        ),
        'namedAs' => 'adapter',
    ),
);

$db = Nc\Kernel::newInstance(array(
    'class' => 'Nc\Sql\Mysql',
    'injections' => array(
        'connection' => $connectionOptions[array_rand($connectionOptions)],
        //'connection' => $connectionOptions[1],
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


$db['connection']->begin();

$db->update('kv', $where = $db['predicate']->eq('k', 's'), array(
    'v' => 'sss',
));

$db['connection']->rollback();

$r = $db['select']->table('kv')->where($where)->__invoke();
print_R($r);

print_R($db);
