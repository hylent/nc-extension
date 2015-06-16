<?php

$a = new nc\sql\mysql();
$a['connection'] = new nc\sql\pdoconnection('mysql:host=192.168.1.13;port=3306;dbname=nc;charset=utf8', 'nc', 'nc');

$r = $a->insert('iv', ['v' => rand(1, 9),], 'id');
var_dump($r);

$r = $a->update('iv', 'id = 1', array(
    'v' => ['+', 1],
));
var_dump($r);

$r = $a->delete('kv', '0');
var_dump($r);

$data = array(
    'v' => 0,
);
var_dump($data);

$r = $a->update('kv', '0', $data);
var_dump($r);

var_dump($data);
