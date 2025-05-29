<?php
	header("Content-Type: text/html; charset=utf-8");
?>

<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>Form Submission Test</title>
		<link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap" rel="stylesheet" />
		<link href="https://fonts.googleapis.com/css2?family=VT323&display=swap" rel="stylesheet" />
		<link href="/cgi/library/css/layout.css" rel="stylesheet" />
	</head>
	<body>
		<h1>Form Submission Test</h1>
		<div class="container">
			<div class="test-section">
				<h2>POST Data Received</h2>
				<div class="cookie-list">
<?php
	if (!empty($_POST)) {
		echo "<ul>";
		foreach ($_POST as $key => $value) {
    		echo "<li><strong>" . htmlspecialchars($key) . ":</strong> " . htmlspecialchars($value) . "</li>";
		}
		echo "</ul>";
	} else {
		echo "<p>No data send.</p>";
	}
 ?>
				</div>
				<a href="/cgi/index.html#form">Back to Tests</a>
			</div>
		</div>
	</div>
</body>
</html>
