<?php

$a = new nc\sql\mysql();

$s = new nc\sql\select();

$s->table('kv')->table('category', 'c');
$r = $a->buildSelectTable($s);
var_dump($r);

$s->field('id')->field('count(*)', 'c');
$r = $a->buildSelectField($s);
var_dump($r);

$s->join('member', 'm', 'm.id = c.member_id')->join('topic', null, 'topic.id = c.id');
$r = $a->buildSelectJoin($s);
var_dump($r);

$s->where("m.id = 1");
$r = $a->buildSelectWhere($s);
var_dump($r);

$s->group('id')->group('name,sb');
$r = $a->buildSelectGroup($s);
var_dump($r);

$r = $a->buildSelectHaving($s);
var_dump($r);

$s->group('id')->order('state', true)->order('id');
$r = $a->buildSelectOrder($s);
var_dump($r);

$r = $a->buildInsertQuery('kv', ['k' => 'kk', 'v' => 'vv']);
var_dump($r);
