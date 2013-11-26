#include "SocketAdapter.h"
#include "Client.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    Client client(argv[1], atoi(argv[2]));
    if(!client.auth("hello", "world"))
    {
        cout << "auth failed" <<endl;
        return -1;
    }
    client.doJob();
    return 0;
}

