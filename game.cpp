#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>
#include <time.h>  
#include <tuple>
#include <algorithm>

// #pragma once
using namespace std;

const int game_width = 640;
const int game_height = 480;

class G {

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_bool done;
    vector<SDL_FPoint> points;
    vector<SDL_Color> colors;
    SDL_Event event;
    public:
    G() 
    {
       SDL_Init(SDL_INIT_VIDEO); 
       SDL_CreateWindowAndRenderer(640*4, 480*4, SDL_WINDOW_ALLOW_HIGHDPI, &window, &renderer);
       SDL_RenderSetScale(renderer,4,4);
    }

    void drawpixel(double xm, double ym, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255) 
    {
        points.emplace_back(xm,ym);
        colors.emplace_back(r,g,b,a);
    }
    void clearpixels()
    {
        points.clear();
    }

    void update() 
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        
        for(long unsigned int i = 0; i < points.size(); i++)
        {
            SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
            SDL_RenderDrawPointF(renderer, points[i].x,points[i].y);
        }


        SDL_RenderPresent(renderer);
    }
    void input() {
        while( SDL_PollEvent( &event ) )
        {  
            switch( event.type )
            {
                /* SDL_QUIT event (window close) */
                case SDL_QUIT:
                    SDL_Quit();
                    exit(0);
                    break;

                default:
                    break;
            }
        }
    }

};

bool isAlive(vector<vector<int>> &cnvs, int x, int y)
{
    // total active neighborng orgs
    int alive = 0;
    // check left
    if(x>0 && cnvs[x-1][y] == 1) alive+=1;
    // check right
    if(x<game_width && cnvs[x+1][y] == 1) alive += 1;
    // check front
    if(y>0 && cnvs[x][y-1] == 1) alive += 1;
    // check bottom
    if(y<game_height && cnvs[x][y+1] == 1) alive += 1;
    
    //check corners
    if(y>0 && x>0 && cnvs[x-1][y-1] == 1) alive += 1;
    if(y>0 && x<game_width && cnvs[x+1][y-1] == 1) alive += 1;
    if(y<game_height && x>0 && cnvs[x-1][y+1] == 1) alive += 1;
    if(y<game_height && x<game_width && cnvs[x+1][y+1] == 1) alive += 1;

    if(alive == 3)
        return true;
    else if(alive > 3)
        return false;
    else if(alive < 2)
        return false;
    
    return cnvs[x][y];
}

int main(int agrc, char* agrv[])
{
    G screen;
    vector<vector<int>> display(game_height,vector<int> (game_width));
    vector<vector<int>> swap(game_height,vector<int> (game_width));

    //
    for(auto &row : display)
    {
        std::generate(row.begin(),row.end(), [](){return rand()%10==0;});
    }

    while(true)
    {
        for(int i=0; i<game_width; ++i)
        {
            for(int j=0; j<game_height; ++j)
            {
                swap[i][j] = isAlive(display,i,j);
            }
        }

        for(int i=0; i<game_width; ++i)
        {
            for(int j=0; j<game_height; ++j)
            {
                if(swap[i][j])
                {
                    screen.drawpixel(i,j);
                }
            }
        }

        std::copy(swap.begin(),swap.end(),display.begin());

        // flashing screen
        screen.update();
        SDL_Delay(20);
        screen.input();
        screen.clearpixels();
    }
}