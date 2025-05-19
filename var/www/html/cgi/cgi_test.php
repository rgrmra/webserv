<?php
header('Content-Type: text/html; charset=utf-8');
?>
<!DOCTYPE html>
<html>
<head>
    <title>CGI Test Result</title>
</head>
<body>
    <h1>CGI Test Results</h1>
    <p>Query Parameters Received:</p>
    <pre><?php print_r($_GET); ?></pre>
    
    <p>Server Environment:</p>
    <pre><?php print_r($_SERVER); ?></pre>
    
    <p><a href="index.html">Back to Tests</a></p>
</body>
</html>
