import socket
import time


def send_chunked_request(host="127.0.0.2", port=18000):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        sock.connect((host, port))

        headers = (
            "GET /cgi-bin/back.sh HTTP/1.1\r\n"
            f"Host: {host}:{port}\r\n"
            "Transfer-Encoding: chunked\r\n"
            "Connection: close\r\n"
            "\r\n"
        )
        # sock.send(headers.encode())
        line = headers.encode()

        # time.sleep(1)  # Simulate processing time between chunks
        # chunks = ["First chunk of data", "Second chunk of data", "Final chunk of data"]
        chunks = [
            "<!DOCTYPE html>",
            '<html lang="en">',
            "<head>",
            '<meta charset="UTF-8">',
            '<meta name="viewport" content="width=device-width, initial-scale=1.0">',
            "<title>CHUNKS</title>",
            '<link rel="stylesheet" href="style.css">',
            '<link rel="preconnect" href="https://fonts.googleapis.com">',
            '<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>',
            "<link",
            'href="https://fonts.googleapis.com/css2?family=Merriweather:ital,wght@0,300;0,400;0,700;0,900;1,300;1,400;1,700;1,900&display=swap"',
            'rel="stylesheet">',
            "</head>",
            "<body>",
            '<div class="center">',
            '<p id="get">That a lot of chunks !</p>',
            "</div>",
            "</body>",
            "</html>",
        ]
        for chunk in chunks:
            size = hex(len(chunk))[2:]
            line += size.encode() + b"\r\n"
            line += chunk.encode() + b"\r\n"
            sock.send(line)
            print(line)
            line = b""
            time.sleep(0.01)

        # Send the final empty chunk to indicate the end
        sock.send(b"0\r\n\r\n")

        # Receive and print the response
        response = b""
        while True:
            data = sock.recv(4096)
            if not data:
                break
            response += data

        print("Server response:", response.decode())

    finally:
        sock.close()


if __name__ == "__main__":
    try:
        send_chunked_request()
    except Exception as e:
        print(f"Error: {e}")
