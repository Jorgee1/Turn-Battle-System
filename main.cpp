#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

#include "Character.h"



using namespace std;



/* TODO:
    Inventario
    Estado
*/


class Window{
    public:
        Window(){
            SCREEN_WIDTH = 640;
            SCREEN_HEIGHT = 480;
            init("");
        }

        Window(string window_name, int width, int height){
            SCREEN_WIDTH = width;
            SCREEN_HEIGHT = height;
            init(window_name);
        }

        void init(string window_name){
            if(SDL_Init( SDL_INIT_VIDEO ) < 0){
                printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            }else{
                window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
                if(window == NULL){
                    printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
                }else{
                    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                    if(render==NULL){
                        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                    }else{
                        if(TTF_Init()<0){
                            printf("TTF_Init: %s\n", TTF_GetError());
                        }
                    }
                }
            }
        }

        ~Window(){
            SDL_DestroyRenderer(render);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
        }

        bool check_exit(){
            while( SDL_PollEvent( &e ) != 0 )
            {
                if( e.type == SDL_QUIT )
                {
                    return false;
                }
            }
            return true;
        }

        void clear_screen(){
            SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(render);
        }

        void update_screen(){
            SDL_RenderPresent(render);
        }

        SDL_Renderer* get_render(){
            return render;
        }

        SDL_Window* get_window(){
            return window;
        }

    private:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
        SDL_Window* window = NULL;
        SDL_Renderer* render = NULL;
        SDL_Event e;

};

class Texture{
    public:
        int h;
        int w;
        SDL_Texture* texture;
        SDL_Renderer* renderer;

        Texture(){
            h = 0;
            w = 0;
            texture = NULL;
            renderer = NULL;
        }

        Texture(SDL_Renderer* renderer){
            texture = NULL;
            h = 0;
            w = 0;
            this->renderer = renderer;
        }

        ~Texture(){
            free();
        }

        void free(){
            if(texture!=NULL){
                SDL_DestroyTexture(texture);
                texture=NULL;
                h=0;
                w=0;
            }
        }

        void render(int x, int y){
            SDL_Rect renderQuad = { x, y, w, h};
            SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
        }
};

class TextureText: public Texture{
    public:
        TTF_Font* font;

        TextureText(){
            font = NULL;
        }

        TextureText(SDL_Renderer* renderer, TTF_Font* font){
            texture = NULL;
            h = 0;
            w = 0;
            this->renderer = renderer;
            this->font = font;
        }

        void create_text_texture(string text, SDL_Color textColor){
            free();
            SDL_Surface* temp_surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
            if( temp_surface == NULL )
            {
                printf("Unable to create text! SDL_TTF Error: %s\n", TTF_GetError());
            }else{
                texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
                if( texture == NULL ){
                    printf( "Unable to create texture! SDL Error: %s\n", SDL_GetError() );
                }else{
                    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
                    w = w;
                    h = h;
                    SDL_FreeSurface(temp_surface);
                }
            }
        }
};

class Font{
    public:
        TTF_Font *font;

        Font(){
            font = NULL;
        }

        Font(string path, int size){
            font = TTF_OpenFont(path.c_str(), size);
            if( font == NULL ){
                printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
            }
        }

        ~Font(){
            TTF_CloseFont(font);
        }

        
};

class Menu{
    public:
        const int ATACK = 0;
        const int HEAL = 1;
        const int STATUS = 2;

        const int YES = 1;
        const int NO = 2;

        void display_battle_options(int x, int y, TextureText textNormal, TextureText textBold, SDL_Color color) const{
            int acc = 0;
            int padding = 20;
            for (int i=0; i<n_battle_options; i++){
                if (i==selector){
                    textBold.create_text_texture(battle_options[i].c_str(), color);
                    textBold.render(x + acc, y);
                    acc = acc + x + textBold.w + padding;
                }else{
                    textNormal.create_text_texture(battle_options[i].c_str(), color);
                    textNormal.render(x + acc, y);
                    acc = acc + x + textNormal.w + padding;
                }
            }
        }

        void display_bool_options() const{
            display_options(bool_options, n_bool_options);
        }

