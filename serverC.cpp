// Socket programming project | Ajithesh | ServerA
//Calling all the header functions
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <strings.h> 
#include <netdb.h> 
#include <vector>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <sys/wait.h> 
#include "iostream"

//Defining all static ports and the Ip address
#define port1 21200
#define port2 22200
#define port3 23200
#define port4 24200
#define port5 25200
#define IPaddr "nunki.usc.edu"

using namespace std;
//To throw error messages
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
// A Class to get the input from a file which has a string and an integer to get servername and cost
class input 
{
    public:
        string servername;
        int cost;
};
// The main starts here
int main ()
{

    // Parameters and variables required for File reading operations
    int ch;
    int cost,i=0,counter=0;
    char name[32];
    char filename1[]="serverC.txt";
    FILE *fp;
    std::vector<input> v; //Vector
   
    // Parameters and variables required for sockets
    int socktcp,sockudp;
    int n;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    socklen_t servlen = sizeof(serv_addr);   
    char buffer[256];
    char buffer1[256];
    struct hostent *client;
    struct hostent *server;

    printf(" \nThe Server C is up and running");
    int costarray[5]={3,0,0,0,0}; // The array which is going to be transmitted

    fp = fopen(filename1, "r");  // Opening the file 

    if (fp == NULL)
        {
            printf ("Error opening the file ServerC.txt\n\n'");
            exit(EXIT_FAILURE);
        } 
    else 
        {    
            while (feof(fp)==0)
               {
                ch = fscanf(fp, "%s %d\n", name, &cost); // Reading the contents of the file
               
                input v1;       
                v1.servername = name; // Saving the values in a vector
                v1.cost = cost;
                if(strcmp(name,"serverA")==0)
                    costarray[1]=cost;
                else if(strcmp(name,"serverB")==0)
                    costarray[2]=cost;    
                else if(strcmp(name,"serverC")==0)
                    costarray[3]=cost;
                else if(strcmp(name,"serverD")==0)
                    costarray[4]=cost;
                v.push_back(v1);
                
                }
            
        }
         fclose(fp); // Closing the file

        printf(" \nThe Server C has the following neighbour information: \nNeighbour \t Cost \n");
        for(int i=0;i<v.size();i++) 
         {
            strcpy(buffer,v[i].servername.c_str());
            
            cout << v[i].servername<<" \t "; // Printing the server information
        
            cout << v[i].cost << "\n";
         
            bzero(buffer,sizeof(buffer));
         }

         // Creating a TCP socket to send server information to client
         // From Beej tutorial
        socktcp = socket(AF_INET,SOCK_STREAM,0); 
        if(socktcp<0)
        error("TCP Socket Creation Failed\n");
        
        client = gethostbyname(IPaddr); // Getting the IP address of the host
        if(client == NULL)
        error("Host name Failed \n");

        memset(cli_addr.sin_zero,'\0',sizeof cli_addr.sin_zero);
        cli_addr.sin_family = AF_INET;  
        cli_addr.sin_port = htons(port5);
        bcopy((char *)client->h_addr,(char *)&cli_addr.sin_addr.s_addr,client->h_length);
   
        if(connect(socktcp,(struct sockaddr *) &cli_addr,sizeof(cli_addr))<0)
            {
                error(" Server C tried connecting with client but in vain \n");
            }
   
        getsockname(socktcp, (struct sockaddr *) &cli_addr, &clilen); //To get the port number of the dynamic TCP socket

        write(socktcp,(char *)costarray,5*sizeof(int)); //Sending the server information to the client

        printf(" \nThe Server C finishes sending its neighbor information "
                "to the Client with TCP port number %d and IP address %s  \nFor "
                "this connection with the Client, the Server C"
                " has ​TCP port number %d and IP address %s"
                ,port5,inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port),inet_ntoa(cli_addr.sin_addr));

        close(socktcp); //Closing the TCP socket once everything is sent

        // Creating an UDP socket which runs in the server all time and waits for infromation
        // Snippets from Beej tutorial
        sockudp = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockudp < 0) 
            {
                error(" The socket is not created \n");

            }

        server=gethostbyname(IPaddr);
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
        serv_addr.sin_port = htons(port3);

        if (bind(sockudp,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
                {
                  error("ERROR in bind \n");
                 
                }
           
        int adjmat[16]; // Matrices for adjacency matrices
        int edgemat[4][4];

        // Receiving values from the client through the UDP socket
        n=recvfrom(sockudp,adjmat,16*sizeof(int),0,(struct sockaddr *) &cli_addr,&clilen);
        if(n<0) error("\nReceiving from client via UDP failed\n");

        getpeername(sockudp,(struct sockaddr *) &cli_addr,&clilen); // To get the port number in the client side
        
        i=0;
        for( int j=0;j<4;j++)
        {
            for(int k=0;k<4;k++)
            {
        
                edgemat[j][k]=adjmat[i];
                i++;
            }

        }
        printf("\nThe server C has received the network topology from the client with UDP port number "
                "%d and the IP address %s as follows:",ntohs(cli_addr.sin_port),inet_ntoa(cli_addr.sin_addr));
        printf("\nEdge \t Cost\n");
        for(int j=0;j<4;j++)
        {
            for(int k=0;k<4;k++)
            {
                if(edgemat[j][k]!=0)
                {
                    if(j<k)
                    printf("%c%c \t %d \n",(j+65),(k+65),edgemat[j][k]);
                }
            }
        
        }
        printf("\nFor this connection with client, The Server C has UDP port number %d and IP address %s \n",port3,inet_ntoa(cli_addr.sin_addr));      
        close(sockudp);
        
        return 0;
}
