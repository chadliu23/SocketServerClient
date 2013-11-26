#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include "SocketAdapter.h"

using namespace std;

SocketAdapter::SocketAdapter()
: m_clientIP()
, m_clientPort(0)
{
	// create a socket
    // socket(int domain, int type, int protocol)
    
	m_fd = socket(AF_INET, SOCK_STREAM, 0);;
    struct timeval tv;
    
    tv.tv_sec = 2;       /* Timeout in seconds */
    
    setsockopt(m_fd, SOL_SOCKET, SO_SNDTIMEO,(struct timeval *)&tv,sizeof(struct timeval));
    setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&tv,sizeof(struct timeval));
	m_client_fd = -1;
}

SocketAdapter::~SocketAdapter()
{
	if(m_fd >= 0)
	{
		close(m_fd);
        m_fd = -1;
	}
	if(m_client_fd >=0 )
	{
		close(m_client_fd);
         m_client_fd = -1;
	}
}

void SocketAdapter::closeClient()
{
    if(m_client_fd >=0 )
	{
        cout <<"closeClient " <<m_clientIP <<":" <<m_clientPort <<endl;
		close(m_client_fd);
        m_client_fd = -1;
	}
}

int SocketAdapter::socketBind(int port)
{
	struct sockaddr_in serv_addr;     
	// clear address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
	
	/* setup the host_addr structure for use in bind call */
    // server byte order
    serv_addr.sin_family = AF_INET;  

    // automatically be filled with current host's IP address
    serv_addr.sin_addr.s_addr = INADDR_ANY;  

    // convert short integer value for port must be converted into network byte order
    serv_addr.sin_port = htons(port);
	return bind(m_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
}

int SocketAdapter::socketListen(int backlog_queue_size)
{
	return listen(m_fd,backlog_queue_size);
}
int SocketAdapter::socketAccept()
{
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);
	m_client_fd = accept(m_fd, (struct sockaddr *) &cli_addr, &clilen);
	m_clientIP.clear();
	m_clientIP = inet_ntoa(cli_addr.sin_addr);
	m_clientPort = ntohs(cli_addr.sin_port);
	return m_client_fd;
}
int SocketAdapter::socketConnect(const std::string& ip, int port)
{
	struct hostent *server;
	server = gethostbyname(ip.c_str());


	if (server == NULL) {
        return -1;
    }

	struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);
	return connect(m_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
}
int SocketAdapter::sendToClient(const std::string& message)
{
	if(m_client_fd< 0)
	{
		return -1;
	}
	return send(m_client_fd, message.c_str(), strlen(message.c_str()), 0);
}

int SocketAdapter::socketWrite(const std::string& message)
{
	return write(m_fd, message.c_str(), strlen(message.c_str()));
}
string SocketAdapter::readFromServer()
{
	char buffer[256];
	bzero(buffer,256);
	read(m_fd, buffer, 256);
	return string(buffer);
}

string SocketAdapter::readFromClient()
{
	char buffer[256];
	bzero(buffer,256);
	read(m_client_fd, buffer, 256);
	return string(buffer);
}

std::string SocketAdapter::clientIP()
{
	return 	m_clientIP;
}
int SocketAdapter::clientPort()
{
	return m_clientPort;
}
