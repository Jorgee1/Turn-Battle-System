#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <map>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <iterator>

#include "common.h"
#include "window.h"
#include "character.h"
#include "texture.h"


using namespace std;



/* TODO:
    Ataques especiales
    Estado
    Inventario
*/


class Menu{
    public:
        const int ATACK = 0;
        const int HEAL = 1;
        const int STATUS = 2;

        const int YES = 0;
        const int NO = 1;

        void display_battle_options(int x, int y, TextureText &textNormal, TextureText &textBold) const{
            int acc = 0;
            int padding = 20;
            for (int i=0; i<battle_options.size(); i++){
                if (i==selector){
                    textBold.create_texture(battle_options[i]);
                    textBold.render(x + acc, y);
                    acc = acc + x + textBold.rect.w + padding;
                }else{
                    textNormal.create_texture(battle_options[i]);
                    textNormal.render(x + acc, y);
                    acc = acc + x + textNormal.rect.w + padding;
                }
            }
        }

        void next_option(){
            if(selector < battle_options.size() - 1){
                selector++;
            }
        }

        void previews_option(){
            if(selector > 0){
                selector--;
            }            
        }

        int get_selector(){
            return selector;
        }

        void next_binary_option(){
            if(binary_selector < bool_options.size() - 1){
                binary_selector++;
            }
        }

        void previews_binary_option(){
            if(binary_selector > 0){
                binary_selector--;
            }            
        }

        int get_binary_selector(){
            return binary_selector;
        }

        const vector<string> battle_options = {"ATACK", "HEAL", "STATUS"};
        const vector<string> bool_options = {"YES", "NO"};

    private:
        int selector = 0;
        int binary_selector = 0;

};

class BattleSystem{
    public:
        Hero *player;
        Monster *monster;
        BattleSystem(){
            srand(time(0));
            player = NULL;
            monster = NULL;
            victory_count = 0;
            defeat_count = 0;
        }

        ~BattleSystem(){
            player = NULL;
            monster = NULL;
            victory_count = 0;
            defeat_count = 0;
        }

        BattleSystem(Hero& player, Monster& enemy){
            srand(time(0));
            this->player = &player;
            this->monster = &enemy;
            victory_count = 0;
            defeat_count = 0;
        }

        string player_attack(){
            int damage = damage_formula(player->get_attk().get_value(), monster->get_def().get_value());
            monster->set_hp(monster->get_hp().get_value() - damage);
            printf("%s\n", monster->repr().c_str());
            return "PLAYER ATTACK, Does " + to_string(damage) + " points of damage";
        }

        string enemy_attack(){ 
            int damage = damage_formula(monster->get_attk().get_value(), player->get_def().get_value());
            player->set_hp(player->get_hp().get_value() - damage);
            return "ENEMY ATTACK, Does " + to_string(damage) + " points of damage";            
        }

        string player_heal(){
            player->set_hp(player->get_hp().get_value() + RECUPERATE_AMOUNT);
            return "PLAYER HEALS, Heals " + to_string(RECUPERATE_AMOUNT) + " points of damage";
        }

        void restore_characters(){
            player->set_hp(player->get_max_hp().get_value());
            monster->set_hp(monster->get_max_hp().get_value());
        }

        bool check_battle_end(){
            int HPs[] = {player->get_hp().get_value(), monster->get_hp().get_value()};
            int total = sizeof(HPs)/sizeof(HPs[0]);

            for(int i=0; i<total; i++){
                if (HPs[i] <= 0){
                    if (i==ENEMY){
                        victory_count++;
                        player->add_exp(monster->get_exp_reward());
                    }else{
                        defeat_count++;
                    }
                    return true;
                }
            }
            return false;
        }

    private:
        const int PLAYER = 0;
        const int ENEMY = 1;
        int victory_count;
        int defeat_count;
        int RECUPERATE_AMOUNT = 5;

        int damage_formula(int caller_attack, int target_defense){
            int extra = 1 + (rand () % 2);
            int damage = caller_attack + extra - target_defense;
            if (damage < 0){
                damage = 0;
            }
            return damage;
        }
}; 

