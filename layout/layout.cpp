#include "layout.h"

BattleSystem::BattleSystem(){
    srand(time(0));
    player = NULL;
    monster = NULL;
    victory_count = 0;
    defeat_count = 0;
}

BattleSystem::~BattleSystem(){
    player = NULL;
    monster = NULL;
    victory_count = 0;
    defeat_count = 0;
}

void BattleSystem::set_player(Hero& player){
    this->player = &player;
}

void BattleSystem::set_enemy(Monster& enemy){
    this->monster = &enemy;
}

string BattleSystem::player_attack(){
    int damage = damage_formula(player->get_attk().get_value(), monster->get_def().get_value());
    monster->set_hp(monster->get_hp().get_value() - damage);
    return "PLAYER ATTACK, Does " + to_string(damage) + " points of damage";
}

string BattleSystem::enemy_attack(){ 
    int damage = damage_formula(monster->get_attk().get_value(), player->get_def().get_value());
    player->set_hp(player->get_hp().get_value() - damage);
    return "ENEMY ATTACK, Does " + to_string(damage) + " points of damage";            
}

string BattleSystem::player_heal(){
    player->set_hp(player->get_hp().get_value() + RECUPERATE_AMOUNT);
    return "PLAYER HEALS, Heals " + to_string(RECUPERATE_AMOUNT) + " points of damage";
}

void BattleSystem::restore_characters(){
    player->set_hp(player->get_max_hp().get_value());
    monster->set_hp(monster->get_max_hp().get_value());
}

bool BattleSystem::check_battle_end(){
    enum entity_id{
        PLAYER,
        ENEMY
    };
    
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

int BattleSystem::damage_formula(int caller_attack, int target_defense){
    int extra = 1 + (rand () % 2);
    int damage = caller_attack + extra - target_defense;
    if (damage < 0){
        damage = 0;
    }
    return damage;
}


// Layouts


BattleLayout::BattleLayout(){
    currentKeyStates = NULL;
    layout_current   = NULL;
    controls_rules   = NULL;
    locks            = NULL;
    text             = NULL;
    selector = 0;
}

BattleLayout::~BattleLayout(){
    currentKeyStates = NULL;
    layout_current   = NULL;
    controls_rules   = NULL;
    locks            = NULL;
    text             = NULL;
    selector = 0;
}

BattleLayout::BattleLayout(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
    init(layout_current, controls_rules, locks, text_obj);
}

void BattleLayout::init(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
    currentKeyStates = SDL_GetKeyboardState(NULL);
    this->layout_current = &layout_current;
    this->controls_rules = &controls_rules;
    this->locks          = &locks;
    this->text           = text_obj;

    selector = 0;
}

void BattleLayout::next_option(){
    if(selector < TOTAL_ACTIONS - 1){
        selector++;
    }
}

void BattleLayout::previews_option(){
    if(selector > 0){
        selector--;
    }            
}

void BattleLayout::action(){
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

void BattleLayout::check_player_action(){
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

void BattleLayout::render(int x, int y){
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



StatsLayout::StatsLayout(){
    currentKeyStates = NULL;
    layout_current   = NULL;
    controls_rules   = NULL;
    locks            = NULL;
    text             = NULL;
    player           = NULL;
}

StatsLayout::~StatsLayout(){
    currentKeyStates = NULL;
    layout_current   = NULL;
    controls_rules   = NULL;
    locks            = NULL;
    text             = NULL;
    player           = NULL;
}

StatsLayout::StatsLayout(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
    init(layout_current, controls_rules, locks, text_obj);
}

void StatsLayout::init(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
    currentKeyStates = SDL_GetKeyboardState(NULL);
    this->layout_current = &layout_current;
    this->controls_rules = &controls_rules;
    this->locks          = &locks;
    this->text           = text_obj;
}

void StatsLayout::set_player(Hero &player){
    this->player = &player;
}

void StatsLayout::action(){
    *layout_current = LAYOUT_COMBAT;
}

void StatsLayout::check_player_action(){

    if( (currentKeyStates[controls_rules->action_button]) && (!locks->action_button) ){
        action();
        locks->action_button = true;
    }

    if(( !currentKeyStates[controls_rules->action_button] ) && (locks->action_button)){
        locks->action_button = false;
    }
}

void StatsLayout::render(int x, int y){
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




GameOverLayout::GameOverLayout(){
    currentKeyStates = NULL;
    layout_current   = NULL;
    controls_rules   = NULL;
    locks            = NULL;
    text             = NULL;

    exit = false;
    selector = 0;
}

GameOverLayout::~GameOverLayout(){
    currentKeyStates = NULL;
    layout_current   = NULL;
    controls_rules   = NULL;
    locks            = NULL;
    text             = NULL;
    
    exit = false;
    selector = 0;
}

GameOverLayout::GameOverLayout(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
    init(layout_current, controls_rules, locks, text_obj);
}

void GameOverLayout::init(int &layout_current, controls &controls_rules, controls_locks& locks, TextureText text_obj[]){
    currentKeyStates = SDL_GetKeyboardState(NULL);
    this->layout_current = &layout_current;
    this->controls_rules = &controls_rules;
    this->locks          = &locks;
    this->exit           = &exit;
    this->text           = text_obj;

    exit = false;
    selector = 0;
}

void GameOverLayout::next_option(){
    if(selector < TOTAL_ACTIONS - 1){
        selector++;
    }
}

void GameOverLayout::previews_option(){
    if(selector > 0){
        selector--;
    }            
}

bool GameOverLayout::check_exit(){
    return exit;
}

void GameOverLayout::action(){
    if(selector == YES){
        *layout_current = LAYOUT_COMBAT;
    }else if(selector == NO){
        exit = true;
    }
}

void GameOverLayout::check_player_action(){
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

void GameOverLayout::render(int x, int y){
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