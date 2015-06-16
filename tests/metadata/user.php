<?php
return array(
'table' => 'user',
'relations' => array(
    'class' => 'Nc\Sql\RelationContainer',
    'injections' => array(
        'topics' => array(
            'class' => 'Nc\Sql\HavingManyRelation',
            'properties' => array(
                'target' => 'topic',
                'targetForeignId' => 'user_id',
            ),
        ),
        'replies' => array(
            'class' => 'Nc\Sql\HavingManyRelation',
            'properties' => array(
                'target' => 'reply',
                'targetForeignId' => 'user_id',
            ),
        ),
    ),
),
);
