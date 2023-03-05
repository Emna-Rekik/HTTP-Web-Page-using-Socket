CC := gcc

main : HttpServer HttpClient

HttpServer : HttpServer.c
	$(CC) HttpServer.c -o HttpServer
	
Httpclient : HttpClient.c
	$(CC)  HttpClient.c -o HttpClient
