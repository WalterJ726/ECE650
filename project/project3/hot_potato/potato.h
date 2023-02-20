#include <cstdio>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <cstring>
#include <string>
#include <stdlib.h>


using namespace std;



typedef struct potato_t{
    int hops;
}potato;

struct player_info{
  char ipAddress[INET6_ADDRSTRLEN];
  unsigned int port_num;
};


// class Player{
//     private:
//         char IP_addr[];
//         unsigned int port_num;
//     public:
//         // Player(){
//         //     IP_addr = "0.0.0.0";
//         //     port_num = 12345;
//         // }
//         Player(char* IP_addr, unsigned int & port_num) : IP_addr(IP_addr), port_num(port_num) {}
        
//         char* get_IP_addr(){
//             return IP_addr;
//         }
        
//         unsigned int get_port_num(){
//             return port_num;
//         }
// };

void printErrorMsg(string msg){
  cerr << msg << endl;
  exit(EXIT_FAILURE);
}

long int convertNum(char* line){
  char * ptr;
  long int result = strtol(line, &ptr, 10);
  if (*ptr != '\0') {
    printErrorMsg("encounter some character before number in words");
  }
  // there is no digits at all
  if (result == 0 && *(ptr - 1) != 0) {
    printErrorMsg("there is no digits at all");
  }
  // result is not a valid number
  if (result < 0) {
    printErrorMsg("result is not a valid number");
  }
  return result;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int try_send_all(int sock_fd, void* data, size_t data_len, int flag){
    size_t total = 0;
    size_t byteleft = data_len;
    int n;
    while (total < data_len){
      n = send(sock_fd, data + total, byteleft, flag); 
      if (n == -1) {break;}
      total += n;
      byteleft -= n;
    }

    return n == -1 ? -1 : 0;
}


unsigned short get_in_port(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return (((struct sockaddr_in*)sa)->sin_port);
    }
    return (((struct sockaddr_in6*)sa)->sin6_port);
}




int initServer(char* port_num){
  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo *host_info_list;
  const char *hostname = NULL;
  const char *port     = port_num;

  memset(&host_info, 0, sizeof(host_info));

  host_info.ai_family   = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags    = AI_PASSIVE;

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

  int yes = 1;
  status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: cannot bind socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  status = listen(socket_fd, 100);
  if (status == -1) {
    cerr << "Error: cannot listen on socket" << endl; 
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if
  freeaddrinfo(host_info_list);
  cout << "Waiting for connection on port " << port << endl;
  return socket_fd;
}




