#include "exerciser.h"
#include "query_funcs.h"


int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  /* Create SQL statement */
  create_all_tables(C, "createTable.sql");
  create_state_table(C, "state.txt");
  create_color_table(C, "color.txt");
  create_team_table(C, "team.txt");
  create_player_table(C, "player.txt");

  exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}


