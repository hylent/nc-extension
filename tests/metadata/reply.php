<?php
return array(
'table' => 'reply',
'relations' => array(
    'class' => 'Nc\Sql\RelationContainer',
    'injections' => array(
        'category' => array(
            'class' => 'Nc\Sql\BelongingToRelation',
            'properties' => array(
                'target' => 'category',
                'foreignId' => 'category_id',
            ),
        ),
        'user' => array(
            'class' => 'Nc\Sql\BelongingToRelation',
            'properties' => array(
                'target' => 'user',
                'foreignId' => 'user_id',
            ),
        ),
    ),
),
);
