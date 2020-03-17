#include <stdio.h>
#include <time.h>
#include<windows.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define GAME_TERMINATE -1
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

bool* keys;

// ALLEGRO Variables
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT_QUEUE *Bullet_Event=NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_BITMAP *ching = NULL;
ALLEGRO_BITMAP *Ctrl = NULL;
ALLEGRO_BITMAP *movePic = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *menu = NULL;
ALLEGRO_BITMAP *chinese = NULL;
ALLEGRO_KEYBOARD_STATE keyState ;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_TIMER *timer2 = NULL;
ALLEGRO_TIMER *timer3 = NULL;
ALLEGRO_TIMER *ESC_Timer = NULL;
ALLEGRO_SAMPLE *song=NULL;
ALLEGRO_SAMPLE *song1=NULL;
ALLEGRO_SAMPLE *song2=NULL;
ALLEGRO_SAMPLE *song3=NULL;
ALLEGRO_SAMPLE *song4=NULL;
ALLEGRO_SAMPLE *song5=NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_EVENT event;
ALLEGRO_EVENT Bevent;
//Custom Definition
const char *title = "Final Project 106034071";
const float FPS = 60;
const int WIDTH = 400;
const int HEIGHT = 600;
bool direction[4]={0};


typedef struct character
{
    int x;
    int y;
    int life;
    bool show_bullet;
    ALLEGRO_BITMAP *image_path;
    ALLEGRO_TIMER *bullet_timer;

}Character;

Character MyAircraft;
Character TeemoLeft;
Character character3;
Character bullet[20];
Character Bching[9];

int imageWidth = 0;
int imageHeight = 0;
int draw = 0;
int done = 0;
int window = 1;
int judge_next_window = 0;
int Bx;
int maxLife=300;
int mylife=6;
double By;
bool ture = true; //true: appear, false: disappear
bool next = false; //true: trigger
bool dir = true; //true: left, false: right
bool attack=false;
bool showESC=false;
bool bullet_move=false;
bool first=true;

void show_err_msg(int msg);
void game_init();
void game_begin();
int process_event();
int game_run();
int B_Process();
void game_destroy();
void shooting();
void showLife();

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int msg = 0;
    TeemoLeft.life=maxLife;
    MyAircraft.life=mylife;
    game_init();
    game_begin();

    while (msg != GAME_TERMINATE)
    {
        msg = game_run();
        if (msg == GAME_TERMINATE)
            printf("Game Over\n");
    }

    game_destroy();
    return 0;
}

void show_err_msg(int msg)
{
    fprintf(stderr, "unexpected msg: %d\n", msg);
    game_destroy();
    exit(9);
}

void game_init()
{
    if (!al_init())
    {
        show_err_msg(-1);
    }
    if(!al_install_audio())
    {
        fprintf(stderr, "failed to initialize audio!\n");
        show_err_msg(-2);
    }
    if(!al_init_acodec_addon())
    {
        fprintf(stderr, "failed to initialize audio codecs!\n");
        show_err_msg(-3);
    }
    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "failed to reserve samples!\n");
        show_err_msg(-4);
    }
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    event_queue = al_create_event_queue();
    if (display == NULL || event_queue == NULL)
    {
        show_err_msg(-5);
    }
    // Initialize Allegro settings
    al_set_window_position(display, 0,0 );
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_install_keyboard();
    //al_install_mouse();
    al_install_audio();
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    //al_register_event_source(event_queue, al_get_mouse_event_source() );

}

void game_begin() {
    // Load sound
    song = al_load_sample( "hello.wav" );
    song1 = al_load_sample( "9701.wav" );
    song2 = al_load_sample( "gaming.wav" );
    song3 = al_load_sample( "hit.wav" );
    song4 = al_load_sample( "win.wav" );

    if (!song)
    {
        printf( "Audio clip sample not loaded!\n" );
        show_err_msg(-6);
    }
    // Loop the song until the display closes
    al_play_sample(song, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    al_clear_to_color(al_map_rgb(218,112,214));
    // Load and draw text
    font = al_load_ttf_font("pirulen.ttf",12,0);


    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+120 , ALLEGRO_ALIGN_CENTRE, "1. Start");
    al_draw_rectangle(WIDTH/2-150, 410, WIDTH/2+150, 450, al_map_rgb(255, 255, 255), 0);

    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+170 , ALLEGRO_ALIGN_CENTRE, "2. About");
    al_draw_rectangle(WIDTH/2-150, 460, WIDTH/2+150, 500, al_map_rgb(255, 255, 255), 0);

    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220 , ALLEGRO_ALIGN_CENTRE, "3. Exit");
    al_draw_rectangle(WIDTH/2-150, 510, WIDTH/2+150, 550, al_map_rgb(255, 255, 255), 0);

    menu = al_load_bitmap("menu.png");
    al_draw_bitmap(menu, WIDTH/2-82, HEIGHT/2-150, 0);

    chinese = al_load_bitmap("ch.png");
    al_draw_bitmap(chinese, WIDTH/2-76, HEIGHT/2-210, 0);

    al_flip_display();
}

