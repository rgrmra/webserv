<?php
header('Content-Type: text/html; charset=utf-8');
?>
<!DOCTYPE html>
<html>
<head>
    <title>Form Submission Result</title>
</head>
<body>
    <h1>Form Submission Results</h1>
    <p>POST Data Received:</p>
    <pre><?php print_r($_POST); ?></pre>
    <p><a href="index.html">Back to Tests</a></p>
</body>
</html>
