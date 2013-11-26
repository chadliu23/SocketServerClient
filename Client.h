#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "ISocketAdapter.h"
#include "SocketAdapter.h"

class Client
{
public:
	Client(const std::string& ip, int port, ISocketAdapter* psocket = new SocketAdapter());
	~Client();
	bool sendTime();
	bool doJob();
	bool auth(const std::string& name, const std::string& password);
private:
	ISocketAdapter* m_socket;
	bool m_isAuth;
};

#endif