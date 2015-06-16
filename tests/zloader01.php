<?php

if (rand() % 2)
{
    $l = new nc\loader\classpathloader();
    $l->add('c', __DIR__ . '/c.php');
}
else
{
    $l = new nc\loader\namespacedirectoryloader();
    $l->add('c', __DIR__);
}

echo get_class($l), PHP_EOL;
$c = new c();

print_R($c);
