<?php

$s = new nc\sql\select();
$s['adapter'] = new nc\sql\mysql();
$s['adapter']['connection'] = new nc\sql\pdoconnection('mysql:host=192.168.1.13;port=3306;dbname=nc;charset=utf8', 'nc', 'nc');

$s->table('kv')->paginate(10, 1, 1);

var_dump($s);
echo $s->__toString(), PHP_EOL;

$r = $s();

var_dump($r);
var_dump($s);
