<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);
    header("Content-Type: text/html; charset=utf-8");

    $uploadDir = '../../uploads/';
    $message = '';

    if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['delete_files'])) {
        if (!empty($_POST['files'])) {
            foreach ($_POST['files'] as $fileName) {
                $filePath = $uploadDir . basename($fileName);
                if (file_exists($filePath)) {
                    if (unlink($filePath)) {
                        $message .= "<p>Deleted: <strong>" . htmlspecialchars($fileName) . "</strong></p>";
                    } else {
                        $message .= "<p>Could not delete: <strong>" . htmlspecialchars($fileName) . "</strong></p>";
                    }
                } else {
                    $message .= "<p>File not found: <strong>" . htmlspecialchars($fileName) . "</strong></p>";
                }
            }
        } else {
            $message = "<p>No files selected for deletion.</p>";
        }
    }

    $files = array_diff(scandir($uploadDir), array('.', '..'));
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>Delete Uploaded Files Test</title>
        <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap" rel="stylesheet" />
        <link href="/cgi/library/css/layout.css" rel="stylesheet" />
    </head>
    <body>
        <h1>Delete Uploaded Files Test</h1>
        <div class="container">
            <div class="test-section">
                <h2>Files in Upload Directory</h2>
                <form method="POST" action="delete_files.php">
                    <div class="cookie-list">
                        <?php if (!empty($files)): ?>
                            <ul>
                                <?php foreach ($files as $file): ?>
                                    <li>
                                        <label>
                                            <input type="checkbox" name="files[]" value="<?= htmlspecialchars($file) ?>" />
                                            <?= htmlspecialchars($file) ?>
                                        </label>
                                    </li>
                                <?php endforeach; ?>
                            </ul>
                        <?php else: ?>
                            <p>No files found in the upload directory.</p>
                        <?php endif; ?>
                    </div>
                    <input type="submit" name="delete_files" value="Delete Selected Files"/>
                </form>
                <div class="result-message">
                    <?= $message ?>
                </div>
                <a href="/cgi/index.html#delete">Back to Tests</a>
            </div>
        </div>
    </body>
</html>
