# FTP_Client_Server
Implementation of FTP Client Server in C. Includes custom client and server programs that provide functionality to authenticate a user, list remote files, and retrieve remote files
To compile and link ftserver:

	$ cd server/
	$ make
To compile and link ftclient:

	$ cd client/
	$ make
To run ftserve:

	$ server/ftserver PORTNO
To run ftclient:

	$ client/ftclient HOSTNAME PORTNO

	Commands:
		list
		get <filename>
		quit
Available commands:

list            - retrieve list of files in the current remote directory
get <filename>  - get the specified file
quit            - end the ftp session
Logging In:

	Name: anonymous
	Password: [empty]