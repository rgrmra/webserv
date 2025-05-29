import platform
import sys

print("Content-Type: text/html\n")
print(f"""
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>Tic Tac Toe</title>
        <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap" rel="stylesheet" />
        <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined"/>
        <link href="/cgi/library/css/layout.css" rel="stylesheet">
    </head>
    <body>
        <h1>Python CGI</h1>
        <div class="container">
            <div class="test-section">
                <h2>Python Info</h2>
                <div class="cookie-list">
                    <ul>
                        <li><strong>Python Version: </strong>{sys.version}</li>
                        <li><strong>Version Info: </strong>{sys.version_info}</li>
                        <li><strong>Platform: </strong>{platform.platform()}</li>
                        <li><strong>Processor: </strong>{platform.processor()}</li>
                        <li><strong>System: </strong>{platform.system()}</li>
                        <li><strong>Machine: </strong>{platform.machine()}</li>
                    </ul>
                </div>
                <a href="/cgi/index.html#cgi_version">Back to Tests</a>
            </div>
        </div>
    </body>
</html>
""")
