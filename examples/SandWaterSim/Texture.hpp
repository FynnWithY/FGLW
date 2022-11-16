#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include<FGLW/FGLW.hpp>
#include<iostream>
typedef unsigned int uint32;
typedef unsigned char uint8;

class Texture
{
private:
    uint32 handle,*data;
    int width,height;
public:
    Texture(/* args */)=default;
    Texture(int width,int height,unsigned int *data);
    ~Texture()=default;
    void UpdateTexture();
    void Uniform(uint32 shader,const char* name,int t=0);
    void Bind(int t=0);
};

Texture::Texture(int width,int height,unsigned int *data):width(width),height(height),data(data)
{
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    UpdateTexture();
}   

void Texture::UpdateTexture()
{
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Uniform(uint32 shader,const char* name,int t)
{
    Bind(t);
    glUniform1i(glGetUniformLocation(shader,name),t);
}

void Texture::Bind(int t)
{
    glActiveTexture(GL_TEXTURE0+t);
    glBindTexture(GL_TEXTURE_2D, handle);
}

#endif