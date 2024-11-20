(lldb) target create "./webserv"
Current executable set to '/home/ibaby/Desktop/circle_5/webserv/webserv' (x86_64).
(lldb) settings set -- target.run-args  "configExample.conf"
Process 72786 launched: '/home/ibaby/Desktop/circle_5/webserv/webserv' (x86_64)
---------------------[NEW SERVER ADDED]---------------------
listen:  8080
server_names:  42.fr www.42.fr
root:  URIs
index:  index.html
uploads_folder:  /var/web/ups
allowed_methods:   GET DELETE POST OPTIONS
error_pages:  404 error 401 401.html 500 501.htm /var/web/error_pages
auto_index:  on
cgi:  .py /bin/python3
cgi:  .sh /bin/bash
cgi:  .php /bin/php8.1
client_max_body_size:  10
location /cgi-bin
{
	root: /var/web/
	index: index.html
	autoIndex: true
	cgi: .py /bin/python3
	redirect: default
	allowed Methods: GET POST 
	max body size: default
	file upload: false
	upload folder: default
}

location /uploads
{
	root: /var/web/site_0
	index: index.html
	autoIndex: false
	cgi: default
	redirect: default
	allowed Methods: GET POST DELETE OPTIONS 
	max body size: default
	file upload: true
	upload folder: /var/web/ups
}

location /dieri
{
	root: default
	index: dieriba.html
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=7ij8Qj4JLjI
	allowed Methods: GET POST 
	max body size: 1024
	file upload: false
	upload folder: default
}

location /rayan
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=5Fj-VIslrFc
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /skibidi
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=WePNs-G7puA
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /soundous
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=rzeZcdW4Pbo
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /mouslim
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/shorts/SqXXwcs8iyQ
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /rico
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=akU7HXvYft8
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /olivia
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=cordADXcng0
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /anastasia
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=7ij8Qj4JLjI
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /moussa
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/shorts/hfaGQeX-bE4
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /mohamed
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=0cJjmXEhG-Y
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /dasha
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=LjxulQ1bEWg
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /alix
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=MaFEHf34fCQ
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /ana
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/shorts/Nx9epM1I_Q8
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /lomaubob
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=2XOz8ZhOoWo
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

location /naomie
{
	root: default
	index: default
	autoIndex: false
	cgi: default
	redirect: https://www.youtube.com/watch?v=2EwViQxSJJQ
	allowed Methods: none
	max body size: default
	file upload: false
	upload folder: default
}

---------------------[NEW SERVER ADDED]---------------------
listen:             8081
server_names:     localhost
root:  URIs
index:              index.html
auto_index:         on
allowed_methods:     GET POST DELETE
---------------------[NEW SERVER ADDED]---------------------
listen:             8082
server_names:     dieriba.fr
index:             index.html
root:  URIs
allowed_methods:     GET POST DELETE
auto_index:         on
---------------------[NEW SERVER ADDED]---------------------
listen:             8083
server_names:     localhost
index:             index.html
root:  URIs
allowed_methods:     POST DELETE
auto_index:         on
---------------------[NEW SERVER ADDED]---------------------
listen:             8084
server_names:     localhost
index:             index.html
root:  URIs
allowed_methods:     GET POST DELETE
auto_index:         on
---------------------[NEW SERVER ADDED]---------------------
listen:             8085
server_names:     localhost
auto_index:         on
index:             index.html
root:  URIs
allowed_methods:     GET POST DELETE
---------------------[NEW SERVER ADDED]---------------------
listen:             8086
server_names:     localhost
index:             index.html
auto_index:         on
root:  URIs
allowed_methods:     GET POST DELETE
---------------------[NEW SERVER ADDED]---------------------
listen:             8087
server_names:     localhost
index:             index.html
auto_index:         on
root:  URIs
allowed_methods:     GET POST DELETE
---------------------[NEW SERVER ADDED]---------------------
listen:             8088
server_names:     localhost
index:             index.html
auto_index:         on
root:  URIs
allowed_methods:     GET POST DELETE
---------------------[NEW SERVER ADDED]---------------------
listen:             8089
server_names:     localhost
index:             index.html
auto_index:         on
root:             /var/web/site_8/
allowed_methods:     GET POST DELETE
---------------------[NEW SERVER ADDED]---------------------
listen:             8090
server_names:     localhost
index:             index.html
auto_index:         on
root:             /var/web/site_9/
allowed_methods:     GET POST DELETE
PARSING OK!
server on : http://127.0.0.1:8080
server on : http://127.0.0.1:8081
server on : http://127.0.0.1:8082
server on : http://127.0.0.1:8083
server on : http://127.0.0.1:8084
server on : http://127.0.0.1:8085
server on : http://127.0.0.1:8086
server on : http://127.0.0.1:8087
server on : http://127.0.0.1:8088
server on : http://127.0.0.1:8089
server on : http://127.0.0.1:8090
Process 72786 stopped
* thread #1, name = 'webserv', stop reason = signal SIGSTOP
    frame #0: 0x00007ffff7b6bdea libc.so.6`epoll_wait + 26
libc.so.6`epoll_wait:
->  0x7ffff7b6bdea <+26>: cmpq   $-0x1000, %rax            ; imm = 0xF000 
    0x7ffff7b6bdf0 <+32>: ja     0x7ffff7b6be50            ; <+128>
    0x7ffff7b6bdf2 <+34>: retq   
    0x7ffff7b6bdf3 <+35>: nopl   (%rax,%rax)
^C
^C

