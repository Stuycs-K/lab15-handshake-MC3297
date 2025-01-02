#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  char message[] = "hello amogus ";
  write(to_server, message, sizeof(message));

  char buffer[BUFFER_SIZE];
  read(from_server, buffer, BUFFER_SIZE);

  printf("client's message: %s\n", buffer);
}
