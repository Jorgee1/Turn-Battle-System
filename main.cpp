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

        void set_player(Hero& player){
            this->player = &player;
        }

        void set_enemy(Monster& enemy){
            this->monster = &enemy;
        }

        string player_attack(){
            int damage = damage_formula(player->get_attk().get_value(), monster->get_def().get_value());
            monster->set_hp(monster->get_hp().get_value() - damage);
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

class BattleLayout{
    public:
        vector<string> mesages;
        BattleSystem battle;
        TextureText *text;

        const enum ACTIONS{
            ATACK,
            HEAL,
            STATUS,

            TOTAL_ACTIONS
        };

        string action_names[TOTAL_ACTIONS] = {
            "ATTACK",
            "HEAL",
            "STATUS"
        };

        int selector;

        const Uint8* currentKeyStates;
        int* layout_current;
        controls* controls_rules;
        controls_locks* locks;

        BattleLayout(){
            currentKeyStates = NULL;
            layout_current   = NULL;
            controls_rules   = NULL;
            locks            = NULL;
            text             = NULL;
            selector = 0;
        }

        ~BattleLayout(){
            currentKeyStates = NULL;
            layout_current   = NULL;
            controls_rules   = NULL;
            locks            = NULL;
            text             = NULL;
            selector = 0;
        }

        BattleLayout(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
            init(layout_current, controls_rules, locks, text_obj);
        }

        void init(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
            currentKeyStates = SDL_GetKeyboardState(NULL);
            this->layout_current = &layout_current;
            this->controls_rules = &controls_rules;
            this->locks          = &locks;
            this->text           = text_obj;

            selector = 0;
        }

        void next_option(){
            if(selector < TOTAL_ACTIONS - 1){
                selector++;
            }
        }

        void previews_option(){
            if(selector > 0){
                selector--;
            }            
        }

        void action(){
            if(selector==ATACK){
                mesages.clear();
                mesages.push_back(battle.player_attack());
                mesages.push_back(battle.enemy_attack());
            }else if(selector==HEAL){
                mesages.clear();
                mesages.push_back(battle.player_heal());
                mesages.push_back(battle.enemy_attack());
            }else if (selector==STATUS){
                *layout_current = LAYOUT_STATUS;
            }

            if (battle.check_battle_end()){
                battle.restore_characters();
                *layout_current = LAYOUT_GAME_OVER;
            }
        }

        void check_player_action(){
            if( (currentKeyStates[controls_rules->move_right]) && (!locks->move_right) ){
                next_option();
                locks->move_right = true;
            }else if( (currentKeyStates[controls_rules->move_left]) && (!locks->move_left) ){
                previews_option();
                locks->move_left = true;
            }

            if( (currentKeyStates[controls_rules->action_button]) && (!locks->action_button) ){
                action();
                locks->action_button = true;
            }

            if(( !currentKeyStates[controls_rules->move_right] ) && (locks->move_right)){
                locks->move_right = false;
            }
            if(( !currentKeyStates[controls_rules->move_left] ) && (locks->move_left)){
                locks->move_left = false;
            }
            if(( !currentKeyStates[controls_rules->action_button] ) && (locks->action_button)){
                locks->action_button = false;
            }
            if(( !currentKeyStates[controls_rules->cancel_button] ) && (locks->cancel_button)){
                locks->cancel_button = false;
            }
        }

        void render(int x, int y){
            int acc_x = 0;
            int acc_y = 0;
            int padding = 20;


            text[TEXT_WHITE].create_texture((battle.player)->repr());
            text[TEXT_WHITE].render(x, y);
            acc_y += text[TEXT_WHITE].rect.h;

            text[TEXT_WHITE].create_texture((battle.monster)->repr());
            text[TEXT_WHITE].render(x, y + acc_y);
            acc_y += text[TEXT_WHITE].rect.h;


            for (int i=0; i<TOTAL_ACTIONS; i++){
                if (i==selector){
                    text[TEXT_RED].create_texture(action_names[i]);
                    text[TEXT_RED].render(x + acc_x, y + acc_y);
                    acc_x += text[TEXT_RED].rect.w + padding;
                }else{
                    text[TEXT_WHITE].create_texture(action_names[i]);
                    text[TEXT_WHITE].render(x + acc_x, y + acc_y);
                    acc_x += text[TEXT_WHITE].rect.w + padding;
                }
            }

            acc_x = 0;
            acc_y += text[TEXT_WHITE].rect.h + padding;

            for(int i=0;i<mesages.size();i++){
                text[TEXT_WHITE].create_texture(mesages[i]);
                text[TEXT_WHITE].render(x + acc_x, y + acc_y);
                acc_y += text[TEXT_WHITE].rect.h;
            }
        }

};

class StatsLayout{
    public:
        TextureText *text;
        const Uint8* currentKeyStates;
        int* layout_current;
        controls* controls_rules;
        controls_locks* locks;
        Hero* player;

        StatsLayout(){
            currentKeyStates = NULL;
            layout_current   = NULL;
            controls_rules   = NULL;
            locks            = NULL;
            text             = NULL;
            player           = NULL;
        }

        ~StatsLayout(){
            currentKeyStates = NULL;
            layout_current   = NULL;
            controls_rules   = NULL;
            locks            = NULL;
            text             = NULL;
            player           = NULL;
        }

        StatsLayout(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
            init(layout_current, controls_rules, locks, text_obj);
        }

        void init(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
            currentKeyStates = SDL_GetKeyboardState(NULL);
            this->layout_current = &layout_current;
            this->controls_rules = &controls_rules;
            this->locks          = &locks;
            this->text           = text_obj;
        }

        void set_player(Hero &player){
            this->player = &player;
        }

        void action(){
            *layout_current = LAYOUT_COMBAT;
        }

        void check_player_action(){

            if( (currentKeyStates[controls_rules->action_button]) && (!locks->action_button) ){
                action();
                locks->action_button = true;
            }

            if(( !currentKeyStates[controls_rules->action_button] ) && (locks->action_button)){
                locks->action_button = false;
            }
        }

        void render(int x, int y){
            int acc_x = 0;
            int acc_y = 0;

            text[TEXT_WHITE].create_texture(player->get_name());
            text[TEXT_WHITE].render(x + acc_x, y + acc_y);
            acc_y += text[TEXT_WHITE].rect.h;

            vector<Status> player_status = player->get_stats();
            for(int i=0; i<player_status.size(); i++){
                string temp_text = player_status[i].get_name() + ": " + to_string(player_status[i].get_value());
                text[TEXT_WHITE].create_texture(temp_text);
                text[TEXT_WHITE].render(x + acc_x, y + acc_y);
                acc_y += text[TEXT_WHITE].rect.h;
            }
        }
};

class GameOverLayout{
    public:
        TextureText *text;
        const Uint8* currentKeyStates;
        int* layout_current;

        controls* controls_rules;
        controls_locks* locks;

        const enum ACTIONS{
            YES,
            NO,

            TOTAL_ACTIONS
        };

        string action_names[TOTAL_ACTIONS] = {
            "YES",
            "NO"
        };

        int selector;
        bool exit;

        GameOverLayout(){
            currentKeyStates = NULL;
            layout_current   = NULL;
            controls_rules   = NULL;
            locks            = NULL;
            text             = NULL;

            exit = false;
            selector = 0;
        }

        ~GameOverLayout(){
            currentKeyStates = NULL;
            layout_current   = NULL;
            controls_rules   = NULL;
            locks            = NULL;
            text             = NULL;
            
            exit = false;
            selector = 0;
        }

        GameOverLayout(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
            init(layout_current, controls_rules, locks, text_obj);
        }

        void init(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
            currentKeyStates = SDL_GetKeyboardState(NULL);
            this->layout_current = &layout_current;
            this->controls_rules = &controls_rules;
            this->locks          = &locks;
            this->exit           = &exit;
            this->text           = text_obj;

            exit = false;
            selector = 0;
        }


        void next_option(){
            if(selector < TOTAL_ACTIONS - 1){
                selector++;
            }
        }

        void previews_option(){
            if(selector > 0){
                selector--;
            }            
        }

        bool check_exit(){
            return exit;
        }

        void action(){
            if(selector == YES){
                *layout_current = LAYOUT_COMBAT;
            }else if(selector == NO){
                exit = true;
            }
        }

        void check_player_action(){
            if( (currentKeyStates[ controls_rules->move_right ]) && (!locks->move_right) ){
                next_option();
                locks->action_button = true;
            }else if( (currentKeyStates[controls_rules->move_left]) && (!locks->move_left) ){
                previews_option();
                locks->move_left = true;
            }

            if( (currentKeyStates[controls_rules->action_button]) && (!locks->action_button) ){
                action();
                locks->action_button = true;
            }

            if(( !currentKeyStates[controls_rules->move_right] ) && (locks->move_right)){
                locks->move_right = false;
            }
            if(( !currentKeyStates[controls_rules->move_left] ) && (locks->move_left)){
                locks->move_left = false;
            }
            if(( !currentKeyStates[controls_rules->action_button] ) && (locks->action_button)){
                locks->action_button = false;
            }
        }

        void render(int x, int y){
            int acc_x = 0;
            int acc_y = 0;
            int padding = 10;

            text[TEXT_WHITE].create_texture("GAME OVER");
            text[TEXT_WHITE].render(x + acc_x, y + acc_y);

            acc_y += text[TEXT_WHITE].rect.h;
            
            text[TEXT_WHITE].create_texture("Continue?");
            text[TEXT_WHITE].render(x + acc_x, y + acc_y);

            acc_y += text[TEXT_WHITE].rect.h + padding;

            for(int i=0; i<TOTAL_ACTIONS;i++){
                if(i==selector){
                    text[TEXT_RED].create_texture(action_names[i]);
                    text[TEXT_RED].render(x + acc_x, y + acc_y + text[TEXT_RED].rect.h);
                    acc_x += text[TEXT_RED].rect.w + padding;
                }else{
                    text[TEXT_WHITE].create_texture(action_names[i]);
                    text[TEXT_WHITE].render(x + acc_x, y + acc_y + text[TEXT_WHITE].rect.h);
                    acc_x += text[TEXT_WHITE].rect.w + padding;
                }
            }
        }

};

int main(int argc, char* args[] ){
    const string GAME_NAME = "Turn Battle System";
    const string PATH_ICON = "icon.bmp";
    const string FONT_PATH_REGULAR = "fonts/LiberationMono-Regular.ttf";

    const int SCREEN_WIDTH  = 800;
    const int SCREEN_HEIGHT = 600;
    const int FONT_SIZE     =  18;

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

    int layout_current      = 0;   
    bool exit               = false;

    // Sub systems init
    Window window(GAME_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, colors[TEXT_BLACK]);
    window.set_icon(PATH_ICON);
    
    TextureText text_normal_obj[TOTAL_TEXT];
    for(int i=0; i<TOTAL_TEXT; i++){
        text_normal_obj[i].init(window.get_render(), FONT_PATH_REGULAR, colors[i], FONT_SIZE);
    }    

    // Game init
    Hero player;
    Monster enemy;

    player.init("Player", 10, 3, 1, 1);
    enemy.init("Enemy", 10, 3, 1, 1, 25);

    BattleLayout battle_layout(layout_current, controls_rules, locks, text_normal_obj);
    battle_layout.battle.set_player(player);
    battle_layout.battle.set_enemy(enemy);

    StatsLayout stats_layout(layout_current, controls_rules, locks, text_normal_obj);
    stats_layout.set_player(player);

    GameOverLayout gameover_layout(layout_current, controls_rules, locks, text_normal_obj);

    while(!exit){
        if(window.check_exit() || gameover_layout.check_exit()){
            exit = true;
        }else{
            window.clear_screen();

            // Layouts
            if (layout_current == LAYOUT_COMBAT){
                battle_layout.check_player_action();
                battle_layout.render(0, 0);
            }else if (layout_current == LAYOUT_STATUS){
                stats_layout.check_player_action();
                stats_layout.render(0, 0);
            }else if(layout_current == LAYOUT_GAME_OVER){
                gameover_layout.check_player_action();
                gameover_layout.render(0, 0);
            }

            window.update_screen();
        }
        
    }
    return 0;
}
