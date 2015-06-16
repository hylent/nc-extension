<?php

$c = new nc\sql\pdoconnection('mysql:host=192.168.1.13;port=3306;dbname=nc;charset=utf8', 'nc', 'nc');
var_dump($c);

$r = $c->query("select count(*) from kv", $c::READ | $c::ONE);
var_dump($r);

$r = $c->query("select * from kv where k = 's'", $c::READ | $c::ROW);
var_dump($r);

$r = $c->query("select * from kv where k = 'sb'", $c::READ | $c::ROW);
var_dump($r);

$r = $c->query("select * from kv", $c::READ | $c::ALL);
var_dump($r);

$r = $c->query("select * from category", $c::READ | $c::ALL);
var_dump($r);

$r = $c->query("select * from sb", $c::READ | $c::ALL);
var_dump($r);
