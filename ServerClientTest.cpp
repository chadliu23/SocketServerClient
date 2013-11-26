#include <limits.h>
#include "Server.h"
#include "Client.h"
#include "ISocketAdapter.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <vector> 
#include <string>

using namespace testing;
using namespace std;
class MockSocketAdapter: public ISocketAdapter
{
public:
	MOCK_METHOD1(socketBind, int(int port));
	MOCK_METHOD1(socketListen, int(int backlog_queue_size));
	MOCK_METHOD0(socketAccept , int());
	MOCK_METHOD2(socketConnect, int(const std::string& ip, int port));
	MOCK_METHOD1(sendToClient, int(const std::string& message));
	MOCK_METHOD1(socketWrite, int(const std::string& message));
	MOCK_METHOD0(readFromServer, std::string());
	MOCK_METHOD0(readFromClient, std::string());
	MOCK_METHOD0(clientIP, std::string());
    MOCK_METHOD0(closeClient, void());
	MOCK_METHOD0(clientPort, int());
};

TEST(ServerTest, Server_Initialize)
{
	StrictMock<MockSocketAdapter>* adapter = new StrictMock<MockSocketAdapter>();
	int port = 5566;
	EXPECT_CALL(*adapter, socketBind(port))
		.Times(1)
		.WillOnce(Return(1));

	EXPECT_CALL(*adapter, socketListen(5))
		.Times(1)
		.WillOnce(Return(1));

	Server s(port,adapter);
}
TEST(ServerTest, Server_Auth_Success)
{
	StrictMock<MockSocketAdapter>* adapter = new StrictMock<MockSocketAdapter>();
	int port = 5566;
	EXPECT_CALL(*adapter, clientPort())
		.WillRepeatedly(Return(7788));
	EXPECT_CALL(*adapter, clientIP())
		.WillRepeatedly(Return(""));	

	EXPECT_CALL(*adapter, socketBind(port))
		.Times(1)
		.WillOnce(Return(1));

	EXPECT_CALL(*adapter, socketListen(5))
		.Times(1)
		.WillOnce(Return(1));

	EXPECT_CALL(*adapter, socketAccept())
		.Times(AtLeast(1))
		.WillOnce(Return(1))
		.WillRepeatedly(Return(-1));

	EXPECT_CALL(*adapter, readFromClient())
		.Times(AtLeast(1))
		.WillOnce(Return("auth:hello:world"))
		.WillRepeatedly(Return(""));

	EXPECT_CALL(*adapter, sendToClient("ACCEPT"))
		.Times(1)
		.WillOnce(Return(1));

	EXPECT_CALL(*adapter, closeClient())
	   	.WillRepeatedly(Return());

	Server s(port,adapter);
	s.start();
}

TEST(ServerTest, Server_Auth_Fail)
{
	StrictMock<MockSocketAdapter>* adapter = new StrictMock<MockSocketAdapter>();
	int port = 5566;
	EXPECT_CALL(*adapter, clientPort())
		.WillRepeatedly(Return(7788));
	EXPECT_CALL(*adapter, clientIP())
		.WillRepeatedly(Return(""));	

	EXPECT_CALL(*adapter, socketBind(port))
		.Times(1)
		.WillOnce(Return(1));

	EXPECT_CALL(*adapter, socketListen(5))
		.Times(1)
		.WillOnce(Return(1));

	EXPECT_CALL(*adapter, socketAccept())
		.Times(AtLeast(1))
		.WillOnce(Return(1))
		.WillRepeatedly(Return(-1));

	EXPECT_CALL(*adapter, readFromClient())
		.Times(AtLeast(1))
		.WillOnce(Return("auth:"))
		.WillRepeatedly(Return(""));

	EXPECT_CALL(*adapter, sendToClient("REJECT"))
		.Times(1)
		.WillOnce(Return(1));

	EXPECT_CALL(*adapter, closeClient())
	   	.WillRepeatedly(Return());

	Server s(port,adapter);
	s.start();
}

TEST(ClientTest, Client_Init)
{
	StrictMock<MockSocketAdapter>* adapter = new StrictMock<MockSocketAdapter>();
	int port = 5566;
	string ip("localhost");
	EXPECT_CALL(*adapter, socketConnect(ip, port))
	   	.WillRepeatedly(Return(true));
	
	Client client(ip,port,adapter);
}

TEST(ClientTest, Client_Auth_Reject)
{
	StrictMock<MockSocketAdapter>* adapter = new StrictMock<MockSocketAdapter>();
	int port = 5566;
	string ip("localhost");
	EXPECT_CALL(*adapter, socketConnect(ip, port))
	   	.WillRepeatedly(Return(true));
	EXPECT_CALL(*adapter, socketWrite("auth:hello:world"))
		.Times(1)
		.WillOnce(Return(1));
	EXPECT_CALL(*adapter, readFromServer())
		.Times(1)
		.WillOnce(Return("REJECT"));

	Client client(ip,port,adapter);
	client.auth("hello", "world");
	client.doJob();
}



class MockTimeSocketAdapter: public ISocketAdapter
{
public:
	MockTimeSocketAdapter(): readFromServerCount(0){}
	int socketBind(int ){ return 1;}
	int socketListen(int ){ return 1;}
	int socketAccept (){return 1;}
	int socketConnect(const std::string& , int ){return 1;}
	int sendToClient(const std::string& ){return 1;}
	int socketWrite(const std::string& message)
	{
		m_sendToServer.push_back(message);
		return 1;
	}
	string readFromServer()
	{

		++readFromServerCount;
		if(readFromServerCount == 1)
		{
			return "ACCEPT";
		}
		if(readFromServerCount == 2)
		{
			return "ACK";
		}
		return "";
	}
	string readFromClient(){return "";}
	string clientIP(){return "";}
    void closeClient(){}
	int clientPort(){return 0;}
	std::vector<string> getSend2Server(){return m_sendToServer;}
private:
	vector<string> m_sendToServer;
	int readFromServerCount;
};

TEST(ClientTest, ClientSendTime)
{
	MockTimeSocketAdapter* adapter = new MockTimeSocketAdapter();
	int port = 5566;
	string ip("localhost");
	Client c(ip,port,adapter);

	c.auth("hello", "world");
	c.doJob();
	std::vector<string> v = adapter->getSend2Server();
	EXPECT_EQ( 7, v.size()); //1 auth + 1 ACK + 5 retry
	std::vector<int> seconds;
	for (unsigned i = 1; i < v.size(); ++i)
	{
		struct tm tm;
		if (strptime(v[i].c_str(), "%c\n", &tm) == NULL)
		{
			cout <<"error on \"" <<v[i]<<"\""<<endl;
		}
		seconds.push_back(tm.tm_sec);
	}
	EXPECT_EQ(6, seconds.size());
	EXPECT_EQ(15, seconds[1] - seconds[0]);
	EXPECT_EQ(2, seconds[2] - seconds[1]);
	
}
