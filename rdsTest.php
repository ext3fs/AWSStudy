<!DOCKTYPE>
<html>
	<head><meta charset="utf-8"></head>
	<body>
		<form action="rdsReceiver.php" method="post">
			<p>
				Title :
				<input type="text" name="title">
			</p>
			<p>
				Text :
				<textarea name="description">
				</textarea>
			</p>
			<p><input type="submit"></p>
		</form>
		<ul>
<?php
include("db.php");
$res = mysqli_query($read, "select * from topic");
while($row = mysqli_fetch_assoc($res)){
	$title = mysqli_real_escape_string($read,$row["title"]);
	print("<li>{$title}</li>");
}
?>
		</ul>
	</body>
</html>














