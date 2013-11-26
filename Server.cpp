#include "Server.h"
#include <iostream>
using namespace std;

Server::Server(int port, ISocketAdapter* socketAdapter)
{
	m_socket = socketAdapter;
	if(m_socket == NULL)
	{
		cerr <<"ERROR NULL socket" <<endl;
		return;
	}
	if(!m_socket->isSocketOpen())
    {
        cerr <<"ERROR opening socket" <<endl;
        return ;
    }
    
    if (m_socket->socketBind(port) < 0)
    {
        cerr << "ERROR on binding" <<endl;
        return ;
    }
    
     
    m_socket->socketListen(5);
}

Server::~Server()
{
	if(m_socket != NULL)
	{
		delete m_socket;
		m_socket = NULL;
	}

}

void Server::start()
{
	if(m_socket == NULL)
	{
		return;
	}
	while(true)
    {
        if(m_socket->socketAccept() < 0)
        {
            cerr <<"ERROR on accept" <<endl;
             return ;
        }
        cout <<"server: got connection from "
             <<m_socket->clientIP() <<" port " <<m_socket->clientPort() <<endl;
        int emptyMessageCount = 0;
        while (emptyMessageCount < 10) {

            // This send() function sends the 13 bytes of the string to the new socket
            //socketAdapter.sendToClient("Hello, world!\n");
            string message = m_socket->readFromClient();
            if(message.find("auth:")!= string::npos)
            {
                if(message == "auth:hello:world")
                {
                    cout <<m_socket->clientIP() <<":" <<m_socket->clientPort()
                        <<": <auth> : " <<message <<endl;
                    m_socket->sendToClient("ACCEPT");
                }else
                {
                    m_socket->sendToClient("REJECT");
                    break;
                }
            }
            
            else if(message != "")
            {
               cout <<m_socket->clientIP() <<":" <<m_socket->clientPort() 
                    <<": <mesg> : " <<message;
               m_socket->sendToClient("ACK");
            }
            else
            {
              ++emptyMessageCount;
            }
        }
        m_socket->closeClient();
    }

}