<?php

if (empty($_SERVER['QUERY_STRING']))
{
    foreach (glob('z*.php') as $f)
        printf('<a href="?%s">%s</a><br />', $f, $f);

    return;
}

set_error_handler(function($n, $s, $f, $l) {
    throw new ErrorException("{$s} ({$f}:{$l})");
});

header('Content-type: text/plain; charset=utf-8');

$ts = microtime(true);

try
{
    require $_SERVER['QUERY_STRING'];
}
catch (exception $ex)
{
    echo $ex, PHP_EOL;
}

echo PHP_EOL, '--------------------------------------------------', PHP_EOL;
echo sprintf('%.3fms %.3fM', (microtime(true) - $ts) * 1e3, memory_get_peak_usage() / (1 << 20)), PHP_EOL;
print_R(nc\sql\connection::getqueries());
print_R(get_included_files());
