#ifndef ISOCKETADPTER_H
#define ISOCKETADPTER_H
#include <string>

class ISocketAdapter
{
public:
	virtual int socketBind(int port) = 0;
	virtual int socketListen(int backlog_queue_size) = 0;
	virtual int socketAccept() = 0;
	virtual int socketConnect(const std::string& ip, int port) = 0;
	virtual int sendToClient(const std::string& message) = 0;
	virtual int socketWrite(const std::string& message) = 0;
	virtual std::string readFromServer() = 0;
	virtual std::string readFromClient() = 0;
	virtual std::string clientIP() = 0;
    virtual void closeClient() = 0;
	virtual int clientPort() = 0;
    virtual ~ISocketAdapter(){}
	bool isSocketOpen() { return m_fd >= 0; }
	
protected:
	int m_fd;
	int m_client_fd;
};

#endif