int process_event()
{
    // Request the event

    al_wait_for_event(event_queue, &event);

    // Our setting for controlling animation
    if(event.timer.source == timer)
    {
        if(TeemoLeft.x < -150) dir = false;
        else if(TeemoLeft.x > WIDTH+50) dir = true;

        if(dir) TeemoLeft.x -= 5;
        else TeemoLeft.x += 5;
    }
    if(event.timer.source == timer2)
    {
        ture = false;
        next = true;
    }
    if(event.timer.source == timer3)
    {
        if(next) next = false;
        else ture = true;
    }

    Bching[0].x -= 1;
    Bching[0].y += 3;
    //Bching[1].x += 3;
    Bching[1].y += 3;
    Bching[2].x += 1;
    Bching[2].y += 3;
    Bching[3].x -= 1;
    Bching[3].y += 3;
    //Bching[4].x += 3;
    Bching[4].y += 3;
    Bching[5].x += 1;
    Bching[5].y += 3;
    for(int i=0;i<20;i++)
    {
        bullet[i].y -= 5;
    }
    int flag=1;
    // Keyboard
    if(event.type == ALLEGRO_EVENT_KEY_DOWN)
    {

        switch(event.keyboard.keycode)
        {
            // Control
            case ALLEGRO_KEY_UP:
                direction[0] = true;
                break;
            case ALLEGRO_KEY_DOWN:
                direction[1] = true;
                break;
            case ALLEGRO_KEY_LEFT:
                direction[2] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                direction[3] = true;
                break;
            case ALLEGRO_KEY_LCTRL:
                attack = true;
                break;
            // For Start Menu

        }
    }
    else if(event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(event.keyboard.keycode)
        {

            // Control
            case ALLEGRO_KEY_UP:
                direction[0] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                direction[1] = false;
                break;
            case ALLEGRO_KEY_LEFT:
                direction[2] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                direction[3] = false;
                break;

            case ALLEGRO_KEY_ESCAPE:
                judge_next_window = -1;
                break;
            case ALLEGRO_KEY_1:
                judge_next_window = 1;
                break;
            case ALLEGRO_KEY_2:
                judge_next_window = 2;
                break;
            case ALLEGRO_KEY_3:
                judge_next_window = 3;
                break;
            case ALLEGRO_KEY_LCTRL:
                attack = false;
                break;
            case ALLEGRO_KEY_ENTER:
                judge_next_window = 4;
                break;

        }
    }
    // Shutdown our program
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;

    if(MyAircraft.y>HEIGHT/2)  MyAircraft.y -= direction[0]*5;
    if(MyAircraft.y<HEIGHT-162)MyAircraft.y += direction[1]*5;
    if(MyAircraft.x>0)  MyAircraft.x -= direction[2]*5;
    if(MyAircraft.x<WIDTH-100)MyAircraft.x += direction[3]*5;

    for(int i=0;i<20;i++)
    {
        if(bullet[i].y<0 && event.timer.source==bullet[i].bullet_timer && attack)
        {
            bullet[i].x = MyAircraft.x+35;
            bullet[i].y = MyAircraft.y;
        }
    }
    int x = rand()%10;
    for(int i=0;i<9;i++)
    {
        if(Bching[i].y>600 && (x==3 || x== 5 || x==7))
        {
            Bching[i].x = TeemoLeft.x;
            Bching[i].y = TeemoLeft.y;
        }
    }
    return 0;
}

