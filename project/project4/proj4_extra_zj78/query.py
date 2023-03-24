from player_models import *

def query1(use_mpg, min_mpg, max_mpg,
            use_ppg, min_ppg, max_ppg,
            use_rpg, min_rpg, max_rpg,
            use_apg, min_apg, max_apg,
            use_spg, min_spg, max_spg,
            use_bpg, min_bpg,max_bpg
            ):
    query = Player.select()
    if (use_mpg):
        query = query.filter(mpg__lte=max_mpg, mpg__gte=min_mpg)
    if (use_ppg):
        query = query.filter(ppg__lte=max_ppg, ppg__gte=min_ppg)
    if (use_rpg):
        query = query.filter(rpg__lte=max_rpg, rpg__gte=min_rpg)
    if (use_apg):
        query = query.filter(apg__lte=max_apg, apg__gte=min_apg)
    if (use_spg):
        query = query.filter(spg__lte=max_spg, spg__gte=min_spg)
    if (use_bpg):
        query = query.filter(bpg__lte=max_bpg, bpg__gte=min_bpg)
    print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME ", end='')
    print("MPG PPG RPG APG SPG BPG")
    for player in query:
        print("{} {} {} {} {} {} {} {} {} {} {}".format(player.player_id, 
                                    player.team_id,
                                   player.uniform_num, player.first_name,
                                   player.last_name,player.mpg, 
                                   player.ppg, player.rpg, player.apg,
                                   player.spg, player.bpg))
    pass

def queryTest():
    query = Team.select()
    res = query.filter(name="UNC")
    print("NAME ")
    for team in res:
        print(team.name)
    pass

def query2(team_color):
    query = Team.select(Team.name).join(Color).where(Color.name==team_color)
    print("NAME ")
    for team in query:
        print(team.name)
   
def query3(team_name):
    query = Player.select(Player.first_name, Player.last_name).join(Team).where(Team.name==team_name).order_by(Player.ppg.desc())     
    print("FIRST_NAME LAST_NAME")
    for player in query:
        print("{} {}".format(player.first_name, player.last_name))

def query4(team_state, team_color):
    query = Player.select(Player.uniform_num, Player.first_name, Player.last_name).join(Team).join_from(Team, Color).join_from(Team, State).where((Color.name==team_color) & (State.name==team_state))
    # print(query.sql())
    print("UNIFORM_NUM FIRST_NAME LAST_NAME")
    for player in query:
        print("{} {} {}".format(player.uniform_num, player.first_name, player.last_name))

def query5(num_wins):
    query = Player.select(Player.first_name, Player.last_name, Team.name, Team.wins).join(Team).where(Team.wins>num_wins)
    # print(query.sql())
    print("FIRST_NAME LAST_NAME NAME WINS")
    for player in query:
        print("{} {} {} {}".format(player.first_name, player.last_name, player.team.name, player.team.wins))

query1(1, 10, 20, 1, 5, 10, 1, 7, 10, 0, 0, 0, 1, 0.2, 1.0, 1, 0.5, 3.0)
query1(1,35, 40,\
    0, 13, 19,\
    0, 5, 9,\
    0, 1, 4,\
    0, 0.8, 1.5,\
    0, 0.4, 3.4)
query2("Green")
query3("Miami")
query4("NC", "Green")
query5(12)