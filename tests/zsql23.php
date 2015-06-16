<?php

// definitions for test

const OCI_NUM = 1;
define('OCI_ASSOC', 2);
const OCI_NO_AUTO_COMMIT = 4;
const OCI_COMMIT_ON_SUCCESS = 8;

function oci_new_connect()
{
    echo '## ', __FUNCTION__, PHP_EOL;
    return new stdclass;
}

function oci_commit()
{
    echo '## ', __FUNCTION__, PHP_EOL;
    return true;
}

function oci_rollback()
{
    echo '## ', __FUNCTION__, PHP_EOL;
    return false;
}

function oci_parse()
{
    echo '## ', __FUNCTION__, PHP_EOL;
    return new stdclass;
}

function oci_execute()
{
    echo '## ', __FUNCTION__, PHP_EOL;
    return true;
}

function oci_error()
{
    echo '## ', __FUNCTION__, PHP_EOL;
    return ['message' => 'you are fucked!'];
}

function oci_fetch_array($s, $mode)
{
    echo '## ', __FUNCTION__, PHP_EOL;

    if (1)
    {
        static $ri = 5;

        if (--$ri)
        {
            return ['SB'. $ri => 'FUCK'];
        }

        return false;
    }

    return [0 => 'NWTG'];
}


$c = new nc\sql\ociconnection('user', 'pass');
var_dump($c);

$r = $c->query("", $c::ONE);
var_dump($r);

return;

$r = $c->query("", $c::ROW);
var_dump($r);

$r = $c->query("", $c::ALL);
var_dump($r);

$c->begin();
var_dump($c);
$c->commit();
var_dump($c);

$c->begin();
$c->rollback();
