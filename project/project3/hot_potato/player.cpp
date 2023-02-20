#include "potato.h"

using namespace std;

int main(int argc, char *argv[])
{
  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo *host_info_list;
  const char *hostname = argv[1];
  const char *port     = "4444";


  if (argc < 2) {
      cout << "Syntax: client <hostname>\n" << endl;
      return 1;
  }

  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family   = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0) {
    cerr << "Error: cannot get address info for host" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  socket_fd = socket(host_info_list->ai_family, 
		     host_info_list->ai_socktype, 
		     host_info_list->ai_protocol);
  if (socket_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if
  
  cout << "Connecting to " << hostname << " on port " << port << "..." << endl;
  
  status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: cannot connect to socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  int player_id_info[2];
  recv(socket_fd, player_id_info, sizeof(player_id_info), 0);
  struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);

  struct sockaddr_in clientAddr;//客户端地址
  unsigned int clientAddrLen = sizeof(clientAddr);
  char ipAddress[INET6_ADDRSTRLEN];
  getsockname(socket_fd, (struct sockaddr*)&socket_addr, &socket_addr_len);//获取sockfd表示的连接上的本地地址
  inet_ntop(socket_addr.ss_family,
        get_in_addr((struct sockaddr *)&socket_addr),
        ipAddress, sizeof ipAddress);
  cout << "My id is " << player_id_info[0] << " Total player number is " << player_id_info[1] << endl;
  // cout << "Client IP: " << s << endl;
  // cout << "Client Port: " << get_in_port((struct sockaddr *)&socket_addr) << endl;
  // cout << "Client Port ntohs: " << ntohs(get_in_port((struct sockaddr *)&socket_addr)) << endl;


  // recv prev player's info
  // Player* prevPlayer = new Player(); // player a and player a();
  int MAX_DATASIZE = 100;
  char prev_Player_ip[MAX_DATASIZE];
  unsigned int prevPlayer_port_num = 0;
  struct player_info next_player;
  recv(socket_fd, &next_player, sizeof(next_player), 0);
  // recv(socket_fd, &prevPlayer, sizeof(prevPlayer), 0);
  // recv(socket_fd, &prevPlayer_port_num, sizeof(prevPlayer_port_num), 0);
  // int numbytes = recv(socket_fd, prev_Player_ip, MAX_DATASIZE - 1, 0);
  // prev_Player_ip[numbytes] = '\0';
  // cout << strlen(prev_Player_ip) << endl;
  cout << "Next player IP: " << next_player.ipAddress << endl;
  cout << "Next player Port: " << next_player.port_num << endl;
  freeaddrinfo(host_info_list);
  close(socket_fd);
  return 0;
}
