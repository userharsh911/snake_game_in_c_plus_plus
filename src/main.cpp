#include <raylib.h>
#include <iostream>
#include <deque>
#include <vector>
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
        ClearBackground(LIGHTGRAY);
    }
};

class Food : public Game{
    public:
    Vector2 pos;
    Food(){
        pos.x = GetRandomValue(0, cell_count - 1);
        pos.y = GetRandomValue(0, cell_count - 1);
    }
    void draw(){
        DrawRectangle(pos.x * cell_size, pos.y * cell_size, cell_size, cell_size, RED);
    }
};

class Snake : public Game{
    deque<Vector2> body;
    char axis;
    bool dir;
    bool prev_dir;
    public:

    Snake(){
        Vector2 vec {(float)GetRandomValue(0, cell_count / 2), (float)GetRandomValue(0, cell_count - 1)};
        body.push_back(vec);
        dir = true;
        axis = 'x';
        prev_dir = true; 
    }
    void add(Food &food){
        if(axis == 'x' && body.front().x == food.pos.x && body.front().y == food.pos.y){
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
            cout<<"equal y"<<endl;
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
        for(auto vec : body){
            DrawRectangle(vec.x * cell_size, vec.y* cell_size, cell_size, cell_size, BLACK);
        }
    }
    void move(){
        if(axis == 'x'){
            if(body.front().x < 0) body.front().x = cell_count;
            else if(body.back().x >= cell_count) body.back().x = 0;
            if(prev_dir){
                cout<<body.front().x<<endl;
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
    void walls_collision(){
        if(body.front().x < 0 || body.front().y < 0 || body.front().x == cell_count || body.front().y == cell_count){
            axis = 'r';
            if(body.front().x < 0) body.front().x = 0; 
            if(body.front().y < 0) body.front().y = 0; 
            if(body.front().x == cell_count) body.front().x = cell_count - 1;
            if(body.front().y == cell_count) body.front().y = cell_count - 1;
        }
    }
    void reset(){
        if(IsKeyPressed(KEY_R)){
            Vector2 vec {(float)GetRandomValue(0, cell_count - 1), (float)GetRandomValue(0, cell_count - 1)};
            body.clear();
            body.push_back(vec);
            axis = 'x';
            prev_dir = true;
        }
    } 
};

int main(){
    Game game;
    game.initialize_window();

    Snake snake;
    Food food;
    SetTargetFPS(60);
    
    
    while(!WindowShouldClose()){
        WaitTime(0.07);
        snake.input_key();
        snake.move();
        snake.walls_collision();
        
        snake.add(food);
        snake.reset();
        BeginDrawing();
        
        game.draw_background();
        food.draw();
        snake.draw();


        EndDrawing();
    }

    CloseWindow();
    return 0;
}