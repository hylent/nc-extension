# Nc extension

PHP 5.4 extension developed with C language.

The main purpose of this repo is to set an milestone to PHP kernel studying.

## Kernel

Kernel module is designed to solve dependency issues between objects.

```
// Containers
class Container extends Nc\Kernel {}
$c = new Container();
$c['hello'] = 'world';
echo $c['hello']; // world
var_dump(isset($c['hello'])); // true
var_dump(isset($c['foo'])); // false
unset($c['hello']);
echo $c['hello']; // null

// Relation between objects
class A { public $b; }
class B { public $a; }
$a = new A();
$b = new B();
$a->b = $b;
$b->a = $a;
var_dump($a->b); // B
var_dump($b->a); // A

// Simplify dependency injection between objects
class A extends Nc\Kernel {}
class B extends Nc\Kernel {}
$a = new A();
$b = new B();
$a->inject('b', $b, 'a'); // One invocation
var_dump($a['b']); // B
var_dump($b['a']); // A

// Lazy initialization
class A extends Nc\Kernel {}
class B extends Nc\Kernel {}
$a = new A();
$a->setInjection('b', array(
    'class' => 'B',
    'namedAs' => 'a',
));
var_dump($a['b']); // B construct at use

// Configurable dependencies
class A extends Nc\Kernel {}
class B extends Nc\Kernel { public function __construct($option) { $this->option = $option; } }
class C extends Nc\Kernel { public function __construct() { echo 'C', PHP_EOL; } }
$a = Nc\Kernel::newInstance(array(
    'class' => 'A',
    'injections' => array(
        'b' => array(
            'class' => 'B',
            'namedAs' => 'a',
        ),
        'c' => array(
            'class' => 'C',
            'namedAs' => 'a',
            'alwaysNew' => true,
        ),
    ),
));
var_dump($a['b']); // B
var_dump($a['b']['a']); // A
var_dump($a['c']); // C, outputs 'C'
var_dump($a['c']); // C, outputs 'C' again, because `alwaysNew` is set to `true`
```

## Loader

Loader module aims at class auto-loading.

```
// Class `C` is in path `./c.php`
(new Nc\Loader\ClassPathLoader)->add('C', __DIR__ . '/c.php');
new C;

// Class `Mall\Controller\Product` is in path `./mall/controller/product.php`
(new Nc\Loader\NamespaceDirectoryLoader)->add('Mall', __DIR__, true);
new Mall\Controller\Product;
```

## Sql

Sql module aims at wrapping low-level database connections, transactions, queries, results fetching.
For now it supports Mysql/PostgreSQL/Oracle.

```
// Connection
$connection = new Nc\Sql\PdoConnection('mysql:host=127.0.0.1;port=3306;dbname=nc', 'nc', 'nc');

// Test tables
// create table kv (k varchar(10) not null primary key, v text, t int not null default 0);
// create table iv (i int not null auto_increment primary key, v text);

// Transactions
$connection->begin();
if (true) {
    $connection->commit();
} else {
    $connection->rollback();
}

// Query
$result = $connection->query("set names utf8", 0);
var_dump($result); // true

// Query some records
$result = $connection->query("show tables", $connection::ALL);
var_dump($result); // Two-dimensional array

// Query one record
$result = $connection->query("select * from kv where k = 'nc'", $connection::ROW);
var_dump($result); // One-dimensional array

// Qyery one column
$result = $connection->query("select count(*) from kv", $connection::ONE);
var_dump($result); // A numeric string

// Adapters
$adapter = new Nc\Sql\Mysql();
$adapter['connection'] = $connection; // Set connection of this Adapter

// Deletes
$adapter->delete('kv', "k = 'test'"); // true

// Inserts
$adapter->insert('kv', ['k' => 'nc', 'v' => 'value of nc']); // true

// Inserts with insertId returned
$adapter->insert('iv', ['v' => 'test'], 'id'); // A numeric string

// Updates
$adapter->update('kv', "k = 'nc'", ['v' => 'new value of nc']); // true

// Updates Expression
$adapter->update('kv', "k = 'nc'", ['t' => ['+', 2]]); // t+=2, return true

// Selects
$select = new Nc\Sql\Select();
$select['adapter'] = $adapter; // Set adapter of this Select

$select->table('kv')->where("k = 'nc'");
print_R($select->__invoke());
echo $select->__toString(); // select * from kv

$d = $select->table('kv')->where("k = 'nc'")->flag($connection::ROW)->__invoke(); // select * from kv where k = 'nc'

$d = $select->table('iv', 'a')
    ->field('a.i')
    ->field('count(b.k)', 'cnt')
    ->join('kv', 'b', 'a.i = b.t')
    ->group('a.i')
    ->__invoke();

// Conditions
$where = new Nc\Sql\Predicate();
$where['adapter'] = $adapter; // Set adapter of this Predicate

echo $where->eq('k', 'nc')->__toString(); // k = 'nc'

echo $where->gt('t', 1)
    ->like('v', 'value', false)
    ->in('k', ['nc', 'test']); // t > '1' and v like 'value%' and k in ('nc', 'test')

echo $where->gt('t', 1)
    ->like('v', 'value', false)
    ->setAnd()
    ->in('k', ['nc', 'test'])
    ->setOr(); // ((t > '1' and v like 'value%') or k in ('nc', 'test'))

// Take advantage of Nc\Kernel di functionalities
$db = Nc\Kernel::newInstance(array(
    'class' => 'Nc\Sql\Mysql',
    'injections' => array(
        'connection' => array(
            'class' => 'Nc\Sql\PdoConnection',
            'arguments' => array(
                'mysql:host=127.0.0.1;port=3306;dbname=nc',
                'nc',
                'nc',
            ),
            'namedAs' => 'adapter',
        ),
        'select' => array(
            'class' => 'Nc\Sql\Select',
            'namedAs' => 'adapter',
            'alwaysNew' => true,
        ),
        'predicate' => array(
            'class' => 'Nc\Sql\Predicate',
            'namedAs' => 'adapter',
            'alwaysNew' => true,
        ),
    ),
));

$connection = $db['connection']; // Connection
$select = $db['select']; // Select, newly created each time
$where = $db['predicate']; // Predicate, newly created each time

// Brand-new usage by Nc\Kernel
$where = $db['predicate']->eq('k', 'nc');
$db['connection']->begin();
$a = $db['select']->table('kv')->where($where)->flag(Nc\Sql\Connection::ROW)->select();
$a['t'] = rand(0, 9);
$db->update('kv', $where, $a);
$db['connection']->commit();
```
