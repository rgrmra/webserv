print('Content-Type: text/html\n')
print(f"""
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
        <h1>Directory Test</h1>
        <div class="container">
            <div class="test-section">
                <h2>Directory</h2>
                <iframe src="/directory"></iframe>
                <a href="/cgi/index.html#directory">Back to Tests</a>
            </div>
        </div>
    </body>
</html>
""")
