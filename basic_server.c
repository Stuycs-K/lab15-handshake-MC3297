#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;
  
  from_client = server_handshake( &to_client );
  
  char message[BUFFER_SIZE];
  read(from_client, message, sizeof(message));
  
  char cat[] = "3297";
  strcat(message, cat);
  printf("message from server to client: %s\n", message);
  write(to_client, message, sizeof(message));
}
