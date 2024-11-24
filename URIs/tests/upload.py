#!/usr/bin/python3

import os
import sys

content_length = os.getenv('CONTENT_LENGTH')
content_type = os.getenv('CONTENT_TYPE')
upload_save = os.getenv('UPLOAD_SAVE')

if ((not content_length) or (not content_type) or (not upload_save)):
	sys.exit(1)

boundary = content_type.split("boundary=")[-1]
boundary_b = b"\r\n--" + boundary.encode()
input_data = sys.stdin.buffer.read(int(content_length))

split_input = input_data.split(boundary_b)

for content in split_input:
	if b"filename" in content:
		filename = content.split(b"filename=\"")[1].split(b"\"")[0].decode()
		file_content = content.split(b"\r\n\r\n")[1]

		file_path = '.' + os.path.join(os.path.dirname(__file__), upload_save, filename)
		os.makedirs(os.path.dirname(file_path), exist_ok=True)
		with open(file_path, 'wb') as file:
			file.write(file_content)

		body = "File succesfully uploaded !"
		body_length = len(body)

		print("Content-Type: text/plain\r")
		print(f"Content-Length: {body_length}\r\n\r")
		print(f"{body}")
		sys.exit(0)

sys.exit(1)
