#include <stdio.h>
#include "server.h"

int server()
{
	return 33;
}

int main(int argc, char* argv[])
{
	printf("Server: Hello world %i\n", server());
	return 0;
}
