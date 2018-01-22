# myCloud
A client-server based application that runs and saves binary files over the internet. Project adopted from Systems Programming class


To compile, enter the directory of the client or server and type:
make

To run the client:
./client <address> <port> <secretkey>

To run the server:
./server <port> <secretkey>

These must both be running at the same time for successful execution!

Known Bugs:
Upon compile for server, it prompts error with unused port variable, however this variable is set and used later on in the program.


The csapp.c and csapp.h files are from source files found in the textbook:
Computer Systems: A Programmer's Perspective Third Edition by Randal Bryant and David O'Hallaron
