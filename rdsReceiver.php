<?php
include("db.php");
$title = mysqli_real_escape_string($write,$_POST["title"]);
$desc = mysqli_real_escape_string($write,$_POST["description"]);
$sql = "insert into topic (title,description,author,created) values('{$title}','{$desc}','ext7fs',now())";
mysqli_query($write, $sql);
header("location:rdsTest.php");
?>
