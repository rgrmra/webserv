#!/usr/bin/php-cgi
<?php
	//phpinfo()
	$data = file_get_contents("php://input");
	echo "received: " . $data;
?>
