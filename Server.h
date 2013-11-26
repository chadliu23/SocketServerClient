#ifndef SERVER_H
#define SERVER_H
#include "ISocketAdapter.h"
#include "SocketAdapter.h"
class Server
{
public:
	Server(int port, ISocketAdapter* socketAdapter = new SocketAdapter());
	~Server();
	void start();
private:
	ISocketAdapter* m_socket;
};

#endif