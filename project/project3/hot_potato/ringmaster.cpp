#include "potato.h"


int main(int argc, char *argv[])
{
  if (argc != 4){
    printErrorMsg("Usage: programName <port_num> <num_players> <num_hops>.\n");
  }

  int status = 0;
  // int port_num = convertNum(argv[1]);
  int num_players = convertNum(argv[2]);
  int num_hops = convertNum(argv[3]);
  int socket_fd = initServer(argv[1]);
  char ipAddress[INET6_ADDRSTRLEN];
  vector<int> client_connection_fd_list;
  // vector<Player> client_connection_player_list;
  vector<player_info> client_connection_info_list;
  vector<unsigned int> client_connection_port_list;
  for (int i = 0; i < num_players; i ++ ){
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_connection_fd == -1) {
      cerr << "Error: cannot accept connection on socket" << endl;
      return -1;
    } //if
    client_connection_fd_list.push_back(client_connection_fd);
    int player_id_info[2] = {i, num_players}; // pointer decay
    size_t len = sizeof(player_id_info);
    // can not send data at a time
    
    send(client_connection_fd, player_id_info, len, 0); 
    // get an ack?
    // printf("player %d is ready", i);

    // status = getsockname(client_connection_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    inet_ntop(socket_addr.ss_family,
        get_in_addr((struct sockaddr *)&socket_addr),
        ipAddress, sizeof ipAddress);
    unsigned int client_port_num = ntohs(get_in_port((struct sockaddr *)&socket_addr));
    struct player_info now_player;
    strcpy(now_player.ipAddress, ipAddress);
    now_player.port_num = client_port_num;
    client_connection_info_list.push_back(now_player);
    // client_connection_ip_list.push_back();
    // client_connection_port_list.push_back(client_port_num);
    // client_connection_player_list.push_back(Player(ipAddress, client_port_num));
    // cout << "Client IP: " << ipAddress << endl;
    // cout << "Client IP through Class: " << client_connection_player_list[i].get_IP_addr() << endl;
    // cout << "Client Port: " << get_in_port((struct sockaddr *)&socket_addr) << endl;
    // cout << "Client Port ntohs: " << ntohs(get_in_port((struct sockaddr *)&socket_addr)) << endl;
    // cout << "Client Port ntohs through Class: " << client_connection_player_list[i].get_port_num() << endl;
  }
  // send the final player's info to the first player
  // send(client_connection_fd_list[0], client_connection_player_list[num_players - 1], sizeof client_connection_player_list[num_players - 1], 0);
  
  for (int i = 0; i < num_players; i ++ ){
      // send prev player's info to the current player
      // TODO: can I send a class through socket
      // send(client_connection_fd, client_connection_player_list[i - 1], sizeof(*client_connection_player_list[i - 1]), 0);
        struct player_info next_player = client_connection_info_list[(i + 1) % num_players];
        // cout << next_player.ipAddress << endl;
        // cout << strlen(next_player.ipAddress) << endl;
        // cout << next_player.port_num << endl;
        try_send_all(client_connection_fd_list[i], &next_player, sizeof(next_player), 0);
        // try_send_all(client_connection_fd_list[i], prev_Player_ip, strlen(prev_Player_ip) + 1, 0);
        // send(client_connection_fd_list[i], &prev_Player_port_num, sizeof(prev_Player_port_num), 0);
    }
  close(socket_fd);

  return 0;
}
