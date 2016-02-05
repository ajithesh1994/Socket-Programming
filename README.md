# Socket-Programming
Djikstra's MST | TCP and UDP Sockets

EE 450 Socket Programming project | Fall 2015 | Professor Zahid Ali

Full name: AJITHESH SRINIVASA RAGHAVAN
USC Student ID: 7444367200
Session: 1

Abstract:
---------

In this project, there are 4 servers and a client which are connected in a single network. The servers are connected with each other with cost lengths which vary. The server programs which are serverA.cpp, serverB.cpp, serverC.cpp , serverD.cpp read files from text files serverA.txt, serverB.txt,serverC.txt,serverD.txt correspondingly. These text files contains informations about how each server is connected to the others.

After reading all these informations from the files, each of the server program sends them to the client. The Client receives the topology information from each of the server and creates a overall network topology in the form of adjacency matrix. This adjacency matrix is broadcasted to all the 4 servers across the network.

Then the client calculates a minimum spanning tree of the topology and displays it.

Code files and working:
-----------------------

serverA.cpp->

Once the program is compiled and run, the server is supposed to be up and running. The program then opens the file "serverA.txt" and reads the contents of those files and puts them in a data type called VECTOR. ( This vector concept is implemented so that it will be easy to push the contents of the file easily into the data type and save them separately ) .

Once the string names and the integers are saved separately, the integers alone are put into an separate integer array with an array identifier in the front to show from which server the information is from. 

Then a TCP socket is created and the integer array is sent to the client after connecting with the client address using its well known static port number. Write() function is used to transfer the information. Then the TCP socket is closed since its use is done.

The server constantly runs on UDP. Hence an UDP socket is opened now, binded with a static port number and it keeps on running whilst waiting for information from the client. Using recvfrom() function the adjacency matrix is got from the client in the form another integer array and this is displayed in the server side in the form of Edges and costs. This conversion from 2*2 matrix to edge finding is done using upper traingular matrix concept.

serverB.cpp , serverC.cpp , serverD.cpp ->

All these 3 programs work in the same way as the above serverA.cpp except the fact that it will take the information input from different text files which has its own topology information.


client.cpp  -->

When the client program starts, the client host is supposed to be up and running. The client runs on a TCP server background. First, a TCP socket is created and bound with a static port number. Then it keeps waiting for information from the servers. Once it has received 4 different topology information from each of the 4 servers the loop closes. Until then the client keeps receiving the information and keeps putting them in an integer array which will be combined as an Adjacency matrix which will be broadcasted back again to all 4 servers.

This broadcasting of the adjacency matrix information is done using the UDP socket. Each UDP socket will be created while sending to one server, gets closed and opens again when we are sending to another server. This broadcasting using TCP socket also runs in a loop which gets executed 4 times for 4 servers with different static port numbers.

Once we have the graph information, the Minimum spanning tree algorithm is executed. Prims algorithm was used in my program to create a minimum spanning tree without any loops. The adjacency matrix is passed as an argument to a MST function which computes the tree and returns back and prints it.


Method to Run the programs:
---------------------------


Method 1 => ( MAKEFILE )

step1 : type "make clean"
step2 : type "make localhost" ( If the programs are downloaded into the local system and run"

	( While doing this the IPaddr in the #define section should be changed to "localhost"

Method 2 => Normal compilation

The programs can be run in the following order: ( In different terminals )

1. g++ client.cpp -o client -lnsl -lsocket -lresolv
2. g++ serverA.cpp -o serverA -lnsl -lsocket -lresolv
3. g++ serverB.cpp -o serverB -lnsl -lsocket -lresolv
4. g++ serverC.cpp -o serverC -lnsl -lsocket -lresolv
5. g++ serverD.cpp -o serverD -lnsl -lsocket -lresolv

The server A,B,C,D can be run in any order. But the client should be run first followed by the servers.

The static ports are hard coded in the # define section of the code. 
If the programs are run on nunki the IP address is defined as "nunki.usc.edu"
If the programs are run on a localhost the IP address is defined as "localhost".

Method 3 => ( Makefile in Nunki )

step1 : type "make clean"
step2 : type "make all"  ( Authentication required in Nunki for this )


Format of messages exchanged:
----------------------------

All the exchanges in my program takes place by passing integer arrays only. 

Server to client ( TCP )
------------------------

integer array[] = {server_identifier_number, cost1, cost2, cost3, cost4}

Client to Server ( UDP ) ( Adjacency matrix information ) 
-------------------------

integer array[]= { cost1, cost2, cost3 .... cost 16} . ( 16 elements for the 4*4 matrix )


Idiosyncrasy :
------------

The code would not fail in any condition except the fact that if the cost lengths are given in any range beyond the INT values. Even if Long int is used in the client program to give the sum of the cost lenghts to give the cost of the tree, nunki has the same range values for int and long int.

Some of the string manipulations and vectors would not work in C programming. Hence C++ is used.


Re-Used code:
------------

All the snippets in the programs were typed by me. Some of the socket functions were referred from the beej tutorial. All the logic part used in the program was typed by me. The Algorithm for the minimum spanning tree is Prims Algorithm and it was referred from the internet and the website geeksforgeeks.
