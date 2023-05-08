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
  // cout << "Table created successfully" << endl;
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
  // cout << "add values: " << first_name  << " successful" << endl;

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
  // cout << "add values: " << name  << " successful" << endl;
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
  // cout << "add values: " << name  << " successful" << endl;
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
  // cout << "add values: " << name  << " successful" << endl;
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
    string sql = "SELECT * from PLAYER";
    if (use_mpg){
      sql += " WHERE ";
      sql += "MPG>=" + to_string(min_mpg);
      sql += " AND " + to_string(max_mpg) + ">=MPG";
    }
    if (use_ppg){
      if (use_mpg){
        sql += " AND ";
      } else {
        sql += " WHERE ";
      }
      sql += "PPG>=" + to_string(min_ppg);
      sql += " AND " + to_string(max_ppg) + ">=PPG";
    }
    if (use_rpg){
      if (use_mpg || use_ppg){
        sql += " AND ";
      } else {
        sql += " WHERE ";
      }
      sql += "RPG>=" + to_string(min_rpg);
      sql += " AND " + to_string(max_rpg) + ">=RPG";
    }
    if (use_apg){
      if (use_mpg || use_ppg || use_rpg){
        sql += " AND ";
      } else {
        sql += " WHERE ";
      }
      sql += "APG>=" + to_string(min_apg);
      sql += " AND " + to_string(max_apg) + ">=APG";
    } 
    if (use_spg){
      if (use_mpg || use_ppg || use_rpg || use_apg){
        sql += " AND ";
      } else {
        sql += " WHERE ";
      }
      sql += "SPG>=" + to_string(min_spg);
      sql += " AND " + to_string(max_spg) + ">=SPG";
    }
    if (use_bpg){
      if (use_mpg || use_ppg || use_rpg || use_apg || use_spg){
        sql += " AND ";
      } else {
        sql += " WHERE ";
      }
      sql += "BPG>=" + to_string(min_bpg);
      sql += " AND " + to_string(max_bpg) + ">=BPG";
    }
    sql += ";";
    // cout << sql << endl;
    /* Create a non-transactional object. */
    nontransaction N(*C);
    /* Execute SQL query */
    result R( N.exec( sql ));
    cout << "PLAYER_ID " << "TEAM_ID " << "UNIFORM_NUM " << "FIRST_NAME ";
    cout << "LAST_NAME " << "MPG " << "PPG " << "RPG " << "APG "; 
    cout << "SPG " << "BPG " << endl;
    /* List down all the records */
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << c[0].as<int>() << " ";
      cout << c[1].as<int>() << " ";
      cout << c[2].as<int>() << " ";
      cout << c[3].as<string>() << " ";
      cout << c[4].as<string>() << " ";
      cout << c[5].as<int>() << " ";
      cout << c[6].as<int>() << " ";
      cout << c[7].as<int>() << " ";
      cout << c[8].as<int>() << " ";
      cout << fixed << setprecision(1);
      cout << c[9].as<double>() << " ";
      cout << c[10].as<double>() << endl;
    }
}


void query2(connection *C, string team_color)
{
      /* Create SQL statement */
    string sql = "SELECT TEAM.NAME from TEAM, COLOR ";
    sql += "WHERE COLOR.COLOR_ID=TEAM.COLOR_ID AND ";
    sql += "COLOR.NAME=" + C->quote(team_color) + ";";
    // cout << sql << endl;
    /* Create a non-transactional object. */
    nontransaction N(*C);
      
    /* Execute SQL query */
    result R( N.exec( sql ));
    cout << "NAME " << endl;
    /* List down all the records */
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << c[0].as<string>() << endl;
    }
}


void query3(connection *C, string team_name)
{
    /* Create SQL statement */
    string sql = "SELECT FIRST_NAME, LAST_NAME from TEAM, PLAYER ";
    sql += "WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND ";
    sql += "TEAM.NAME=" + C->quote(team_name) + " ";
    sql += "ORDER BY PPG DESC;";
    // cout << sql << endl;
    /* Create a non-transactional object. */
    nontransaction N(*C);
      
    /* Execute SQL query */
    result R( N.exec( sql ));
    cout << "FIRST_NAME " << "LAST_NAME" << endl;
    /* List down all the records */
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << c[0].as<string>() << " ";
      cout << c[1].as<string>() << endl;
    }
}


void query4(connection *C, string team_state, string team_color)
{
      /* Create SQL statement */
    string sql = "SELECT UNIFORM_NUM,FIRST_NAME,LAST_NAME from STATE,COLOR,PLAYER,TEAM ";
    sql += "WHERE TEAM.TEAM_ID=PLAYER.TEAM_ID AND TEAM.STATE_ID=STATE.STATE_ID AND ";
    sql += "TEAM.COLOR_ID=COLOR.COLOR_ID AND ";
    sql += "COLOR.NAME=" + C->quote(team_color) + " AND ";
    sql += "STATE.NAME=" + C->quote(team_state) + ";";
    // cout << sql << endl;
    /* Create a non-transactional object. */
    nontransaction N(*C);
      
    /* Execute SQL query */
    result R( N.exec( sql ));
    cout << "UNIFORM_NUM " << "FIRST_NAME " << "LAST_NAME" << endl;
    /* List down all the records */
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << c[0].as<int>() << " ";
      cout << c[1].as<string>() << " ";
      cout << c[2].as<string>() << endl;
    }
}


void query5(connection *C, int num_wins)
{
    /* Create SQL statement */
    string sql = "SELECT FIRST_NAME,LAST_NAME,TEAM.NAME,WINS from PLAYER,TEAM ";
    sql += "WHERE TEAM.TEAM_ID=PLAYER.TEAM_ID AND ";
    sql += "WINS>" + to_string(num_wins) + ";";
    // cout << sql << endl;
    /* Create a non-transactional object. */
    nontransaction N(*C);
    /* Execute SQL query */
    result R( N.exec( sql ));
    cout << "FIRST_NAME " << "LAST_NAME " << "NAME " << "WINS" << endl;
    /* List down all the records */
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << c[0].as<string>() << " ";
      cout << c[1].as<string>() << " ";
      cout << c[2].as<string>() << " ";
      cout << c[3].as<int>() << endl;
    }
}
