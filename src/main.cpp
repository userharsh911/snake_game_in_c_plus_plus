#include <raylib.h>
#include <iostream>
#include <deque>
using namespace std;

class Game{
    protected:
    int cell_size;
    int cell_count;
    public:
    Game() : cell_size(30), cell_count(25){}
    void initialize_window(){
        InitWindow(cell_size * cell_count, cell_size * cell_count, "Snake Game");
    }
    void draw_background(){
        Color color = {108, 227, 108, 1};
        ClearBackground(color);
    }
};

class Food : public Game{
    Color color;
    public:
    Vector2 pos;
    Texture2D texture;
    Food(){
        pos.x = GetRandomValue(0, cell_count - 1);
        pos.y = GetRandomValue(0, cell_count - 1);
        color = RED;
        Image image = LoadImage("./src/image/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }
    void draw(){
        // DrawRectangle(pos.x * cell_size, pos.y * cell_size, cell_size, cell_size, color);
        DrawTexture(texture, pos.x * cell_size, pos.y * cell_size, WHITE);
    }
    ~Food(){UnloadTexture(texture);}
};

class Snake : public Game{
    deque<Vector2> body;
    char axis;
    bool dir;
    bool prev_dir;
    int score;
    Color color;
    Texture2D texture;
    Texture2D body_texture;
    public:
    Snake(){
        Vector2 vec {(float)GetRandomValue(0, cell_count / 2), (float)GetRandomValue(0, cell_count - 1)};
        body.push_back(vec);
        dir = true;
        axis = 'x';
        prev_dir = true;
        color = BLACK;
        score = 0;
    }
    void print_score(){
        string msg = "Score: "+to_string(score);
        DrawText(&msg[0], 0, 0, 20, BLACK);
    }
    void eat(Food &food, Sound &snake_bite){
        if(axis == 'x' && body.front().x == food.pos.x && body.front().y == food.pos.y){
            PlaySound(snake_bite);
            score++;
            if(dir){
                Vector2 vec = {body.back().x + 1,body.back().y};
                body.push_back(vec);
            }else{
                Vector2 vec = {body.back().x - 1,body.back().y};
                body.push_back(vec);
            }
            food.pos.x = GetRandomValue(0, cell_count - 1);
            food.pos.y = GetRandomValue(0, cell_count - 1);
        }else if(axis == 'y' && body.front().x == food.pos.x && body.front().y == food.pos.y){
            PlaySound(snake_bite);
            score++;
            if(dir){
                Vector2 vec = {body.back().x,body.back().y - 1};
                body.push_back(vec);
            }else{
                Vector2 vec = {body.back().x,body.back().y + 1};
                body.push_back(vec);
            }
            food.pos.x = GetRandomValue(0, cell_count - 1);
            food.pos.y = GetRandomValue(0, cell_count - 1);
        }
    }
    void draw(){
        Image image = LoadImage("./src/image/snake_head.png");
        Image body_image = LoadImage("./src/image/snake_body.png");
        if(axis == 'x' && dir) ImageRotate(&image, -90);
        else if(axis == 'x' && !dir) ImageRotate(&image, 90);
        else if(axis == 'y' && !dir) ImageRotate(&image, 180);
        texture = LoadTextureFromImage(image);
        body_texture = LoadTextureFromImage(body_image);
        UnloadImage(image);
        UnloadImage(body_image);
        DrawTexture(texture, body.front().x * cell_size, body.front().y * cell_size,WHITE);
        for(unsigned int i = 1; i < body.size(); i++){
            DrawTexture(body_texture, body[i].x * cell_size, body[i].y * cell_size,WHITE);
        }
    }
    ~Snake(){
        UnloadTexture(texture);
        UnloadTexture(body_texture);
    }
    void game_lost(Sound &game_over, Music &print_score){
        StopMusicStream(print_score);
        PlaySound(game_over);
        Vector2 vec;
        if(axis == 'x' && dir) vec = {body.back().x - 1, body.back().y};
        else if(axis == 'x' && !dir) vec = {body.back().x + 1, body.back().y};
        else if(axis == 'y' && dir) vec = {body.back().x, body.back().y - 1};
        else if(axis == 'y' && !dir) vec = {body.back().x, body.back().y + 1};
        body.push_back(vec);
        body.pop_front();
        axis = 'p';
    }
    void move(){
        if(axis == 'x'){
            if(body.front().x < 0) body.front().x = cell_count;
            else if(body.back().x >= cell_count) body.back().x = 0;
            if(prev_dir){
                float pos_x = body.front().x + 1;
                if(!dir) pos_x -= 2;
                Vector2 vec = {pos_x,body.front().y};
                body.pop_back();
                body.push_front(vec);
            }else{
                float pos_x = body.front().x - 1;
                if(dir) pos_x += 2;
                Vector2 vec = {pos_x,body.front().y};
                body.pop_back();
                body.push_front(vec);
            }
        }else if(axis == 'y'){
            if(body.front().y < 0) body.front().y = cell_count;
            else if(body.back().y >= cell_count) body.back().y = 0;
            if(prev_dir){
                float pos_y = body.front().y + 1;
                if(!dir) pos_y -= 2;
                Vector2 vec = {body.front().x, pos_y};
                body.pop_back();
                body.push_front(vec);
            }else{
                float pos_y = body.front().y - 1;
                if(dir) pos_y += 2;
                Vector2 vec = {body.front().x, pos_y};
                body.pop_back();
                body.push_front(vec);

            }
        }
    }
    void input_key(){
        if(axis == 'x' && IsKeyPressed(KEY_W)){
            axis = 'y';
            prev_dir = dir;
            dir = false;
        }else if(axis == 'x' && IsKeyPressed(KEY_S)){
            axis = 'y';
            prev_dir = dir;
            dir = true;
        }else if(axis == 'y' && IsKeyPressed(KEY_A)){
            axis = 'x';
            prev_dir = dir;
            dir = false;
        }else if(axis == 'y' && IsKeyPressed(KEY_D)){
            axis = 'x';
            prev_dir = dir;
            dir = true;
        }
    }
    void walls_collision(Sound &game_over, Music &print_score){
        if(body.front().x < 0 || body.front().y < 0 || body.front().x == cell_count || body.front().y == cell_count){
            game_lost(game_over, print_score);
        }
    }
    void self_eat(Sound &game_over, Music &print_score){
        Vector2 head = body.front();
        for(unsigned int i = 1; i < body.size(); i++){
            if(body[i].x == head.x && body[i].y == head.y){
                game_lost(game_over, print_score);
                return;
            }
        }
    }
    void reset(){
        if(IsKeyPressed(KEY_R) && axis == 'p'){
            Vector2 vec {(float)GetRandomValue(0, cell_count / 2), (float)GetRandomValue(0, cell_count - 1)};
            body.clear();
            body.push_back(vec);
            axis = 'x';
            prev_dir = true;
            dir = true;
            score = 0;
        }
    } 
};

int main(){
    Game game;
    game.initialize_window();
    InitAudioDevice();
    Snake snake;
    Food food;
    Sound snake_bite = LoadSound("./src/audio/snake_bite.wav");
    Sound game_over = LoadSound("./src/audio/game_over.mp3");
    Music bg = LoadMusicStream("./src/audio/bg.mp3");
    float pan = 0.0f;
    SetMusicPan(bg, pan);

    float volume = 0.4f;
    SetMusicVolume(bg,volume);
    
    PlayMusicStream(bg);
    
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        WaitTime(0.07);
        UpdateMusicStream(bg);
        snake.input_key();
        snake.move();
        snake.walls_collision(game_over, bg);
        snake.self_eat(game_over, bg);

        if(IsKeyPressed(KEY_R)) StopSound(game_over), PlayMusicStream(bg);

        snake.eat(food, snake_bite);
        snake.reset();
        BeginDrawing();
        
        game.draw_background();
        food.draw();
        snake.draw();
        snake.print_score();

        EndDrawing();
    }
    UnloadSound(snake_bite);
    UnloadMusicStream(bg);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}