        void next_option(){
            if(selector < n_battle_options - 1){
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

    private:
        int selector = 0;
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


class BattleSystem{
    public:
        BattleSystem(){
            Hero temp_hero;
            Monster temp_monster;
            player = temp_hero;
            monster = temp_monster;
            victory_count = 0;
            defeat_count = 0;
        }

        BattleSystem(Hero& player, Monster& enemy){
            this->player = player;
            this->monster = enemy;
            victory_count = 0;
            defeat_count = 0;
        }

        void player_attack(){
            cout << "PLAYER ATTACK" << endl;
            int damage = damage_formula(player.get_attk().get_value(), monster.get_def().get_value());
            cout << "Does " << damage << " points of damage" << endl;
            monster.set_hp(monster.get_hp().get_value() - damage);
        }

        void enemy_attack(){
            cout << "ENEMY ATTACK" << endl;
            int damage = damage_formula(monster.get_attk().get_value(), player.get_def().get_value());
            cout << "Does " << damage << " points of damage" << endl;
            player.set_hp(player.get_hp().get_value() - damage);
        }

        void player_heal(){
            cout << "PLAYER HEALS" << endl;
            player.set_hp(player.get_hp().get_value() + RECUPERATE_AMOUNT);
            cout << "Heals " << RECUPERATE_AMOUNT << " points of damage" << endl;
        }

        void display_characters(int x, int y, TextureText temp_texture, SDL_Color color){
            temp_texture.create_text_texture(player.repr(), color);
            temp_texture.render(x, y);
            temp_texture.create_text_texture(monster.repr(), color);
            temp_texture.render(x, y + temp_texture.h);
            
            y+=2*temp_texture.h;
        }

        void restore_characters(){
            player.set_hp(player.get_max_hp().get_value());
            monster.set_hp(monster.get_max_hp().get_value());
        }

        bool check_battle_end(){
            int HPs[] = {player.get_hp().get_value(), monster.get_hp().get_value()};
            int total = sizeof(HPs)/sizeof(HPs[0]);

            for(int i=0; i<total; i++){
                if (HPs[i] <= 0){
                    //display_characters();
                    if (i==ENEMY){
                        cout << "VICTORY" << endl;
                        victory_count++;
                        player.add_exp(monster.get_exp_reward());
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
            vector<Status> stats = player.get_stats();
            for (int i=0; i<stats.size();i++){
                cout << stats[i].get_name() << " - " << stats[i].get_value() << endl;
            }
        }

    private:
        Hero player;
        Monster monster;
        const int PLAYER = 0;
        const int ENEMY = 1;
        int victory_count;
        int defeat_count;
        const static int entity_number = 2;
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

    srand(time(0));
    Hero player;
    Monster enemy;
    player.init("Player", 10, 3, 1, 1);
    enemy.init("Enemy", 10, 3, 1, 1, 25);

    Window window("Test", 800, 600);
    BattleSystem battle(player, enemy);
    Menu menu;
    SDL_Color black_color = { 0, 0, 0 };
    Font normal_font("fonts/LiberationMono-Regular.ttf", 18);
    Font bold_font("fonts/LiberationMono-Bold.ttf", 18);

    TextureText text(window.get_render(), normal_font.font);
    TextureText textBold(window.get_render(), bold_font.font);

    const Uint8* currentKeyStates = NULL;

    bool key_lock_up = false;
    bool key_lock_down = false;
    bool key_lock_enter = false;

    while(window.check_exit()){

        window.clear_screen();

        battle.display_characters(0, 0, text, black_color);

        menu.display_battle_options(0, 300, text, textBold, black_color);

        // Get input
                
        currentKeyStates = SDL_GetKeyboardState( NULL );

        if( (currentKeyStates[ SDL_SCANCODE_RIGHT ]) && (!key_lock_up) ){
            menu.next_option();
            key_lock_up = true;
        }else if( (currentKeyStates[ SDL_SCANCODE_LEFT ]) && (!key_lock_down) ){
            menu.previews_option();
            key_lock_down = true;
        }

        if( (currentKeyStates[ SDL_SCANCODE_RETURN ]) && (!key_lock_enter) ){
            if(menu.get_selector()==menu.ATACK){
                battle.player_attack();
                battle.enemy_attack();
            }else if(menu.get_selector()==menu.HEAL){
                battle.player_heal();
                battle.enemy_attack();
            }else if (menu.get_selector()==menu.STATUS){
                battle.display_player_stats();
            }

            if (battle.check_battle_end()){
                battle.display_vitory_count();
                battle.restore_characters();
            }

            key_lock_enter = true;
        }

        if(( !currentKeyStates[ SDL_SCANCODE_RIGHT ] ) && (key_lock_up)){
            key_lock_up = false;
        }

        if(( !currentKeyStates[ SDL_SCANCODE_LEFT ] ) && (key_lock_down)){
            key_lock_down = false;
        }

        if(( !currentKeyStates[ SDL_SCANCODE_RETURN ] ) && (key_lock_enter)){
            key_lock_enter = false;
        }


        
        window.update_screen();
        /*
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
        */
    }
    cout << "GAME OVER" << endl;
    return 0;
}
