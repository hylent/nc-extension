<?php

$modelFactory = require 'modelFactory.php';

$topicModel = $modelFactory->topic;
// print_R($topicModel); return;


$topicEntity = $topicModel->getEntity();
$topicEntity->load('1=1');
print_R($topicEntity->records());

$userRelation = $topicEntity->getRelation('user');
$categoryRelation = $topicEntity->getRelation('category');
$replyRelation = $topicEntity->getRelation('replies');

print_R($userRelation()->records());
print_R($categoryRelation()->records());
print_R($replyRelation()->groupedRecords('topic_id'));

// return;

$tagRelation = $topicEntity->getRelation('tags');
$tagRelation->getPredicate()->gt('tag_id', 0);
$tagRelation->getSelect()->order('tag_id');
print_R($tagRelation()->records());
