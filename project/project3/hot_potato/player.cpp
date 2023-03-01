#include "potato.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 3){
    printErrorMsg("Usage: programName <host_name> <port_num>.\n");
  }
  // connect to server
  int ringmaster_socket_fd = connect_to_server(argv[1], argv[2]);
  
  // receive player id and total number 
  int player_id_info[2]; 
  recv(ringmaster_socket_fd, player_id_info, sizeof(player_id_info), 0);

  int listen_socket_fd = start_listen(""); // random select a port to listen
  std::string ownIpaddress = getIpaddress(listen_socket_fd, "Own");
  unsigned int ownPort = getPort(listen_socket_fd, "Own");

  struct player_info own_info;
  strcpy(own_info.ipAddress, ownIpaddress.c_str());
  own_info.port_num = ownPort;
  try_send_all(ringmaster_socket_fd, &own_info, sizeof(own_info), 0);


  

  struct player_info next_player;
  recv(ringmaster_socket_fd, &next_player, sizeof(next_player), 0);
  cout << "Next player IP: " << next_player.ipAddress << endl;
  cout << "Next player Port: " << next_player.port_num << endl;
  char port_num_input[10];
  sprintf(port_num_input, "%u", next_player.port_num);
  // connect to next player
  int next_player_fd = connect_to_server(next_player.ipAddress, port_num_input);

  char prev_player_port_num[10];
  sprintf(prev_player_port_num, "%u", client_port_num);
  int prev_player_fd = start_listen(prev_player_port_num);


//   int s1, s2, n;
//   fd_set readfds;
//   struct timeval tv;
//   char buf1[256], buf2[256];

//   // pretend we've connected both to a server at this point
//    //s1 = socket(...);
//    //s2 = socket(...);
//   //connect(s1, ...)...
//   //connect(s2, ...)...
//   // clear the set ahead of time
//   FD_ZERO(&readfds);

// // add our descriptors to the set
//   FD_SET(s1, &readfds);
//   FD_SET(s2, &readfds);

//   // since we got s2 second, it's the "greater", so we use that for
//   // the n param in select()
//   n = s2 + 1;

//   // wait until either socket has data ready to be recv()d (timeout 10.5 secs)
//   tv.tv_sec = 10;
//   tv.tv_usec = 500000;
//   rv = select(n, &readfds, NULL, NULL, &tv);
//   if (rv == -1) {
//   perror("select"); // error occurred in select()
//   } else if (rv == 0) {
//   printf("Timeout occurred! No data after 10.5 seconds.\n");
//   } else {
//   // one or both of the descriptors have data
//   if (FD_ISSET(s1, &readfds)) {
//   recv(s1, buf1, sizeof buf1, 0);
//   }
//   if (FD_ISSET(s2, &readfds)) {
//   recv(s2, buf2, sizeof buf2, 0);
//   }
//   }

  close(prev_player_fd);
  close(next_player_fd);
  close(ringmaster_socket_fd);
  return 0;
}
