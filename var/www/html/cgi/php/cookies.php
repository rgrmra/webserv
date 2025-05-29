<?php
	if ($_SERVER["REQUEST_METHOD"] === "POST" && isset($_POST["set_cookie"])) {
		$name = trim($_POST["cookie_name"] ?? "");
		$value = trim($_POST["cookie_value"] ?? "");
		if ($name && $value) {
			setcookie($name, $value, [
				"expires" => time() + 3600,
				"path" => "/",
				"secure" => isset($_SERVER["HTTPS"]),
				"httponly" => true,
				"samesite" => "Lax",
			]);
			header("Location: " . $_SERVER["PHP_SELF"]);
			exit();
		}
	}
	
	if ($_SERVER["REQUEST_METHOD"] === "POST" && isset($_POST["delete_last_cookie"])) {
		if (!empty($_COOKIE)) {
			$lastCookieName = array_key_last($_COOKIE);
			setcookie($lastCookieName, "", [
				"expires" => time() - 3600,
				"path" => "/",
				"secure" => isset($_SERVER["HTTPS"]),
				"httponly" => true,
				"samesite" => "Lax",
			]);
		}
		header("Location: " . $_SERVER["PHP_SELF"]);
		exit();
	}
?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
    	<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>Cookie Test</title>
		<link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap" rel="stylesheet">
		<link href="/cgi/library/css/layout.css" rel="stylesheet">
	</head>
	<body>
		<h1>Cookie Test</h1>
		<div class="container">
			<div class="test-section">
				<h2>Set a Cookie</h2>
				<form method="POST">
					<div class="form-group">
						<label for="cookie_name">Name:</label>
						<input type="text" name="cookie_name" id="cookie_name" placeholder="e.g., username" required>
					</div>
					<div class="form-group">
						<label for="cookie_value">Value:</label>
						<input type="text" name="cookie_value" id="cookie_value" placeholder="e.g., JohnDoe" required>
					</div>
					<input type="submit" name="set_cookie" value="Upload File" />
				</form>
				<h3>Actions</h3>
				<form method="POST" style="display: inline;">
					<div class="form-group">
						<input type="submit" name="delete_last_cookie" value="Delete Last Cookie" />
					</div>
				</form>
				<div class="cookie-list">
					<h3>Current Cookies</h3>
					<?php
						 if (!empty($_COOKIE)) {
							echo "<ul>";
							foreach ($_COOKIE as $name => $value) {
								echo "<li><strong>" . htmlspecialchars($name) . ":</strong> " . htmlspecialchars($value) . "</li>";
							}
							echo "</ul>";
						} else {
							echo "<p>No cookies set for this domain.</p>";
						}
					 ?>
				</div>
				<a href="/cgi/index.html#cookie">Back to Tests</a>
			</div>
		</div>
	</body>
</html>
