<?php
	header("Content-Type: text/html; charset=utf-8");
?>

<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8" />
	<meta name="viewport" content="width=device-width, initial-scale=1.0" />
	<title>CGI Test</title>
	<link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap" rel="stylesheet" />
	<link href="/cgi/library/css/layout.css" rel="stylesheet">
	</head>
	<body>
		<h1>CGI Test</h1>
		<div class="container">
			<div class="test-section">
				<h2>Query Parameters Received</h2>
				<div class="cookie-list">
<?php
	 if (!empty($_SERVER['QUERY_STRING'])) {
		echo "<ul>";
		$query_strings = explode('&', $_SERVER['QUERY_STRING']);
		foreach ($query_strings as $query_string) {
			$values= explode('=', $query_string);
			echo "<li><strong>" . htmlspecialchars($values[0]) . ":</strong> " . htmlspecialchars($values[1]) . "</li>";
		}
		echo "</ul>";
	} else {
		echo "<p>No cookies set for this domain.</p>";
	}
 ?>
				</div>
				<h2>Server Environment Variables</h2>
				<div class="cookie-list">
<?php
	if (!empty($_SERVER)) {
		echo "<ul>";
		foreach ($_SERVER as $name => $value) {
			echo "<li><strong>" . htmlspecialchars($name) . ":</strong> " . htmlspecialchars($value) . "</li>";
		}
			echo "</ul>";
	} else {
		echo "<p>No cookies set for this domain.</p>";
	}
?>
				</div>
				<a href="/cgi/index.html#cgi">Back to Tests</a>
			</div>
		</div>
	</body>
</html>
