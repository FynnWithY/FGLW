#include <FGLW/FGLW.hpp>
#include "Materials.hpp"
#include "Shaders.hpp"
#include "Texture.hpp"
#include <time.h>

struct InputState
{
    double x, y;

    int width, height;
    int buttons[3];
};
int main()
{
    InputState input;
    input.x = 0;
    input.y = 0;
    input.width = FGLW_RESOLUTION_W;
    input.height = FGLW_RESOLUTION_H;
    FGLW::FGLW window(1, 1, FGLW_RESOLUTION_W, FGLW_RESOLUTION_H, "Water and Sand");
    window.MakeContextCurrent();
    FGLW::FGLW::GLInit();
    window.SetUserData(&input);
    window.SetEventCallbackFunction([](FGLW::FGLW *windowptr, FGLW::EventType type, FGLW::voidptr eventData)
                                    {
        FGLW::FGLW& window=*windowptr;
        InputState& state=*(InputState*) window.GetUserData();
        switch(type)
        {
            case FGLW::MOUSE_MOVE:
                {FGLW::MouseMoveEvent& e=*(FGLW::MouseMoveEvent*)eventData;
                state.x=(float)e.x/(float)state.width;
                state.y=1-(float)e.y/(float)state.height;
                break;}
            case FGLW::WINDOW_RESIZE:{
                FGLW::WindowResizeEvent& e=*(FGLW::WindowResizeEvent*)eventData;
                glViewport(0,0,e.width,e.height);
                state.width=e.width;
                state.height=e.height;
                break;}
            case FGLW::MOUSE_BUTTON:{
                FGLW::MouseButtonEvent& e=*(FGLW::MouseButtonEvent*)eventData;
                state.buttons[e.button]=e.mode;
                break;}
        }; });
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

    uint32 *screen = (uint32 *)malloc(WIDTH * HEIGHT * sizeof(uint32));
    MaterialType *mat = (MaterialType *)malloc(WIDTH * HEIGHT * sizeof(MaterialType));
    Texture texture(WIDTH, HEIGHT, screen);
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        mat[i] = MAT_AIR;
        screen[i] = Material<MAT_AIR>::color;
    }
    texture.UpdateTexture();
    int running = 1;
    mat[WIDTH / 2 + (HEIGHT - 2) * WIDTH] = MAT_WATER;

    while (running)
    {
        if (input.buttons[0])
        {
            float i = (int)(input.x * WIDTH) + (int)(input.y * HEIGHT) * WIDTH;
            mat[(int)i] = MAT_SAND;
        }
        if (input.buttons[1])
        {
            float i = (int)(input.x * WIDTH) + (int)(input.y * HEIGHT) * WIDTH;
            mat[(int)i] = MAT_WATER;
        }
        for (int x = WIDTH-1; x >= 0; x--)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                switch ((MaterialType)mat[x + y * WIDTH])
                {
                case MAT_SAND:
                    Material<MAT_SAND>::Update(mat, x, y);
                    break;
                case MAT_WATER:
                    Material<MAT_WATER>::Update(mat, x, y);
                    break;
                case MAT_AIR:
                    break;
                }
            }
        }
        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                switch ((MaterialType)mat[x + y * WIDTH])
                {
                case MAT_SAND:
                    screen[x + y * WIDTH] = Material<MAT_SAND>::color;
                    break;
                case MAT_AIR:
                    screen[x + y * WIDTH] = Material<MAT_AIR>::color;
                    break;
                case MAT_WATER:
                    screen[x + y * WIDTH] = Material<MAT_WATER>::color;
                    break;
                }
            }
        }
        texture.UpdateTexture();
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        texture.Uniform(shaderProgram, "text");
        glDrawArrays(GL_TRIANGLES, 0, 6);
        running = window.PollEvents();
        window.SwapBuffers();
    }
}