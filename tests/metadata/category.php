<?php
return array(
'table' => 'category',
'relations' => array(
    'class' => 'Nc\Sql\RelationContainer',
    'injections' => array(
        'parent' => array(
            'class' => 'Nc\Sql\BelongingToRelation',
            'properties' => array(
                'target' => 'category',
                'foreignId' => 'pid',
            ),
        ),
        'children' => array(
            'class' => 'Nc\Sql\HavingManyRelation',
            'properties' => array(
                'target' => 'category',
                'targetForeignId' => 'pid',
            ),
        ),
        'topics' => array(
            'class' => 'Nc\Sql\HavingManyRelation',
            'properties' => array(
                'target' => 'topic',
                'targetForeignId' => 'category_id',
            ),
        ),
    ),
),
);
