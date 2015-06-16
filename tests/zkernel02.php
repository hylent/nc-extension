<?php

class a extends nc\kernel {}
class b extends nc\kernel {}
class c extends b {}

class d
{
    public static function dd()
    {
        print_R(b::newinstance('c'));
        print_R(b::newinstance('a'));
    }
}

print_R(b::newinstance('c'));
print_R(b::newinstance('a'));

echo '-------', PHP_EOL;
d::dd();
