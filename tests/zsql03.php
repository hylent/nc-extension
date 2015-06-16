<?php

$p = new nc\sql\select();

$p->paginate(7, 2);

$p->setNumRows(25);

print_R($p);
