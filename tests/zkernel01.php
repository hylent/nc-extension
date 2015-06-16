<?php
class a extends nc\kernel {}
class b extends nc\kernel {}
class c extends nc\kernel {}

$a = nc\kernel::newinstance(array(
    'class' => 'a',
    'injections' => array(
        'b' => array(
            'class' => 'b',
            'namedAs' => 'a',
        ),
    ),
));

$a['b']->setInjection('c', array(
    'class' => 'c',
    'namedAs' => 'b',
    'injections' => array(
        'd' => 'stdclass',
    ),
));

print_R($a['b']['c']['d']);
print_R($a);
