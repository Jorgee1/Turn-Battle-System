#ifndef COMMON_H
#define COMMON_H


struct controls{
    int action_button;
    int cancel_button;
    int start_button;
    int move_up;
    int move_down;
    int move_left;
    int move_right;
};

struct controls_locks{
    bool action_button = true;
    bool cancel_button = true;
    bool start_button  = true;
    bool move_up       = true;
    bool move_down     = true;
    bool move_left     = true;
    bool move_right    = true;
};

const enum text_keys{
    TEXT_BLACK,
    TEXT_RED,
    TEXT_GREEN,
    TEXT_BLUE,
    TEXT_WHITE,
    TOTAL_TEXT,
};

const enum LAYOUT_ID{
    LAYOUT_COMBAT,
    LAYOUT_STATUS,
    LAYOUT_GAME_OVER,
    TOTAL_LAYOUT
};


#endif