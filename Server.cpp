/* The port number is passed as an argument */
#include <iostream>
#include "SocketAdapter.h"

#include <unistd.h>

using namespace std;
int main(int argc, char *argv[])
{

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
    SocketAdapter socketAdapter;
    if(!socketAdapter.isSocketOpen())
    {
        cerr <<"ERROR opening socket" <<endl;
        return -1;
    }
    
    if (socketAdapter.socketBind(atoi(argv[1])) < 0)
    {
        cerr << "ERROR on binding" <<endl;
        return -1;
    }
    
     // This listen() call tells the socket to listen to the incoming connections.
     // The listen() function places all incoming connection into a backlog queue
     // until accept() call accepts the connection.
     // Here, we set the maximum size for the backlog queue to 5.
     socketAdapter.socketListen(5);

     // The accept() call actually accepts an incoming connection

     // This accept() function will write the connecting client's address info 
     // into the the address structure and the size of that structure is clilen.
     // The accept() returns a new socket file descriptor for the accepted connection.
     // So, the original socket file descriptor can continue to be used 
     // for accepting new connections while the new socker file descriptor is used for
     // communicating with the connected client.
    
    
    while(true)
    {
        if(socketAdapter.socketAccept() < 0)
        {
            cerr <<"ERROR on accept" <<endl;
             return -1;
        }
        cout <<"server: got connection from "
             <<socketAdapter.clientIP() <<" port " <<socketAdapter.clientPort() <<endl;
        int emptyMessageCount = 0;
        while (emptyMessageCount < 10) {

            // This send() function sends the 13 bytes of the string to the new socket
            //socketAdapter.sendToClient("Hello, world!\n");
            string message = socketAdapter.readFromClient();
    
            if(message == "hello:world")
            {
                cout <<socketAdapter.clientIP() <<":" <<socketAdapter.clientPort() 
                     <<"<auth>: " <<message <<endl;
                socketAdapter.sendToClient("ACCEPT");
           }
           else if(message != "")
           {
               cout <<socketAdapter.clientIP() <<":" <<socketAdapter.clientPort() <<" :" <<message;
               socketAdapter.sendToClient("ACK");
           }
           else
          {
              ++emptyMessageCount;
           }
        }
        socketAdapter.closeClient();
    }
     return 0;
}

