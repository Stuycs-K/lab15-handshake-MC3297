#include "pipe_networking.h"
#include <signal.h>

void si(int sig) {
	remove(WKP);
	exit(0);
}

int main() {
	signal(SIGINT, si);
	while (1) {
		int to_client;
		int from_client;
		from_client = server_handshake(&to_client);

		char buffer[BUFFER_SIZE];
		read(from_client, buffer, sizeof(buffer));
		char cat[] = " sample code";
		strcat(buffer, cat);
		printf("server received message: %s\n", buffer);
		write(to_client, buffer, sizeof(buffer));

		close(to_client);
		close(from_client);
	}
}