int game_run()
{
    int error = 0;
    // First window(Menu)
    if(window == 1)
    {
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
            if(judge_next_window==-1)
            {
                al_clear_to_color(al_map_rgb(218,112,214));
                // Load and draw text

                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+120 , ALLEGRO_ALIGN_CENTRE, "1. Start");
                al_draw_rectangle(WIDTH/2-150, 410, WIDTH/2+150, 450, al_map_rgb(255, 255, 255), 0);

                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+170 , ALLEGRO_ALIGN_CENTRE, "2. About");
                al_draw_rectangle(WIDTH/2-150, 460, WIDTH/2+150, 500, al_map_rgb(255, 255, 255), 0);

                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220 , ALLEGRO_ALIGN_CENTRE, "3. Exit");
                al_draw_rectangle(WIDTH/2-150, 510, WIDTH/2+150, 550, al_map_rgb(255, 255, 255), 0);
                menu = al_load_bitmap("menu.png");
                al_draw_bitmap(menu, WIDTH/2-82, HEIGHT/2-150, 0);
                chinese = al_load_bitmap("ch.png");
                al_draw_bitmap(chinese, WIDTH/2-76, HEIGHT/2-210, 0);
                al_flip_display();
            }
            else if(judge_next_window==1)
            {
                for(int i=0;i<20;i++)
                {
                    al_clear_to_color(al_map_rgb(0,0,0));
                    al_destroy_sample(song);
                    if(i%3==0)al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 , ALLEGRO_ALIGN_CENTRE, "LOADING.");
                    if(i%3==1)al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 , ALLEGRO_ALIGN_CENTRE, "LOADING..");
                    if(i%3==2)al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 , ALLEGRO_ALIGN_CENTRE, "LOADING...");
                    bullet[i].bullet_timer = al_create_timer(1);
                    al_register_event_source(event_queue, al_get_timer_event_source(bullet[i].bullet_timer)) ;
                    al_start_timer(bullet[i].bullet_timer);
                    bullet[i].x = MyAircraft.x+35;
                    bullet[i].y = MyAircraft.y;
                    Sleep(300);
                    al_flip_display();

                }

                window = 2;
                // Setting Character
                MyAircraft.x = WIDTH / 2;
                MyAircraft.y = HEIGHT / 2 + 150;
                TeemoLeft.x = WIDTH/2;
                TeemoLeft.y = HEIGHT / 2 - 280;
                MyAircraft.image_path = al_load_bitmap("tower.png");
                TeemoLeft.image_path = al_load_bitmap("teemo_left.png");
                character3.image_path = al_load_bitmap("teemo_right.png");
                background = al_load_bitmap("stage.jpg");
                ching = al_load_bitmap("ching.png");
                for(int i=0;i<9;i++)
                {
                    Bching[i].bullet_timer = al_create_timer(0.5);
                    al_register_event_source(event_queue, al_get_timer_event_source(Bching[i].bullet_timer)) ;
                    al_start_timer(Bching[i].bullet_timer);
                    Bching[i].x = TeemoLeft.x+41;
                    Bching[i].y = TeemoLeft.y+100;
                    Bching[i].image_path = al_load_bitmap("Bching.png");
                }
                //Initialize Timer
                timer  = al_create_timer(1.0/FPS);
                timer2  = al_create_timer(FPS);
                timer3  = al_create_timer(1.0/FPS);
                al_register_event_source(event_queue, al_get_timer_event_source(timer)) ;
                al_register_event_source(event_queue, al_get_timer_event_source(timer2)) ;
                al_register_event_source(event_queue, al_get_timer_event_source(timer3)) ;
                al_start_timer(timer);
                al_start_timer(timer2);
                al_start_timer(timer3);
                al_play_sample(song2, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
            }
            else if(judge_next_window==2)
            {
                al_clear_to_color(al_map_rgb(218,112,214));
                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-160 , ALLEGRO_ALIGN_CENTRE, "Author : 106034071");
                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-140 , ALLEGRO_ALIGN_CENTRE, "Version : 1.0");
                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-120 , ALLEGRO_ALIGN_CENTRE, "Move : ");
                movePic = al_load_bitmap("move.jpg");
                Ctrl = al_load_bitmap("Ctrl.jpg");
                al_draw_bitmap(movePic, WIDTH/2-88, HEIGHT/2-100, 0);
                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+25 , ALLEGRO_ALIGN_CENTRE, "Attack : ");
                al_draw_bitmap(Ctrl, WIDTH/2-50, HEIGHT/2+45, 0);
                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+180 , ALLEGRO_ALIGN_CENTRE, "Press ESC to Return");
                if (!al_is_event_queue_empty(event_queue))
                {
                    error = process_event();
                }
                al_flip_display();
            }
            else if(judge_next_window==3)
            {
                error = GAME_TERMINATE;
            }
        }
    }
    // Second window(Main Game)
    else if(window == 2)
    {
        if(judge_next_window==-1)
        {
            window=3;
            al_destroy_sample(song);
        }
        else
        {
            // Change Image for animation
            al_draw_bitmap(background, 0,0, 0);
            if(ture) al_draw_bitmap(MyAircraft.image_path, MyAircraft.x, MyAircraft.y, 0);

            if(dir) al_draw_bitmap(TeemoLeft.image_path, TeemoLeft.x, TeemoLeft.y, 0);
            else al_draw_bitmap(character3.image_path, TeemoLeft.x, TeemoLeft.y, 0);

            showLife();
            for(int i=0;i<3;i++)
            {
                if((Bching[i].x>=MyAircraft.x-50 && Bching[i].x<=MyAircraft.x+50) && (Bching[i].y>=MyAircraft.y && Bching[i].y<=MyAircraft.y+100))
                {
                    MyAircraft.life--;
                    Bching[i].x = TeemoLeft.x;
                    Bching[i].y = TeemoLeft.y;
                }
                else
                {
                    al_draw_bitmap(Bching[i].image_path, Bching[i].x, Bching[i].y, 0);
                }
            }
            for(int i=0;i<20;i++)
            {
                if((bullet[i].x>=TeemoLeft.x-41 && bullet[i].x<=TeemoLeft.x+41) && (bullet[i].y>=TeemoLeft.y && bullet[i].y<=TeemoLeft.y+96))
                {
                    TeemoLeft.life-=2;
                    bullet[i].x = MyAircraft.x+35;
                    bullet[i].y = MyAircraft.y;
                }
                else
                {
                    al_draw_bitmap(ching, bullet[i].x, bullet[i].y, 0);
                }
            }
            al_flip_display();
            if(TeemoLeft.life<=0 || MyAircraft.life<=0) window=4;
            al_clear_to_color(al_map_rgb(0,0,0));
            if (!al_is_event_queue_empty(event_queue))
            {
                error = process_event();
            }
            // Listening for new event
        }

    }
    else if(window==3)
    {
        if(judge_next_window==-1)
        {
            al_clear_to_color(al_map_rgb(218,112,214));
            al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-50 , ALLEGRO_ALIGN_CENTRE, "1. Continue");
            al_draw_rectangle(WIDTH/2-150, HEIGHT/2-60, WIDTH/2+150, HEIGHT/2-20, al_map_rgb(255, 255, 255), 0);

            al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 , ALLEGRO_ALIGN_CENTRE, "2. Restart");
            al_draw_rectangle(WIDTH/2-150, HEIGHT/2+40, WIDTH/2+150, HEIGHT/2+80, al_map_rgb(255, 255, 255), 0);

            al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+50 , ALLEGRO_ALIGN_CENTRE, "3. Exit");
            al_draw_rectangle(WIDTH/2-150, HEIGHT/2-10, WIDTH/2+150, HEIGHT/2+30, al_map_rgb(255, 255, 255), 0);

            al_flip_display();
            if (!al_is_event_queue_empty(event_queue))
            {
                error = process_event();
            }
        }
        else if(judge_next_window==1)
        {
            window = 2;
            song = al_load_sample( "hello.wav" );
            al_play_sample(song, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
        }
        else if(judge_next_window==2)
        {
            al_destroy_sample(song1);
            window=1;
            judge_next_window=-1;
            TeemoLeft.life=maxLife;
            al_destroy_event_queue(event_queue);
            al_destroy_display(display);
            al_destroy_timer(timer);
            al_destroy_timer(timer2);
            al_destroy_bitmap(image);
            al_destroy_sample(song);
            al_destroy_sample(song1);
            game_init();
            game_begin();
        }
        else if(judge_next_window==3)
        {
            error = GAME_TERMINATE;
        }

    }
    else if(window==4)
    {
        al_destroy_sample(song2);

        //al_play_sample(song1, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        al_clear_to_color(al_map_rgb(0,0,0));
        if(TeemoLeft.life<=0)
        {
            al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 , ALLEGRO_ALIGN_CENTRE, "You Win");
            al_play_sample(song4, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        }
        else if(MyAircraft.life<=0)
        {
            al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 , ALLEGRO_ALIGN_CENTRE, "You Lose");
            al_play_sample(song1, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        }
        al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+180 , ALLEGRO_ALIGN_CENTRE, "Press ESC to Close");
        al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+200 , ALLEGRO_ALIGN_CENTRE, "or");
        al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220 , ALLEGRO_ALIGN_CENTRE, "Press Enter to Restart");

        al_flip_display();

        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
        }

        if(judge_next_window==-1) error = GAME_TERMINATE;
        else if(judge_next_window==4)
        {
            al_destroy_sample(song2);
            al_destroy_sample(song4);
            window=1;
            judge_next_window=-1;
            MyAircraft.life=mylife;
            TeemoLeft.life=maxLife;
            al_destroy_event_queue(event_queue);
            al_destroy_display(display);
            al_destroy_timer(timer);
            al_destroy_timer(timer2);
            al_destroy_bitmap(image);
            al_destroy_sample(song);
            al_destroy_sample(song1);
            game_init();
            game_begin();
        }
    }
    return error;
}



void showLife()
{
    int i;
    for(i=1;i<=TeemoLeft.life;i++)
    {
        al_draw_filled_rectangle(50+1*i, 5, 50+1*(i+1), 20, al_map_rgb(46, 139, 87));
    }
    for(i=1;i<=MyAircraft.life;i++)
    {
        al_draw_filled_rectangle(50+40*i, 580, 50+40*(i+1), 595, al_map_rgb(178, 34, 34));
    }
}

void game_destroy()
{
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_timer(timer2);
    al_destroy_bitmap(image);
    al_destroy_sample(song);
    al_destroy_sample(song1);
}
