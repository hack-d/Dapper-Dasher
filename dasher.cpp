#include"raylib.h"

struct AnimData{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

//ground check function
bool isOnGround(AnimData data,int windowHeight)
{
    return data.pos.y>=windowHeight-data.rec.height;
}

AnimData updateAnimdata(AnimData data,float deltaTime,int maxFrame)
{
    //update running time
    data.runningTime+=deltaTime;
    if(data.runningTime>=data.updateTime)
    {
        data.runningTime=0.0;
        //update animation frame
        data.rec.x=data.frame*data.rec.width;
        data.frame++;
        if(data.frame>maxFrame)
        {
            data.frame=0;
        }
    }
    return data;
}

int main()
{
    int windowDimensions[2]={512,380};

    bool collision=false;


    InitWindow(windowDimensions[0],windowDimensions[1],"dapper dasher");
    

    int velocity{0};

    //loading backgrounds
    Texture2D background=LoadTexture("textures/far-buildings.png");
    Texture2D midground=LoadTexture("textures/back-buildings.png");
    Texture2D foreground=LoadTexture("textures/foreground.png");
    float bgX{};
    float mgX{};
    float fgX{};

    const int gravity{1000};

    //nebula variables
    //AnimData for nebula
    Texture2D nebula=LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeofNebulae=9999;
    AnimData nebulae[sizeofNebulae]={};
    
    for(int i=0;i<sizeofNebulae;i++)
    {
        nebulae[i].rec.x=0;
        nebulae[i].rec.y=0;
        nebulae[i].rec.height=nebula.height/8;
        nebulae[i].rec.width=nebula.width/8;
        nebulae[i].pos.y=windowDimensions[1]-nebula.height/8;
        nebulae[i].frame=0;
        nebulae[i].runningTime=0.0;
        nebulae[i].updateTime=0.0;
        nebulae[i].pos.x=windowDimensions[0]+i*300;
    }
    
    float finishLine{nebulae[sizeofNebulae-1].pos.x};
    

    //nebula x velocity(pixels/sec)
    int nebVel{-200};

    //scarfy variables
    Texture2D scarfy=LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.height=scarfy.height;
    scarfyData.rec.width=scarfy.width/6;
    scarfyData.rec.x=0;
    scarfyData.rec.y=0;
    scarfyData.pos.x=windowDimensions[0]/2-scarfyData.rec.width/2;
    scarfyData.pos.y=windowDimensions[1]-scarfyData.rec.height;
    scarfyData.frame=0;
    scarfyData.runningTime=0;
    scarfyData.updateTime=1.0/12.0;


    //is in air
    bool isInAir{};
    int jumpVel{-600};



    //FPS setup
    SetTargetFPS(90);

    while(!WindowShouldClose())
    {
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        const float dT{GetFrameTime()};

        bgX-=20*dT;
        mgX-=40*dT;
        fgX-=80*dT;

        //scrolling the background
         if(bgX<=-background.width*2)
        {
            bgX=0.0;
        }

        if(mgX<=-midground.width*2)
        {
            mgX=0.0;
        }

        if(fgX<=-foreground.width*2)
        {
            fgX=0.0;
        }

        //draw the background
        Vector2 bg1Pos{bgX,0.0};
        DrawTextureEx(background,bg1Pos,0.0,2.0,WHITE);
        Vector2 bg2Pos{bgX+background.width*2,0.0};
        DrawTextureEx(background,bg2Pos,0.0,2.0,WHITE);
        
       
        //draw midground
        Vector2 mg1Pos{mgX,0.0};
        DrawTextureEx(midground,mg1Pos,0.0,2.0,WHITE);
        Vector2 mg2Pos{mgX+midground.width*2,0.0};
        DrawTextureEx(midground,mg2Pos,0.0,2.0,WHITE);

        
        //draw foreground
        Vector2 fg1Pos{mgX,0.0};
        DrawTextureEx(foreground,fg1Pos,0.0,2.0,WHITE);
        Vector2 fg2Pos{mgX+foreground.width*2,0.0};
        DrawTextureEx(foreground,fg2Pos,0.0,2.0,WHITE);

        
        //ground check
        if(isOnGround(scarfyData,windowDimensions[1]))
        {   
            //rectangle is on the ground
            velocity=0;
            isInAir=false;
        } 
        else
        {
            //applying gravity cuz rectangle is in the air
            velocity+=gravity*dT;
            isInAir=true;
        }

        if(IsKeyPressed(KEY_SPACE) && isInAir==false) 
        {
            velocity+=jumpVel;
        }
        
        //updating nebula position
        for(int i=0;i<sizeofNebulae;i++)
        {
            nebulae[i].pos.x += nebVel*dT;
        }

        //update the finishline
        finishLine+=nebVel*dT;
        

        //updating scarfy position 
        scarfyData.pos.y+=velocity*dT;

        /*nebula animation*/
        for(int i=0;i<sizeofNebulae;i++)
        {
           nebulae[i]=updateAnimdata(nebulae[i],dT,7);
        }



        //scarfy animation
        if(isInAir==false)
        {
           scarfyData=updateAnimdata(scarfyData,dT,5);  
        }

        
        for(AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x+pad,
                nebula.pos.y+pad,
                nebula.rec.height-2*pad,
                nebula.rec.width-2*pad
            };

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.height,
                scarfyData.rec.width
            };

            if(CheckCollisionRecs(nebRec,scarfyRec))
            {
                collision=true;
            }
        }

          if(collision)
            {
                //lose the game
                DrawText("GAME OVER",windowDimensions[0]/3.5,windowDimensions[1]/2,40,WHITE);
            }
            else if(scarfyData.pos.x>=finishLine)
            {
                //win the game
                 DrawText("YOU WIN",windowDimensions[0]/3.5,windowDimensions[1]/2,40,WHITE);
            }
            else
            {
                //drawing nebula
                for(int i=0;i<sizeofNebulae;i++)
                {
                    DrawTextureRec(nebula,nebulae[i].rec,nebulae[i].pos,WHITE);
                }

                //drawing scarfy
                DrawTextureRec(scarfy,scarfyData.rec,scarfyData.pos,WHITE);
            }


        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();

}