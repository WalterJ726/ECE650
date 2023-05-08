from peewee import *

database = PostgresqlDatabase('ACC_BBALL', **{'user': 'postgres', 'password': 'passw0rd'})
class UnknownField(object):
    def __init__(self, *_, **__): pass

class BaseModel(Model):
    class Meta:
        database = database

class Color(BaseModel):
    color_id = AutoField()
    name = CharField(null=True)

    class Meta:
        table_name = 'color'

class State(BaseModel):
    name = CharField(null=True)
    state_id = AutoField()

    class Meta:
        table_name = 'state'

class Team(BaseModel):
    color = ForeignKeyField(column_name='color_id', field='color_id', model=Color, null=True)
    losses = IntegerField(null=True)
    name = CharField(null=True)
    state = ForeignKeyField(column_name='state_id', field='state_id', model=State, null=True)
    team_id = AutoField()
    wins = IntegerField(null=True)

    class Meta:
        table_name = 'team'

class Player(BaseModel):
    apg = IntegerField(null=True)
    bpg = DecimalField(null=True)
    first_name = CharField(null=True)
    last_name = CharField(null=True)
    mpg = IntegerField(null=True)
    player_id = AutoField()
    ppg = IntegerField(null=True)
    rpg = IntegerField(null=True)
    spg = DecimalField(null=True)
    team = ForeignKeyField(column_name='team_id', field='team_id', model=Team, null=True)
    uniform_num = IntegerField(null=True)

    class Meta:
        table_name = 'player'

