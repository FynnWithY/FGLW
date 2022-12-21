#include <FGLW/FGLW.hpp>
#include <iostream>
#include <math.h>

constexpr float PI=3.1415926;
constexpr float PI_2=PI*2;

struct mat4
{
    float mat[16];
    mat4() = default;
    mat4(float value)
    {
        for (int i = 0; i < 16; i++)
        {
            mat[i] = value;
        }
        setScale(1,1,1);
    }
    operator float *() { return mat; }
    float &operator[](int index) { return mat[index]; }
    void rotate(float angle,unsigned char axis)
    {
        setScale(1,1,1);
        if(axis==0)
        {
            mat[5]=cos(angle);
            mat[6]=-sin(angle);
            mat[9]=sin(angle);
            mat[10]=cos(angle);
        }else if(axis==1)
        {
            mat[0]=cos(angle);
            mat[2]=sin(angle);
            mat[8]=-sin(angle);
            mat[10]=cos(angle);
        }else if(axis==2)
        {
            mat[0]=cos(angle);
            mat[1]=-sin(angle);
            mat[4]=sin(angle);
            mat[5]=cos(angle);
        }
    };
    void transform(float x,float y,float z)
    {
        mat[3]=x;
        mat[7]=y;
        mat[11]=z;
    };
    void setScale(float x,float y,float z)
    {
        mat[0]=x;
        mat[5]=y;
        mat[10]=z;
        mat[15]=1;
    };
    mat4 operator *(mat4 matrix)
    {
        mat4 self=*this;
        mat4 mat;
        for(int x=0;x<4;x++)
        {
            for(int y=0;y<4;y++){
                int y4=y*4;
                mat[y4+x]=matrix[y4+0]*self[x+(0)*4]+
                           matrix[y4+1]*self[x+(1)*4]+
                           matrix[y4+2]*self[x+(2)*4]+
                           matrix[y4+3]*self[x+(3)*4];
            }
        }

        return mat;
    }
};

void CreateProjectionMatrix(mat4 *mat, float width, float height)
{
    mat4 pro(0);

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)height / (float)width;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    pro.mat[0] = fAspectRatio * fFovRad;
    pro.mat[5] = fFovRad;
    pro.mat[10] = fFar / (fFar - fNear);
    pro.mat[11] = (-fFar * fNear) / (fFar - fNear);
    pro.mat[14] = 1.0f;
    pro.mat[15] = 0.0f;
    *mat = pro;
}

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "uniform mat4 camera;\n"
                                 "uniform mat4 transform,rotation,scale;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0)*rotation*transform*camera;\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1,0,1,1);\n"
                                   "}\0";
unsigned int shaderProgram;
void loadShader()
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, &infoLog[0]);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
int width = FGLW_RESOLUTION_W, height = FGLW_RESOLUTION_H;
int main()
{
    FGLW::FGLW window(0, 0, FGLW_RESOLUTION_W, FGLW_RESOLUTION_H, "Cube");
    window.MakeContextCurrent();
    FGLW::FGLW::GLInit();
    loadShader();
    int running = 1;
    mat4 projectionMatrix;
    CreateProjectionMatrix(&projectionMatrix, width, height);
    float vertices[] =
        {
            -1, -1, 1, -1, 1, 1,  1, 1, 1,
            1, 1, 1,   1, -1, 1, -1, -1, 1,

            -1, -1, 1,  -1, 1, 1,  -1, 1, -1,
            -1, 1, -1,  -1, -1, -1,  -1, -1, 1,

            -1, 1, -1,   1, -1, -1,   -1, -1, -1,
            -1, 1, -1,   1, 1, -1,     1, -1, -1,

            1, -1, -1,   1, 1, -1,     1, 1, 1,
            1, 1, 1,     1, -1, 1,     1, -1, -1,
            
            -1, 1, 1,   -1, 1, -1,     1, 1, -1,
            1, 1, -1,    1, 1, 1,      -1, 1, 1,

            -1, -1, 1,   -1, -1, -1,     1, -1, -1,
            1, -1, -1,    1, -1, 1,      -1, -1, 1};

    unsigned int vbo;
    glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    unsigned int vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    float angleX=0,angleY=0,angleZ=0;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    mat4 rotation(0);
    mat4 rotationX(0);
    mat4 rotationY(0);
    mat4 rotationZ(0);
    mat4 scale(0);

    //mat4 mat1(1),mat2(1);
    //mat4 m=mat1*mat2;
    //for(int x=0;x<4;x++)
    //{
    //    for(int y=0;y<4;y++)
    //    {
    //        printf("%f,",m.mat[x+y*4]);
    //    }
    //    printf("\n");
    //}
    while (running)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        mat4 transform(0);
        //transform.mat[15]=1;
        angleX+=.011;
        angleY+=.013;
        angleZ+=.016;
        transform.transform(0,0,5);
        //transform.rotate(angle,0);
        rotationX.rotate(angleX,0);
        rotationY.rotate(angleY,1);
        rotationZ.rotate(angleZ,2);
        rotation=rotationX*rotationY*rotationZ;
        //rotation.rotate(angle,0);
        //scale.setScale(1,1,1);
        //transform=transform*scale*rotation;
        //transform.rotate(angle,0);
        //transform.rotate(angle,1);
        //transform.rotate(angle,2);
        //transform.transform(0,0,-1);
        glBindVertexArray(vao);
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "camera"), 1, 0, projectionMatrix.mat);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, 0, transform.mat);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "rotation"), 1, 0, rotation.mat);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "scale"), 1, 0, scale.mat);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        running = window.PollEvents();
        window.SwapBuffers();

    }
}