<?php

(new Nc\Loader\NamespaceDirectoryLoader)->add('Nc', __DIR__ . '/../lib', true);

return Nc\Kernel::newInstance(array(
    'class' => 'Nc\Sql\DirectoryModelFactory',
    'arguments' => array(
        __DIR__ . '/../data/metadata',
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

