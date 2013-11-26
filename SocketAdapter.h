#ifndef SOCKETADPTER_H
#define SOCKETADPTER_H

#include "ISocketAdapter.h"
#include <string>

class SocketAdapter: public ISocketAdapter
{
public:
	SocketAdapter();
	virtual int socketBind(int port);
	virtual int socketListen(int backlog_queue_size);
	virtual int socketAccept();
	virtual int socketConnect(const std::string& ip, int port);
	virtual int sendToClient(const std::string& message);
	virtual int socketWrite(const std::string& message);
    virtual void closeClient();
	virtual std::string readFromServer();
	virtual std::string readFromClient();
	virtual std::string clientIP();
	virtual int clientPort();
	~SocketAdapter();
private:

	std::string m_clientIP;
	int m_clientPort;
};


#endif