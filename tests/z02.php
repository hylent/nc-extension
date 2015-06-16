<?php

$modelFactory = require 'modelFactory.php';

$entity = $modelFactory->tag['entity'];

$entity->modify(5);
$entity->name = 'm.'. time();
$entity->save();

$entity->persist();

print_R($entity);
