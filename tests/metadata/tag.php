<?php
return array(
'table' => 'tag',
'relations' => array(
    'class' => 'Nc\Sql\RelationContainer',
    'injections' => array(
        'topics' => array(
            'class' => 'Nc\Sql\HavingAndBelongingToManyRelation',
            'properties' => array(
                'target' => 'topic',
                'mediator' => 'topic_tag',
                'selfId' => 'tag_id',
                'targetId' => 'topic_id',
            ),
        ),
    ),
),
);
