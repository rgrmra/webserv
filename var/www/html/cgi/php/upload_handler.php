<?php
	header("Content-Type: text/html; charset=utf-8");

	$uploadDir = "../../uploads/";
	$maxFileSize = 2 * 1024 * 1024;
	$allowedTypes = [ "jpg", "jpeg", "png", "gif", "pdf", "txt", "csv", "sql", ];
?>

<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>File Upload Test</title>
		<link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap" rel="stylesheet" />
		<link href="/cgi/library/css/layout.css" rel="stylesheet" />
	</head>
	<body>
		<h1>File Upload Test</h1>
		<div class="container">
			<div class="test-section">
				<h2>Upload Status</h2>
<?php
	if ($_SERVER["REQUEST_METHOD"] === "POST") {
		if (!isset($_FILES["fileToUpload"]) || $_FILES["fileToUpload"]["error"] !== UPLOAD_ERR_OK) {
			echo '<p class="message error">No file uploaded or an upload error occurred.</p>';
		} else {
			$fileName = basename($_FILES["fileToUpload"]["name"]);
			$targetFile = $uploadDir . $fileName;
			$fileType = strtolower(pathinfo($targetFile, PATHINFO_EXTENSION));
			$fileSize = $_FILES["fileToUpload"]["size"];
			$tmpName = $_FILES["fileToUpload"]["tmp_name"];
			if (!file_exists($uploadDir) && !mkdir($uploadDir, 0755, true)) {
				echo '<p class="message error">Failed to create upload directory. Please check server permissions.</p>';
			} else if ($fileSize > $maxFileSize) {
				echo '<p class="message error">File is too large. Maximum allowed size is 2MB.</p>';
			} else if (!in_array($fileType, $allowedTypes)) {
				echo '<p class="message error">Invalid file type. Allowed types are: ' . implode(", ", $allowedTypes) . '.</p>';
			} else if (file_exists($targetFile)) {
				echo '<p class="message error">Sorry, file already exists. Please rename your file or delete the existing one.</p>';
			} else if (move_uploaded_file($tmpName, $targetFile)) {
				echo '<p class="message success">File <strong>"' . htmlspecialchars($fileName) . '"</strong> uploaded successfully!</p>';
			} else {
				echo '<p class="message error">Error uploading file. Unknown issue.</p>';
			}
			echo '<div class="cookie-list">';
			echo '<ul>';
			foreach ($_FILES['fileToUpload'] as $key => $value) {
				echo '<li><strong>' . htmlspecialchars($key) . ':</strong> ' . htmlspecialchars($value) . '</li>';
			}	
			echo '</ul>';
			echo '</div>';
		}
	}
?>
				<a href="/cgi/index.html#upload">Back to Tests</a>
			</div>
		</div>
	</body>
</html>
