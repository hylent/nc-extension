<?php

$modelFactory = require 'modelFactory.php';

$model = $modelFactory->topic;

$a = $model->select('1 = 1');
print_R($a);

print_R($model);
