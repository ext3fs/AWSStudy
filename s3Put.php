<?php
require 'vendor/autoload.php';
$ar = Array('region'=>'ap-northeast-2', 'version'=>'2006-03-01');
$s3 = new Aws\S3\S3Client($ar);
$s3->putObject(Array(
    'ACL'=>'public-read',
    'SourceFile'=>'ttt.txt',
    'Bucket'=>'ext7fs-bucket',
    'Key'=>'ttt.txt'
));
?>
