#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#include<FGLW/FGLW.hpp>
int main()
{
    FGLW::FGLW window(0,0,FGLW_RESOLUTION_W,FGLW_RESOLUTION_H,"Window");
    window.MakeContextCurrent();
    window.SetIcon("FGLW.ico");
    int running=1;
    while(running)
    {
        running=window.PollEvents();
    }
}