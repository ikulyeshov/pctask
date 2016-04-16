Set of utils that demostrate connection of client to the server.
Multiconnection is supported.
Client have two cameras and simulates video stream to the server.
Each camera supports several modes according task description in doc dir.
Aplpication is tested on Linux under Ubuntu environment.

You can find task description under doc dir.


Usage:
------

Just 'make' in the root dir.
Couple executable should be created:
	1. Client: main/obj/main
	2. Server: server/obj/server

'make clean' cleans temporary files.


Tests:

Client and servers should be run from the terminal since they use output/input. Use separate terminals for each client and server.
You can start any number of client you wish.

First run server. Server command line accepts port number to listen incoming connections (default to 4000):
		
		server/obj/server 4000

than run clients. Client command line accepts two parameters: address and port to connect to (defaults are 127.0.0.1 and 4000).

		main/obj/main 127.0.0.1 4000

Connection can work accross the network between different computers.

Observe log and status messages on the screen.

Main control is done from server concole. Use key strokes:

	'q' 		Exit
	's' 		Start stream for the the current camera (default is first totally 2) and current client (default is first). 
			Frame message start comming from the client to the server and console. User can start as many streams from any 
			client as manu client connected. All thh messages will spam to the server console
	't' 		Stop stream for the current camera and current client. Frame messages stop comming to the server and console
	'p' 		Print all available connections to the console
	'Shift +'/'-'	Change current client
	'>'/'>' 	Change current camera (totally two)
	'a'		Send request for current camera and client capabilities. Outputs to the console
	'o'		Change QOS bitrate. Camera downgrades resolution or upgrades resolution accordingly.


Camera #1 is configured for overheat interval and will be switched off after 10 sedonds elapsed
Server will be notified. Check output in the console.

Todo:
-----
	1. No correct shutdown for connection and applications itself.
	2. No mutex protection for writing to the socket from different thread e.g. when signaling overheat from other thread.
	3. Apps require code review and tuning.
	4. No error and status handling in some places.
	5. No log level control.




