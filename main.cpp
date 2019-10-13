#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

class Character{
    public:

        Character(int max_hp=10){
            MAX_HP = max_hp;
            HP = MAX_HP;
            ATTK = 2;
        }

        int get_hp(){
            return HP;
        }

        int get_attk(){
            return ATTK;
        }

        int get_max_hp(){
            return MAX_HP;
        }

        void set_hp(int value){
            if (value > MAX_HP){
                HP = MAX_HP;
            }else{
                HP = value;
            }
 
            if (value < 0){
                HP = 0;
            }
        }

        string repr(){
            return "<Character [" + to_string(HP) + "/" + to_string(MAX_HP) + "]>";
        }

    private:
        int HP, MAX_HP, ATTK; 


};

void damage_entity(Character& atacker, Character& taker){
    int extra = 1 + (rand () % 2);
    cout << sizeof(atacker) << endl;
    taker.set_hp(taker.get_hp() - (atacker.get_attk() + extra) );
}


void display_characters(Character entity[], int total){
    for(int i=0; i<total; i++){
        cout << i << " - " << entity[i].repr() << endl;
    }
}

void restore_characters(Character entity[], int total){
    for(int i=0; i<total; i++){
        entity[i].set_hp(entity[i].get_max_hp());
    }
}

int main()
{
    int opcion;
    int flag_continue;

    int ATACK = 1;
    int RECUPERATE = 2;
    int RECUPERATE_AMOUNT = 5;
    int PLAYER = 0;
    int ENEMY = 1;
    int victory_count = 0;
    int defeat_count = 0;

    bool SW = true;
    const int entity_n = 2;
    srand(time(0));
    Character entity[entity_n];

    while(SW){
        // Print characters
        display_characters(entity, entity_n);

        cout << "1-ATACK, 2-RECUPERATE" << endl;
        cin >> opcion;

        if(opcion==ATACK){
            cout << "PLAYER ATTACK" << endl;
            damage_entity(entity[PLAYER], entity[ENEMY]);
        }else{
            cout << "PLAYER RECUPERATES" << endl;
            entity[PLAYER].set_hp(entity[PLAYER].get_hp() + RECUPERATE_AMOUNT);
        }
        cout << "ENEMY ATTACK" << endl;
        damage_entity(entity[ENEMY], entity[PLAYER]);

        // Check entity HP
        for(int i=0; i<entity_n; i++){
            if (entity[i].get_hp() <= 0){
                display_characters(entity, entity_n);
                if (i==ENEMY){
                    cout << "VICTORY" << endl;
                    victory_count++;
                }else{
                    cout << "DEFEAT" << endl;
                    defeat_count++;
                }

                cout << "Victories: " << victory_count << endl;
                cout << "Defeats: " << defeat_count << endl;

                cout << "Continue?" << endl;
                cout << "1- YES, 2- NO" << endl;
                cin >> flag_continue;
                if (flag_continue ==  2){
                    SW = false;
                    break;
                }
                restore_characters(entity, entity_n);
            }
        }
    }


    cout << "GAME OVER" << endl;
    return 0;
}
