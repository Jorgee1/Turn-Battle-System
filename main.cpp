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
#include "layout.h"

using namespace std;

/* TODO:
    Ataques especiales
    Estado
    Inventario
*/

struct item{
    int id   = 0;
    int type = 0;
};

class ItemDB{
    public:
        Hero *player;
        enum types{
            TYPE_NONE,
            TYPE_HEALING,
            TOTAL_ITEM_TYPE
        };

        enum healing_items{
            POTION,
            HIPOTION,
            MEGAPOTION,
            TOTAL_HEALING_ITEMS
        };

        ItemDB(){
            player = NULL;
        }

        ~ItemDB(){
            player = NULL;
        }

        void assign_player(Hero &player){
            this->player = &player;
        }

        void use_item(item seleted_item){
            switch (seleted_item.type)
            {
                case TYPE_NONE:{
                    printf("Nothing to do here\n");
                    break;
                }
                case TYPE_HEALING:{
                    switch (seleted_item.id)
                    {
                        case POTION:{
                            printf("Heal 5 HP");
                            break;
                        }
                        case HIPOTION:{
                            printf("Heal 10 HP");
                            break;
                        }
                        case MEGAPOTION:{
                            printf("Heal 15 HP");
                            break;
                        }
                        default:{
                            break;
                        }

                    }
                    break;
                }
                default:{
                    break;
                }

            }
        }
};

class InventoryLayout{
    public:
        TextureText*               text;
        controls*        controls_rules;
        controls_locks*           locks;
        
        const Uint8*   currentKeyStates;

        int* layout_current;
        int selector;

        vector<item>* inventory;

        InventoryLayout(){
            this->text           = NULL;
            this->controls_rules = NULL;
            this->locks          = NULL;
            currentKeyStates     = NULL;
            layout_current       = NULL;
            selector             =    0;
        }

        ~InventoryLayout(){
            this->text           = NULL;
            this->controls_rules = NULL;
            this->locks          = NULL;
            currentKeyStates     = NULL;
            layout_current       = NULL;
            selector             =    0;
        }

        InventoryLayout(
            int &layout_current,
            controls &controls_rules,
            controls_locks &locks,
            TextureText &text
        ){
            this->text           = &text;
            this->controls_rules = &controls_rules;
            this->locks          = &locks;
            this->layout_current = &layout_current;
            currentKeyStates     = SDL_GetKeyboardState(NULL);
            selector             = 0;
        }


        void next_option(){

        }

        void previews_option(){
        
        }

        void action(){

        }

        void cancel (){
            layout_current = 0;
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

            if( (currentKeyStates[controls_rules->action_button]) && (!locks->action_button) ){
                cancel();
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
        }
        
};

int main(int argc, char* args[] ){
    const string GAME_NAME         = "Turn Battle System";
    const string PATH_ICON         = "icon.bmp";
    const string FONT_PATH_REGULAR = "fonts/LiberationMono-Regular.ttf";

    const int SCREEN_WIDTH  = 800;
    const int SCREEN_HEIGHT = 600;
    const int FONT_SIZE     =  18;
    int layout_current      =   0;

    bool exit = false;

    SDL_Color colors[TOTAL_TEXT] = {
        {0x00, 0x00, 0x00, 0xFF},
        {0xFF, 0x00, 0x00, 0xFF},
        {0x00, 0xFF, 0x00, 0xFF},
        {0x00, 0x00, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF}
    };

    controls_locks locks;
    controls controls_rules = {
        SDL_SCANCODE_Z,
        SDL_SCANCODE_X,

        SDL_SCANCODE_RETURN,
        
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_RIGHT
    };




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

    ItemDB item_db;
    item inventory[10];

    inventory[0] = {item_db.POTION, item_db.TYPE_HEALING};
    item_db.use_item(inventory[0]);
    inventory[0] = {0, item_db.TYPE_NONE};
    

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
            }else if(layout_current == LAYOUT_INVENTORY){

            }

            window.update_screen();
        }
        
    }
    return 0;
}