int main(int argc, char* args[] ){
    string PATH_ICON = "icon.bmp";
    string FONT_PATH_REGULAR = "fonts/LiberationMono-Regular.ttf";

    int LAYOUT_COMBAT = 0;
    int LAYOUT_STATUS = 1;
    int LAYOUT_GAME_OVER = 2;
    int layout_current = 0;
    int FONT_SIZE = 18;

    SDL_Color colors[TOTAL_TEXT] = {
        {0x00, 0x00, 0x00, 0xFF},
        {0xFF, 0x00, 0x00, 0xFF},
        {0x00, 0xFF, 0x00, 0xFF},
        {0x00, 0x00, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF}
    };

    controls controls_rules = {
        SDL_SCANCODE_Z,
        SDL_SCANCODE_X,

        SDL_SCANCODE_RETURN,
        
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_RIGHT
    };
    controls_locks locks;

    
    Hero player;
    Monster enemy;

    player.init("Player", 10, 3, 1, 1);
    enemy.init("Enemy", 10, 3, 1, 1, 25);

    Window window("Test", 800, 600, colors[TEXT_BLACK]);
    window.set_icon(PATH_ICON);
    
    BattleSystem battle(player, enemy);
    Menu menu;


    // Load Media
    TextureText text_normal_obj[TOTAL_TEXT];

    for(int i=0; i<TOTAL_TEXT; i++){
        text_normal_obj[i].init(window.get_render(), FONT_PATH_REGULAR, colors[i], FONT_SIZE);
    }


    int normal_text_color   = TEXT_WHITE;
    int selected_text_color = TEXT_RED;

    const Uint8* currentKeyStates = NULL;

    vector<string> mesages;
    bool exit = false;
    int acc = 0;

    while(!exit){
        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();

            
            // Layouts
            if (layout_current == LAYOUT_COMBAT){
                text_normal_obj[normal_text_color].create_texture(player.repr());
                text_normal_obj[normal_text_color].render(0, 0);
                text_normal_obj[normal_text_color].create_texture(enemy.repr());
                text_normal_obj[normal_text_color].render(0, 0 + text_normal_obj[normal_text_color].rect.h);


                menu.display_battle_options(0, 50, text_normal_obj[normal_text_color], text_normal_obj[selected_text_color]);

                acc = 0;
                for(int i=0;i<mesages.size();i++){
                    text_normal_obj[normal_text_color].create_texture(mesages[i]);
                    text_normal_obj[normal_text_color].render(0, 70 + acc);
                    acc = acc + text_normal_obj[normal_text_color].rect.h + 10;
                }

            }else if (layout_current == LAYOUT_STATUS){
                acc = 0;
                text_normal_obj[normal_text_color].create_texture(player.get_name());
                text_normal_obj[normal_text_color].render(0, 0+acc);
                acc = acc + text_normal_obj[normal_text_color].rect.h;

                vector<Status> player_status = player.get_stats();
                for(int i=0; i<player_status.size(); i++){
                    string temp_text = player_status[i].get_name() + ": " + to_string(player_status[i].get_value());
                    text_normal_obj[normal_text_color].create_texture(temp_text);
                    text_normal_obj[normal_text_color].render(0, 0+acc);
                    acc = acc + text_normal_obj[normal_text_color].rect.h;
                }

            }else if(layout_current == LAYOUT_GAME_OVER){
                text_normal_obj[normal_text_color].create_texture("GAME OVER");
                text_normal_obj[normal_text_color].render(0, 0);

                acc = text_normal_obj[normal_text_color].rect.h;
                
                text_normal_obj[normal_text_color].create_texture("Continue?");
                text_normal_obj[normal_text_color].render(0, 0 + acc);
                acc = acc + text_normal_obj[normal_text_color].rect.h + 10;

                int x_acc = 0;
                for(int i=0; i<menu.bool_options.size();i++){
                    if(i==menu.get_binary_selector()){
                        text_normal_obj[selected_text_color].create_texture(menu.bool_options[i]);
                        text_normal_obj[selected_text_color].render(0 + x_acc, 0 + text_normal_obj[selected_text_color].rect.h + acc);
                        x_acc = x_acc + text_normal_obj[selected_text_color].rect.w + 10;
                    }else{
                        text_normal_obj[normal_text_color].create_texture(menu.bool_options[i]);
                        text_normal_obj[normal_text_color].render(0 + x_acc, 0 + text_normal_obj[normal_text_color].rect.h + acc);
                        x_acc = x_acc + text_normal_obj[normal_text_color].rect.w + 10;
                    }
                }

            }

                    
            currentKeyStates = SDL_GetKeyboardState( NULL );

            if (layout_current == LAYOUT_COMBAT){
                if( (currentKeyStates[controls_rules.move_right]) && (!locks.move_right) ){
                    menu.next_option();
                    locks.move_right = true;
                }else if( (currentKeyStates[controls_rules.move_left]) && (!locks.move_left) ){
                    menu.previews_option();
                    locks.move_left = true;
                }

                if( (currentKeyStates[controls_rules.action_button]) && (!locks.action_button) ){
                    if(menu.get_selector()==menu.ATACK){
                        mesages.clear();
                        mesages.push_back(battle.player_attack());
                        mesages.push_back(battle.enemy_attack());
                    }else if(menu.get_selector()==menu.HEAL){
                        mesages.clear();
                        mesages.push_back(battle.player_heal());
                        mesages.push_back(battle.enemy_attack());
                    }else if (menu.get_selector()==menu.STATUS){
                        layout_current = LAYOUT_STATUS;
                    }

                    if (battle.check_battle_end()){
                        battle.restore_characters();
                        layout_current = LAYOUT_GAME_OVER;
                    }

                    locks.action_button = true;
                }
            }else if (layout_current == LAYOUT_STATUS){
                if( (currentKeyStates[controls_rules.cancel_button]) && (!locks.cancel_button) ){
                    layout_current = LAYOUT_COMBAT;
                    locks.cancel_button = true;
                }
            }else if (layout_current == LAYOUT_GAME_OVER){
                if( (currentKeyStates[ controls_rules.move_right ]) && (!locks.move_right) ){
                    menu.next_binary_option();
                    locks.action_button = true;
                }else if( (currentKeyStates[controls_rules.move_left]) && (!locks.move_left) ){
                    menu.previews_binary_option();
                    locks.move_left = true;
                }

                if( (currentKeyStates[controls_rules.action_button]) && (!locks.action_button) ){
                    if(menu.get_binary_selector() == menu.YES){
                        layout_current = LAYOUT_COMBAT;
                        mesages.clear();
                    }else if(menu.get_binary_selector() == menu.NO){
                        exit = true;
                    }
                    locks.action_button = true;
                }


            }

            if(( !currentKeyStates[controls_rules.move_right] ) && (locks.move_right)){
                locks.move_right = false;
            }

            if(( !currentKeyStates[controls_rules.move_left] ) && (locks.move_left)){
                locks.move_left = false;
            }

            if(( !currentKeyStates[controls_rules.action_button] ) && (locks.action_button)){
                locks.action_button = false;
            }

            if(( !currentKeyStates[controls_rules.cancel_button] ) && (locks.cancel_button)){
                locks.cancel_button = false;
            }
        
            window.update_screen();
        }
        

    }
    return 0;
}
