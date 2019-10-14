#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

/* TODO:
    Experience and Levles
    More stats
    More clases
*/

using namespace std;

class Character{
    public:

        Character(){
            NAME = "None";
            MAX_HP = 1;
            HP = MAX_HP;
            ATTK = 0;
            DEF = 0;
            EXPEREIENCE = 0;
            NEXT_LEVEL = 10;
            LEVEL = 1;
        }

        Character(string name, int max_hp, int attack, int defense, int level){
            NAME = name;
            MAX_HP = max_hp;
            HP = MAX_HP;
            ATTK = attack;
            DEF = defense;
            EXPEREIENCE = 0;
            NEXT_LEVEL = 10;
            LEVEL = level;
        }

        ~Character(){}
        
        int get_hp() const { return HP; }

        int get_max_hp() const { return MAX_HP; }

        int get_attk() const { return ATTK; }

        int get_def() const { return DEF; }

        int get_exp() const { return EXPEREIENCE; };

        string get_name() const { return NAME; };

        int get_next_level_exp() const { return NEXT_LEVEL; };
   
        void set_hp(int value){
            if (value < 0){
                HP = 0;
            }else if (value > MAX_HP){
                HP = MAX_HP;
            }else{
                HP = value;
            }
        }

        string repr() const{
            return "<Character " + NAME + " : " + "[" + to_string(HP) + "/" + 
            to_string(MAX_HP) + "]  LVL " + to_string(LEVEL) + ">";
        }

        void add_exp(int value){
            while(value>0){
                if (NEXT_LEVEL > value + EXPEREIENCE){
                    EXPEREIENCE += value;
                    value = 0;
                }else{
                    value = value - NEXT_LEVEL + EXPEREIENCE;
                    EXPEREIENCE += NEXT_LEVEL;
                    
                    level_up();
                }
            }
            
        }

        void display_character_stats() const{
            cout << endl;
            cout << get_name() << " - LEVEL: " << LEVEL << endl;
            cout << "HP      : " << get_hp() << " / " << get_max_hp() << endl;
            cout << "ATACK   : " << get_attk() << endl;
            cout << "DEFENCE : " << get_def() << endl;
            cout << "EXP     : " << get_exp() << "/" << get_next_level_exp() << endl;
            cout << endl;
        }

    private:

        string NAME;
        int HP, MAX_HP, ATTK, DEF; 
        int EXPEREIENCE;
        int NEXT_LEVEL;
        int LEVEL;

        void level_up(){
            LEVEL += 1;
            MAX_HP += 5;
            ATTK += 1;
            DEF += 1;
            EXPEREIENCE = 0;
            NEXT_LEVEL += 2;
        }
};

class BattleSystem{
    public:
        BattleSystem(){
            srand(time(0));
            Character e1, e2;
            entity[PLAYER] = e1;
            entity[ENEMY] = e2;
            victory_count = 0;
            defeat_count = 0;
        }

        BattleSystem(Character& player, Character& enemy){
            srand(time(0));
            entity[PLAYER] = player;
            entity[ENEMY] = enemy;
            victory_count = 0;
            defeat_count = 0;
        }

        void player_attack(){
            cout << "PLAYER ATTACK" << endl;
            attack_entity(PLAYER, ENEMY);
        }

        void player_heal(){
            cout << "PLAYER HEALS" << endl;
            entity[PLAYER].set_hp(entity[PLAYER].get_hp() + RECUPERATE_AMOUNT);
            cout << "Heals " << RECUPERATE_AMOUNT << " points of damage" << endl;
        }

        void enemy_attack(){
            cout << "ENEMY ATTACK" << endl;
            attack_entity(ENEMY, PLAYER);
        }

        void display_characters(){
            for(int i=0; i<entity_number; i++){
                cout << entity[i].repr() << endl;
            }
        }

        void restore_characters(){
            for(int i=0; i<entity_number; i++){
                entity[i].set_hp(entity[i].get_max_hp());
            }
        }

        bool check_battle_end(){
            for(int i=0; i<entity_number; i++){
                if (entity[i].get_hp() <= 0){
                    display_characters();
                    if (i==ENEMY){
                        cout << "VICTORY" << endl;
                        victory_count++;
                        entity[PLAYER].add_exp(26);
                    }else{
                        cout << "DEFEAT" << endl;
                        defeat_count++;
                    }
                    return true;
                }
            }
            return false;
        }

        void display_vitory_count() const{
            cout << "Victories: " << victory_count << endl;
            cout << "Defeats: " << defeat_count << endl;
        }

        void display_player_stats() const{
            entity[PLAYER].display_character_stats();
        }

    private:
        const int PLAYER = 0;
        const int ENEMY = 1;
        int victory_count;
        int defeat_count;
        const static int entity_number = 2;
        int RECUPERATE_AMOUNT = 5;
        Character entity[entity_number];

        void attack_entity(int attacker_id, int defender_id){
            int extra = 1 + (rand () % 2);
            int damage = entity[attacker_id].get_attk() + extra - entity[defender_id].get_def();
            if (damage < 0){
                damage = 0;
            }
            cout << "Does " << damage << " points of damage" << endl;
            entity[defender_id].set_hp(entity[defender_id].get_hp() - damage);
        }
}; 

class Menu{
    public:
        const int ATACK = 1;
        const int HEAL = 2;
        const int STATUS = 3;

        const int YES = 1;
        const int NO = 2;

        void display_battle_options() const{
            display_options(battle_options, n_battle_options);
        }

        void display_bool_options() const{
            display_options(bool_options, n_bool_options);
        }

        int get_option(){
            int opcion;
            cin >> opcion;
            return opcion;
        }

    private:
        const static int n_battle_options = 3;
        const static int n_bool_options = 2;
        const string battle_options[n_battle_options] = {"ATACK", "HEAL", "STATUS"};
        const string bool_options[n_bool_options] = {"YES", "NO"};

        void display_options(const string array[], int total) const{
            string msg = "";
            for(int i=0; i<total; i++){
                msg += to_string(i+1) + " - " +  array[i];
                if (i < total-1){
                    msg += ", ";
                }
            }
            cout << msg << endl;
        }

};



int main()
{
    Character player("Player", 10, 3, 1, 1);
    Character enemy("Enemy", 10, 3, 1, 1);

    BattleSystem battle(player, enemy);
    Menu menu;

    int option;

    while(true){
        // Print characters
        battle.display_characters();
        menu.display_battle_options();

        option = menu.get_option();
        if(option==menu.ATACK){
            battle.player_attack();
            battle.enemy_attack();
        }else if(option==menu.HEAL){
            battle.player_heal();
            battle.enemy_attack();
        }else if (option==menu.STATUS){
            battle.display_player_stats();
        }else{
            cout << "Command Error" << endl;
        }

        

        // Check entity HP
        if (battle.check_battle_end()){
            battle.display_vitory_count();
            cout << "Continue?" << endl;
            menu.display_bool_options();
            option = menu.get_option();
            if (option ==  menu.NO){
               break;
            }else{
                battle.restore_characters();
            }
        }
    }

    cout << "GAME OVER" << endl;
    return 0;
}
