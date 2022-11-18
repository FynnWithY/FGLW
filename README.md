# FGLW
Free and open source window library for C++. \
Supports only Windows for now.

## Platforms
- Windows
## Functions

- ### Create Window 
    ` FGLW(FGLWu32 width,FGLWu32 height,FGLWstring title); ` \
    usage: \
    `FGLW window=FGLW(FGLWu32 width,FGLWu32 height,FGLWstring title);`
- ### Poll Events 
    polls events and returns 0 if window should close \
    ` bool PollEvents();` \
    usage: \
    ``` c++
    if(!window.PollEvents()) 
    {     
        running=0;
    } 
    ```
- ### Swap Buffers
    swaps the window buffers \
    `void SwapBuffers();`
- ### Make Context Current
    makes the window the current opengl context \
    `void MakeContextCurrent();`