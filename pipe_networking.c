#include "pipe_networking.h"

int rand_int() {
  int f = open("/dev/random", O_RDONLY);
  if (f == -1) perror("open dev random");
  int randint;
  read(f, &randint, sizeof(randint));
  close(f);
  return randint;
}

//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int wkp = mkfifo(WKP, 0666);
  if (wkp == -1) perror("mkfifo wkp");
  
  int from_client = open(WKP, O_RDONLY);
  if (from_client == -1) perror("open wkp");
  
  remove(WKP);
  return from_client;
}


/*=========================
  server_handshake 
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client = server_setup();
  
  //receive PP from SYN
  char buffer[HANDSHAKE_BUFFER_SIZE];
  //buffer contains pid of client as private pipe
  if (read(from_client, buffer, sizeof(buffer)) == -1) perror("reading from client");
  
  //send rand_int for SYN_ACK
  *to_client = open(buffer, O_WRONLY);
  if (*to_client == -1) perror("opening PP");
  int randint = rand_int();
  sprintf(buffer, "%d", randint);
  write(*to_client, buffer, sizeof(buffer));
  
  //receive ACK which is randint+1
  int ack;
  read(from_client, &ack, sizeof(ack));
  if (ack != randint+1) perror("ack mismatch with syn_ack");
  
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  //use pid as pp, create pp pipe
  char buffer[HANDSHAKE_BUFFER_SIZE];
  sprintf(buffer, "%d", getpid());
  mkfifo(buffer, 0666);
  
  //open WKP to write SYN (pp as pid)
  *to_server = open(WKP, O_WRONLY);
  write(*to_server, buffer, sizeof(buffer));
  
  int from_server = open(buffer, O_RDONLY);
  read(from_server, buffer, sizeof(buffer));
  remove(buffer);
  
  int syn_ack;
  sscanf(buffer, "%d", &syn_ack);
  sprintf(buffer, "%d", syn_ack+1);
  write(*to_server, buffer, sizeof(buffer));
  
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}


