<?php
 require './vendor/autoload.php';
 $ar = array('region'=>'ap-northeast-2','version'=>'2016-11-15');
 $ec2 = new Aws\Ec2\Ec2Client($ar);
 $inst = $ec2->describeInstances([]);
 print($inst['Reservations'][0]['Instances'][0]['NetworkInterfaces'][0]['Association']['PublicIp']."\n");
 ?>
