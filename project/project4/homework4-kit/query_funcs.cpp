#include "query_funcs.h"

void executeSQL(const string& sql, connection *C){
  /* Create a transactional object. */
  work W(*C);
  /* Execute SQL query */
  W.exec(sql);
  W.commit();
  // cout << "sql execute successful" << endl;
}

std::string quoteString(const std::string& str) {
  return "\'" + str + "\'";
}

void create_all_tables(connection *C, string filename){
  string sql, line;
  ifstream tablesql(filename);
  if (!tablesql){
    cout << "can not open file";
  } else {
    while(getline(tablesql, line)){
        sql += line;
    }
  }
  tablesql.close();
  executeSQL(sql, C);
  cout << "Table created successfully" << endl;
}

void create_state_table(connection *C, string filename){
  string line;
  string state_name;
  int state_id; 
  ifstream stateTable(filename);
  stringstream ss;
  if (!stateTable){
    cout << "can not open file";
  } else {
    while(getline(stateTable, line)){
        ss << line;
        ss >> state_id >> state_name;
        add_state(C, state_name);
        ss.clear();
    }
  }
}

void create_color_table(connection *C, string filename){
  string line;
  string color_name;
  int color_id; 
  ifstream colorTable(filename);
  stringstream ss;
  if (!colorTable){
    cout << "can not open file";
  } else {
    while(getline(colorTable, line)){
        ss << line;
        ss >> color_id >> color_name;
        add_color(C, color_name);
        ss.clear();
    }
  }
}

void create_team_table(connection *C, string filename){
  string line;
  string team_name;
  int team_id, state_id, color_id, wins, losses; 
  ifstream teamTable(filename);
  stringstream ss;
  if (!teamTable){
    cout << "can not open file";
  } else {
    while(getline(teamTable, line)){
        ss << line;
        ss >> team_id >> team_name >> state_id >> color_id >> wins >> losses;
        add_team(C, team_name, state_id, color_id, wins, losses);
        ss.clear();
    }
  }
}

void create_player_table(connection *C, string filename){
  string line;
  string first_name, last_name;
  int player_id, team_id, uniform_num, mpg, ppg, rpg, apg;
  double spg, bpg; 
  ifstream teamTable(filename);
  stringstream ss;
  if (!teamTable){
    cout << "can not open file";
  } else {
    while(getline(teamTable, line)){
        ss << line;
        ss >> player_id >> team_id >> uniform_num >> first_name >> last_name >> mpg >> ppg >> rpg >> apg >> spg >> bpg;
        add_player(C, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
        ss.clear();
    }
  }
}

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
  string sql;
  stringstream ss;
  sql += string("INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) ");
  sql += string("VALUES (");
  sql += to_string(team_id) + ",";
  sql += to_string(jersey_num) + ",";
  sql += C->quote(first_name) + ",";
  sql += C->quote(last_name) + ",";
  sql += to_string(mpg) + ",";
  sql += to_string(ppg) + ",";
  sql += to_string(rpg) + ",";
  sql += to_string(apg) + ",";
  sql += to_string(spg) + ",";
  sql += to_string(bpg);
  sql += string("); ");
  executeSQL(sql, C);
  cout << "add values: " << first_name  << " successful" << endl;

}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
  string sql;
  stringstream ss;
  sql += string("INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) ");
  sql += string("VALUES (");
  sql += C->quote(name) + ",";
  sql += to_string(state_id) + ",";
  sql += to_string(color_id) + ",";
  sql += to_string(wins) + ",";
  sql += to_string(losses);
  sql += string("); ");
  executeSQL(sql, C);
  cout << "add values: " << name  << " successful" << endl;
}


void add_state(connection *C, string name)
{
  string sql;
  stringstream ss;
  sql += string("INSERT INTO STATE (NAME) ");
  sql += string("VALUES (");
  sql += C->quote(name);
  sql += string("); ");
  executeSQL(sql, C);
  cout << "add values: " << name  << " successful" << endl;
}


void add_color(connection *C, string name)
{
  string sql;
  stringstream ss;
  sql += string("INSERT INTO COLOR (NAME) ");
  sql += string("VALUES (");
  sql += C->quote(name);
  sql += string("); ");
  executeSQL(sql, C);
  cout << "add values: " << name  << " successful" << endl;
}

/*
 * All use_ params are used as flags for corresponding attributes
 * a 1 for a use_ param means this attribute is enabled (i.e. a WHERE clause is needed)
 * a 0 for a use_ param means this attribute is disabled
 */
void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    /* Create SQL statement */
    string sql = "SELECT * from COMPANY";

    /* Create a non-transactional object. */
    nontransaction N(C);
      
    /* Execute SQL query */
    result R( N.exec( sql ));
      
    /* List down all the records */
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << "ID = " << c[0].as<int>() << endl;
      cout << "Name = " << c[1].as<string>() << endl;
      cout << "Age = " << c[2].as<int>() << endl;
      cout << "Address = " << c[3].as<string>() << endl;
      cout << "Salary = " << c[4].as<float>() << endl;
    }
    cout << "Operation done successfully" << endl;
}


void query2(connection *C, string team_color)
{
}


void query3(connection *C, string team_name)
{
}


void query4(connection *C, string team_state, string team_color)
{
}


void query5(connection *C, int num_wins)
{
}
