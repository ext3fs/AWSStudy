<?php
require 'vendor/autoload.php';
$param = Array('region'=>'ap-northeast-2', 'version'=>'2006-03-01');
$s3 = new Aws\S3\S3Client($param);
$s3->getObject(Array(
    'Bucket'=>'ext7fs-bucket',
    'Key'=>'curr.cpp',
    'SaveAs'=>fopen('curr.cpp', 'w')
));
