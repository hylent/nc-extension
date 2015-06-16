<?php

$p = new nc\sql\predicate();

$p['adapter'] = new nc\sql\mysql();

$p->raw(1);

$p->eq('k', 1);
$p->like('v', 2, 1);
$p->like('v', 'hh', 0);
$p->between('k', 1, 10);

$p->in('k', [1, 2, 7]);

echo $p, PHP_EOL;

$p->setAnd();


print_R($p);
