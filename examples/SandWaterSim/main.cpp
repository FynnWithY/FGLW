#include<FGLW/FGLW.hpp>
#include "Materials.hpp"
#include "Shaders.hpp"
#include "Texture.hpp"
#include<time.h>

int main()
{
    FGLW window(FGLW_RESOLUTION_W,FGLW_RESOLUTION_H,"Water and Sand");
    window.MakeContextCurrent();
    FGLW::GLInit();
    uint32 vao, vbo;
    loadShader();
    glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float vertices[] =
        {
            -1, -1, 0, 0, 0,
            -1, 1, 0, 0, 1,
            1, 1, 0, 1, 1,
//
            1, 1, 0, 1, 1,
            1, -1, 0, 1, 0,
            -1, -1, 0, 0, 0};
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glCreateVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(float) * 5, NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, sizeof(float) * 5, (void *)(sizeof(float) * 3));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    uint32 screen[WIDTH*HEIGHT];
    MaterialType mat[WIDTH*HEIGHT]={(MaterialType)0};
    Texture texture(WIDTH,HEIGHT,screen);
    for(int i=0;i<WIDTH*HEIGHT;i++)
    {
        mat[i]=MAT_AIR;
        screen[i]=Material<MAT_AIR>::color;
    }
    texture.UpdateTexture();
    int running=1;
    mat[WIDTH/2+(HEIGHT-1)*WIDTH]=MAT_SAND;
    while(running)
    {
        mat[WIDTH/2+(HEIGHT-1)*WIDTH]=MAT_SAND;
        Sleep(10);
        for(int x=0;x<WIDTH;x++)
        {
            for(int y=0;y<HEIGHT;y++)
            {
                switch((MaterialType)mat[x+y*WIDTH])
                {
                    case MAT_SAND:
                        Material<MAT_SAND>::Update((MaterialType*)mat,x,y);
                        break;
                    case MAT_AIR:
                        break;
                }
            }
        }   
        for(int x=0;x<WIDTH;x++)
        {
            for(int y=0;y<HEIGHT;y++)
            {
                switch((MaterialType)mat[x+y*WIDTH])
                {
                    case MAT_SAND:
                        screen[x+y*WIDTH]=Material<MAT_SAND>::color;
                        break;
                    case MAT_AIR:
                        screen[x+y*WIDTH]=Material<MAT_AIR>::color;
                        break;
                }
            }
        }
        texture.UpdateTexture();
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        texture.Uniform(shaderProgram,"text");
        glDrawArrays(GL_TRIANGLES, 0, 6);
        running=window.PollEvents();
        window.SwapBuffers();
    }
}