<?php
parse_str(file_get_contents("php://input"), $data);
print_r($data);
print_r($_SERVER);
print_r($_POST);
$uploadDir = 'uploads/';
$maxFileSize = 100 * 1024 * 1024; // 2MB
$allowedTypes = ['jpg', 'png', 'PNG', 'pdf', 'txt', 'JPG', 'csv', 'CSV', 'sql', 'SQL'];

header('Content-Type: text/html; charset=utf-8');
?>
<!DOCTYPE html>
<html>
<head>
    <title>Upload Result</title>
</head>
<body>
    <h1>File Upload Results</h1>
    <?php
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        if (!file_exists($uploadDir) && !mkdir($uploadDir, 0755, true)) {
            die('<p class="error">Failed to create upload directory</p>');
        }

        $fileName = basename($_FILES['fileToUpload']['name']);
        $targetFile = $uploadDir . $fileName;
        $fileType = strtolower(pathinfo($targetFile, PATHINFO_EXTENSION));
        $fileSize = $_FILES['fileToUpload']['size'];

		echo $fileName . ": " . $fileSize . ": " . $_FILES['fileToUpload']['tmp_name'];

        // Validation checks
        if ($fileSize > $maxFileSize) {
            echo '<p class="error">File too large (max 2MB)</p>';
        } elseif (!in_array($fileType, $allowedTypes)) {
            echo '<p class="error">Invalid file type. Allowed: ' . implode(', ', $allowedTypes) . '</p>';
        } elseif (move_uploaded_file($_FILES['fileToUpload']['tmp_name'], $targetFile)) {
            echo '<p class="success">File uploaded successfully</p>';
            echo "<p>File Info:</p>";
            echo "<pre>";
            print_r($_FILES['fileToUpload']);
            echo "</pre>";
        } else {
            echo '<p class="error">Error uploading file</p>';
        }
    }
    ?>
    <p><a href="index.html">Back to Tests</a></p>
</body>
</html>
