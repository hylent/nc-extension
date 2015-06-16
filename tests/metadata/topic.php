<?php
return array(
'table' => 'topic',
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
        'replies' => array(
            'class' => 'Nc\Sql\HavingManyRelation',
            'properties' => array(
                'target' => 'reply',
                'targetForeignId' => 'topic_id',
            ),
        ),
        'tags' => array(
            'class' => 'Nc\Sql\HavingAndBelongingToManyRelation',
            'properties' => array(
                'target' => 'tag',
                'mediator' => 'topic_tag',
                'selfId' => 'topic_id',
                'targetId' => 'tag_id',
            ),
        ),
    ),
),
);
