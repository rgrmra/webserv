#!/usr/bin/php-cgi
<?php
	//phpinfo();
	echo isset($_GET["teste"]) ? $_GET["teste"] : "Parâmetro não definido";
	//$data = file_get_contents("php://input");
	//echo "received: " . $data;
?>
