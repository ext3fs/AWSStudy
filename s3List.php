<?php
require 'vendor/autoload.php';
$ar = Array('region'=>'ap-northeast-2', 'version'=>'2006-03-01');
$s3 = new Aws\S3\S3Client($ar);
$list = $s3->listObjects(Array('Bucket'=>'ext7fs-bucket'));
$listArray = $list->toArray();
foreach($listArray['Contents'] as $item){
    print($item['Key']."\n");
}
?>
