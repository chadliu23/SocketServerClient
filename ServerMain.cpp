/* The port number is passed as an argument */
#include <iostream>
#include "Server.h"


using namespace std;
int main(int argc, char *argv[])
{

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    Server server(atoi(argv[1]));
    server.start();
    
     return 0;
}

