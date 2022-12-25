# FGLW
Free and open source window library for C++. \
Supports only Windows for now.

## Platforms
- Windows
## Functions

- ### Create Window 
    ```c++
    FGLW(FGLWu32 width,FGLWu32 height,FGLWstring title);
    ``` 
    usage: 
    ```c++ 
    FGLW window=FGLW(FGLWu32 width,FGLWu32 height,FGLWstring title);
    ```
- ### Poll Events 
    polls events and returns 0 if window should close 
    ```c++
     bool PollEvents();
    ``` 
    usage: 
    ``` c++
    if(!window.PollEvents()) 
    {     
        running=0;
    } 
    ```
- ### Swap Buffers
    swaps the window buffers \
    ``` c++
    void SwapBuffers();
    ``` 
- ### Make Context Current
    makes the window the current opengl context \
    `void MakeContextCurrent();`