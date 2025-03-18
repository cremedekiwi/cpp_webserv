import sys
import os
from io import BytesIO
import warnings

warnings.filterwarnings("ignore", category=DeprecationWarning)
import cgi  # noqa

# Get the content length from environment variables
content_length = int(os.environ.get("CONTENT_LENGTH", 0))
post_data = sys.stdin.read(content_length)

parsed = cgi.FieldStorage(
    BytesIO(post_data.encode("utf-8")),
    headers={"content-type": os.environ.get("CONTENT_TYPE")},
    environ={"REQUEST_METHOD": "POST"},
)

fname = parsed["fname"].value  # type: ignore
lname = parsed["lname"].value  # type: ignore

print("Content-Type: text/html")
print()
print(f"""<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Form Response</title>
</head>
<body>
    <h1>Hello, {fname} {lname}!</h1>
    <p>Thank you for submitting your details with a {os.environ.get("CONTENT_TYPE")} .</p>
</body>
</html>
""")
