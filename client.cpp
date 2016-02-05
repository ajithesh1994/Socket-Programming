    // Socket programming project | Ajithesh
    // The required header files are called

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <strings.h>
    #include <errno.h> 
    #include <unistd.h>
    #include <netdb.h> 
    #include <vector>
    #include <sys/types.h> 
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h> 
    #include <sys/wait.h> 
    #include <limits.h>
    #include "iostream"

    //The static port numbers and the IP address is defined ( Hard coded )
    #define port1 21200
    #define port2 22200
    #define port3 23200
    #define port4 24200
    #define port5 25200
    #define IPaddr "nunki.usc.edu"

    //To throw error messages
    void error(const char *msg)
    {
        perror(msg);
        exit(1);
    }

    // The main program starts here
    int main(int argc, char *argv[])
    {
        // The variables requird for the sockets
         int socktcp_client, newsockfd ,sockudp_client;
         socklen_t clilen;
         char buffer[256];
         struct sockaddr_in serv_addr, cli_addr,temp_addr;
         struct hostent *client;
         struct hostent *server;

         // Variables required for adjacency matrix and topology details and MST
         int n,count=0,count1=0;
         int costarray[5];
         int adjmat[4][4];
         int broadcastadjmat[16];
         
        // Opening a TCP socket
         socktcp_client = socket(AF_INET, SOCK_STREAM, 0);
         if (socktcp_client < 0) 
            error("ERROR opening socket\n");
           
         bzero((char *) &serv_addr, sizeof(serv_addr));
         serv_addr.sin_family = AF_INET;
         serv_addr.sin_port = htons(25200);
         //Get's the IP address of the host
         server=gethostbyname(IPaddr);

         if(server == NULL)
            error("Host name Failed \n");

         bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);

         if (bind(socktcp_client, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
            error("\nERROR on binding");

         printf("\nClient has TCP port number %d and IP address %s\n",port5,inet_ntoa(serv_addr.sin_addr));
         listen(socktcp_client,5);
         // Get's the address of the client address
         clilen = sizeof(cli_addr);

         // This loop runs 4 times. i.e for 4 servers. Reads from 4 servers
         
         while(count<4)
         {

         newsockfd = accept(socktcp_client,(struct sockaddr *) &cli_addr,&clilen);
         if (newsockfd < 0) 
            error("\n ERROR on accept");
        
         bzero(buffer,256);
         n = read(newsockfd,costarray,5*sizeof(int));
         if (n < 0) error(" \n ERROR reading from socket");
         else count++; // Increases count to show that information has been read from one server.

         // Getpeername to get the server's port number

         getpeername(socktcp_client, (struct sockaddr *) &cli_addr, &clilen);
         printf(" \nThe Client receives neighbour information from Server %c with TCP port number %d and IP address %s\n"
            ,(costarray[0]+65-1),ntohs(cli_addr.sin_port),inet_ntoa(cli_addr.sin_addr));
         printf(" \nServer %c has the following neighbour information: \nNeighbour \t Cost \n",(costarray[0]+65-1));

         // To form the adjacncy matrix
         for (int i=0;i<4;i++)
            {
                adjmat[(costarray[0])-1][i]=costarray[i+1];
                if(costarray[i+1]!=0)
                {
                    printf("Server %c \t %d \n",(i+65),costarray[i+1]);
                }


            }
            printf("\nFor this connection with the Server %c, The Client has TCP port number %d and IP address %s"
                ,(costarray[0]-1+65),port5,inet_ntoa(cli_addr.sin_addr));
        }
            
        int i=0;
        for( int j=0;j<4;j++)
        {
            for(int k=0;k<4;k++)
            {
           //     printf("%d \t",adjmat[j][k]); // To print the adjacency matrix
                broadcastadjmat[i]=adjmat[j][k];
                i++;
            }
            //printf("\n");
        }

        // Opening the UDP socket
         sockudp_client = socket(AF_INET, SOCK_DGRAM, 0);
         if (sockudp_client < 0) 
         error(" \nERROR opening socket");

    // Get the IP address of host
         client = gethostbyname(IPaddr);

         if(client == NULL)
            error("\n Host name Failed");

         bzero((char *) &cli_addr, sizeof(cli_addr));
         cli_addr.sin_family = AF_INET;
         bcopy((char *)client->h_addr,(char *)&cli_addr.sin_addr.s_addr,client->h_length);

    // Sending the neighbor information back to all servers
         while(count1<4)
         {

            sockudp_client = socket(AF_INET, SOCK_DGRAM, 0);
            if (sockudp_client < 0) 
                error(" \nERROR opening socket");

            client = gethostbyname(IPaddr);

            if(client == NULL)
                error("\n Host name Failed");

         

             bzero((char *) &cli_addr, sizeof(cli_addr));
             cli_addr.sin_family = AF_INET;
             bcopy((char *)client->h_addr,(char *)&cli_addr.sin_addr.s_addr,client->h_length);

             memset(cli_addr.sin_zero,'\0',sizeof cli_addr.sin_zero);
             bzero((char *) &cli_addr, sizeof(cli_addr));
             cli_addr.sin_family = AF_INET;
             bcopy((char *)client->h_addr,(char *)&cli_addr.sin_addr.s_addr,client->h_length);

         // Changing the port number every iteration
             cli_addr.sin_port = htons(port1+(count1*1000));

        //Sending the adjacency matrix values to servers using Integer array             
             n=sendto(sockudp_client,(char *) broadcastadjmat,16*sizeof(int),0,(struct sockaddr *) &cli_addr,sizeof(cli_addr));
             if(n<0) error("\n Sending to server via UDP failed");

             printf("\nThe Client has sent the network topology to the Server %c with UDP port number "
                    "%d and the IP address %s as follows:",(count1+65),ntohs(cli_addr.sin_port),inet_ntoa(cli_addr.sin_addr));
             printf("\nEdge \t Cost\n");
                for(int j=0;j<4;j++)
                     {
                        for(int k=0;k<4;k++)
                            {
                                if(adjmat[j][k]!=0)
                                    {
                                        if(j<k)
                                        printf("%c%c \t %d \n",(j+65),(k+65),adjmat[j][k]);
                                    }
                            }
                    }

        if(connect(sockudp_client,(struct sockaddr *) &cli_addr,sizeof(cli_addr))<0)
        {
            error(" Server A tried connecting with client but in vain \n");
        }

        socklen_t len = sizeof(temp_addr);
        getsockname(sockudp_client,(struct sockaddr *) &temp_addr,&len);
        int dynamic_udpport=ntohs(temp_addr.sin_port); 

        printf("\nFor this connection with the Server %c,the Client has UDP port number %d and IP address %s \n"
            ,(count1+65),dynamic_udpport,inet_ntoa(temp_addr.sin_addr));

         count1++; // Incrementing count every time so that the information is sent to the next server
         close(sockudp_client);

        } //end of while loop

        // Phase 3 : To calculate the minimum spanning tree. Using prims algorithm

         int calculatemst(int adjmat[4][4]);
         calculatemst(adjmat);  // Calls a function which performs the MST

         close(newsockfd);
         close(socktcp_client);
         
        
         return 0; 
    }


    // A function to calculate the minimum index value.

    int findmin(int mstcost[],int mstarray[])
        {
            int min=INT_MAX;
            int minindex;
            for(int i=0;i<4;i++)
                {
                    if(mstarray[i]==0 && mstcost[i]<min )
                    {
                        min=mstcost[i];
                        minindex=i;
                    }
                }
                return(minindex);
        }

    // Uses Prims algorithm to find the MST of a given adjacency matrix . Reference from Geeksforgeeks

    int calculatemst(int adjmat[4][4])
        {
            int mstarray[4]={0,0,0,0};
            int mstcost[4];
            int msttree[4];
            long int sum=0;

             for (int i = 0; i < 4; i++)
                mstcost[i] = INT_MAX;
           
            mstcost[0]=0;
            msttree[0]=-1;

                for(int i=0;i<3;i++)
                    {
                        int m=findmin(mstcost,mstarray);

                        mstarray[m]=1;

                        for(int j=0;j<4;j++)
                            {
                               if(mstarray[j] ==0 && adjmat[m][j]>0 && adjmat[m][j]<mstcost[j])
                               {
                                 msttree[j]=m;
                                mstcost[j]=adjmat[m][j];
                                
                                }

                            }
          
                    }
                    for(int k=0;k<4;k++)
                        sum=sum+mstcost[k];
                printf(" \nThe Client has calculated a tree. The tree cost is %ld :",sum);

                printf("\nEdge \t Cost \n");
                for(int l=1;l<4;l++)
                {
                     //printf("  %d \n", (msttree[l]+65),(65+l), adjmat[l][msttree[l]]);     
                        printf("%c%c \t %d \n",(msttree[l]+65),(65+l), mstcost[l]);
                 
                }
               
        }
           
     
     
