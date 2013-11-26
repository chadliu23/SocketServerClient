#include "Client.h"
#include "SocketAdapter.h"
#include <time.h>  
#include <sstream>
#include <iostream>
#include <unistd.h>
using namespace std;

Client::Client(const std::string& ip, int port, ISocketAdapter* psocket)
	: m_socket(NULL) 
	, m_isAuth(false)
{
	m_socket = psocket;
	if(m_socket == NULL)
	{
		return;
	}
	if(m_socket->socketConnect(ip, port) < 0)
	{
		delete m_socket;
		m_socket = NULL;
	}
}
Client::~Client()
{
	if(m_socket != NULL)
	{
		cout <<"delete m_socket" <<endl;
		delete m_socket;
	}
	cout <<"end of delete m_socket" <<endl;
	m_socket = NULL;
}

bool Client::auth(const std::string& name, const std::string& password)
{
	if(m_socket == NULL)
	{
		return false;
	}

	string toServer;
	toServer += name;
	toServer += ":";
	toServer += password;
	m_socket->socketWrite(toServer);
	cout <<"<Client> auth: " <<toServer <<endl;
	string message = m_socket->readFromServer();
	if(message == "ACCEPT")
	{
		m_isAuth = true;
		return true;
	}
	return false;
}

bool Client::sendTime()
{
	time_t rawtime;
  	struct tm * timeinfo;

  	time (&rawtime);
  	timeinfo = localtime (&rawtime);
	stringstream ss;
	ss << asctime(timeinfo);
	cout <<"<Clent> Send: " <<ss.str() ;
	m_socket->socketWrite(ss.str());
	string message = m_socket->readFromServer();
	if(message == "ACK")
	{
		return true;
	}
	return false;
}
bool Client::doJob()
{
	if(m_socket == NULL || !m_isAuth)
	{
		return false;
	}
	while(true)
	{
		int tryCount = 0;
		while(tryCount < 5)
		{
			if(sendTime())
			{
				break;
			}
			else
			{
				++tryCount;
			}
		}
		if(tryCount == 5)
		{
			return false;
		}
		sleep(15);

	}
	return true;
}