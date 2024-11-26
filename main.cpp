﻿#define _CRT_SECURE_NO_WARNINGS

#define CRES 30 // Circle Resolution = Rezolucija kruga

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLFWcursor* boneCursor = nullptr;

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);
void bindTexture(unsigned int shader, unsigned texture);
void fillDogVAO();
void fillTreeVAO();
void fillFenceVAO();
void fillWindowVAO();
void setupBoneCursor(GLFWwindow* window);


unsigned int VAO[6];
unsigned int VBO[6];



int main(void)
{

    if (!glfwInit())
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 1920;
    unsigned int wHeight = 1080;
    const char wTitle[] = "Na kraj sela zuta kuca";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    setupBoneCursor(window);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }
    unsigned int srbShader = createShader("basic.vert", "srb.frag");
    unsigned int windowShader = createShader("basic.vert", "window.frag");
    unsigned int japShader = createShader("basic.vert", "jap.frag");
    unsigned int fenceShader = createShader("basic.vert", "fence.frag");
    unsigned int treeShader = createShader("tex.vert", "tree.frag");
    unsigned int dogShader = createShader("moving.vert", "tree.frag");


    glGenVertexArrays(6, VAO);
    glGenBuffers(6, VBO);

    float gr = 0 / 255.0;
    float gg = 244 / 255.0;
    float gb = 23 / 255.0;

    float br = 144 / 255.0;
    float bg = 213 / 255.0;
    float bb = 255 / 255.0;

    float yr = 247 / 255.0;
    float yg = 216 / 255.0;
    float yb = 66 / 255.0;

    float bwr = 110 / 255.0;
    float bwg = 38 / 255.0;
    float bwb = 14 / 255.0;

    float rr = 174 / 255.0;
    float rg = 44 / 255.0;
    float rb = 35 / 255.0;

    float vertices[] =
    {
         -1.0, 0.25,    br, bg, bb,
         1.0, 0.25,     br, bg, bb,
         -1.0, 1.0,     br, bg, bb,
         1.0, 1.0,      br, bg, bb,

         -1.0, 0.25,    gr, gg, gb,
         1.0, 0.25,     gr, gg, gb,
         -1.0, -1.0,    gr, gg, gb,
         1.0, -1.0,     gr, gg, gb,

         0.25, -0.60,   yr, yg, yb,
         0.75, -0.60,   yr, yg, yb,
         0.25, 0.30,    yr, yg, yb,
         0.75, 0.30,    yr, yg, yb,

         0.44, -0.60,   bwr, bwg, bwb,
         0.56, -0.60,   bwr, bwg, bwb,
         0.44, -0.35,   bwr, bwg, bwb,
         0.56, -0.35,   bwr, bwg, bwb,

         0.28, -0.28,   0, 0, 0,
         0.42, -0.28,   0, 0, 0,
         0.28, -0.08,   0, 0, 0,
         0.42, -0.08,   0, 0, 0,

         0.58, -0.28,   0, 0, 0,
         0.72, -0.28,   0, 0, 0,
         0.58, -0.08,   0, 0, 0,
         0.72, -0.08,   0, 0, 0,

         0.28, 0.02,   0, 0, 0,
         0.42, 0.02,   0, 0, 0,
         0.28, 0.22,   0, 0, 0,
         0.42, 0.22,   0, 0, 0,

         0.58, 0.02,   0, 0, 0,
         0.72, 0.02,   0, 0, 0,
         0.58, 0.22,   0, 0, 0,
         0.72, 0.22,   0, 0, 0,

         0.20, 0.30,   rr, rg, rb,
         0.80, 0.30,   rr, rg, rb,
         0.50, 0.70,   rr, rg, rb,
    };

    


    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    

    float aspectRatio = (float)wWidth / wHeight;

    float circle[(CRES + 2) * 2];
    float r = 0.1;

    circle[0] = 0.0; // Centar X
    circle[1] = 0.8; // Centar Y

    for (int i = 0; i <= CRES; i++)
    {
        circle[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 360 / CRES)) / aspectRatio;
        circle[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 360 / CRES)) + 0.8;
    }

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    fillWindowVAO();
    fillFenceVAO();
    fillTreeVAO();
    fillDogVAO();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Tekstura
    unsigned treeTexture = loadImageToTexture("res/sljiva.png");
    bindTexture(treeShader, treeTexture);

    unsigned dogTexture = loadImageToTexture("res/balrog.png");
    bindTexture(dogShader, dogTexture);

    //Uniforme
    unsigned int uXpos = glGetUniformLocation(dogShader, "uXpos");
    unsigned int uFlip = glGetUniformLocation(dogShader, "uFlip");

    unsigned int uWhiteLevel = glGetUniformLocation(treeShader, "uWhiteLevel");

    unsigned int uTransparency = glGetUniformLocation(windowShader, "uTransparency");
    unsigned int uPulse = glGetUniformLocation(japShader, "uPulse");
    float transparency = 1.0;

    float x_move = 0;
    float flip = 1.0;

    float whiteLevel = 0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(srbShader);

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            transparency = 0.5;  // Poluprovidno staklo
        }

        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        {
            transparency = 1.0f;  // Neporvidno staklo
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            whiteLevel += 0.0001f; // Povećaj nivo krečenja
            if (whiteLevel > 0.38f) whiteLevel = 0.38f;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            whiteLevel -= 0.0001f; // Smanji nivo krečenja
            if (whiteLevel < 0.0f) whiteLevel = 0.0f; // Ograniči ispod stabla
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            x_move += 0.0001;
            if (x_move > 0.6)
                x_move = 0.6;
            flip = -1.0;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            x_move -= 0.0001;
            if (x_move < -0.45)
                x_move = -0.45;
            flip = 1.0;
        }


        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 32, 3);


        glUseProgram(windowShader);

        glBindVertexArray(VAO[5]);
        glUniform1f(uTransparency, transparency);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        //glLineWidth(4.0);
        //GL_LINE_LOOP za linije

        glUseProgram(japShader);
        glBindVertexArray(VAO[1]);
        float minScale = 0.3f; // Minimalna vrednost
        float maxScale = 0.5f; // Maksimalna vrednost
        float oscillation = minScale + (maxScale - minScale) * (0.5f * (1.0f + sin(glfwGetTime())));
        glUniform1f(uPulse, oscillation);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));


        glUseProgram(fenceShader);
        glBindVertexArray(VAO[2]);
        int i = 0;
        while (i < 34) {
            glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
            i++;
        }

        glUseProgram(treeShader);
        glBindVertexArray(VAO[3]);
        glUniform1f(uWhiteLevel, whiteLevel);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, treeTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        glUseProgram(dogShader);
        glUniform1f(uXpos, x_move);
        glUniform1f(uFlip, flip);

        glBindVertexArray(VAO[4]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dogTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);


        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(2, VBO);
    glDeleteVertexArrays(2, VAO);
    glDeleteProgram(srbShader);
    glDeleteProgram(japShader);

    glfwTerminate();
    return 0;
}


void fillWindowVAO() {
    float window_vertices[] =
    {
         0.29, -0.27,   1, 1, 1,
         0.41, -0.27,   1, 1, 1,
         0.29, -0.09,   1, 1, 1,
         0.41, -0.09,   1, 1, 1,

         0.59, -0.27,   1, 1, 1,
         0.71, -0.27,   1, 1, 1,
         0.59, -0.09,   1, 1, 1,
         0.71, -0.09,   1, 1, 1,

         0.29, 0.03,    1, 1, 1,
         0.41, 0.03,    1, 1, 1,
         0.29, 0.21,    1, 1, 1,
         0.41, 0.21,    1, 1, 1,

         0.59, 0.03,    1, 1, 1,
         0.71, 0.03,    1, 1, 1,
         0.59, 0.21,    1, 1, 1,
         0.71, 0.21,    1, 1, 1,
    };

    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(window_vertices), window_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


void fillFenceVAO() {
    float fence[1000] = {
       -1.0, -0.75,
        1.0, -0.75,
       -1.0, -0.7,
        1.0, -0.7,
    };
    int i = 8;
    float x = -0.98;
    while (x < 1.0) {
        fence[i] = x;
        fence[i + 1] = -1.0;

        fence[i + 2] = x;
        fence[i + 3] = -0.65;

        x += 0.03;

        fence[i + 4] = x;
        fence[i + 5] = -1.0;

        fence[i + 6] = x;
        fence[i + 7] = -0.65;

        x += 0.03;
        i += 8;
    }
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fence), fence, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void fillTreeVAO() {
    float tree_vertices[] =
    {   //X    Y        S    T 
        -0.35, 0.6,         0.0, 1.0,//gore levo
        -1.0, 0.6,          1.0, 1.0, //gore desno
        -0.35, -0.35,       0.0, 0.0, //dole levo
        -1.0, -0.35,       1.0, 0.0, //dole desno
    };
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tree_vertices), tree_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void fillDogVAO() {
    float dog_vertices[] =
    {   //X    Y        S    T 
        -0.35, -0.07,        1.0, 1.0,//gore desno
        -0.55, -0.07,        0.0, 1.0, //gore levo
        -0.35, -0.53,       1.0, 0.0, //dole desno
        -0.55, -0.53,       0.0, 0.0, //dole levo
    };

    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dog_vertices), dog_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void bindTexture(unsigned int shader, unsigned texture) {
    glBindTexture(GL_TEXTURE_2D, texture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(texture);
    unsigned uTexLoc2 = glGetUniformLocation(shader, "uTex");
    glUniform1i(uTexLoc2, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)
    glUseProgram(0);
}

//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################



unsigned int compileShader(GLenum type, const char* source)
{
    //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
    //Citanje izvornog koda iz fajla
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); //Izvorni kod sejdera koji citamo iz fajla na putanji "source"

    int shader = glCreateShader(type); //Napravimo prazan sejder odredjenog tipa (vertex ili fragment)
    
    int success; //Da li je kompajliranje bilo uspjesno (1 - da)
    char infoLog[512]; //Poruka o gresci (Objasnjava sta je puklo unutar sejdera)
    glShaderSource(shader, 1, &sourceCode, NULL); //Postavi izvorni kod sejdera
    glCompileShader(shader); //Kompajliraj sejder

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); //Provjeri da li je sejder uspjesno kompajliran
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); //Pribavi poruku o gresci
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    //Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource

    unsigned int program; //Objedinjeni sejder
    unsigned int vertexShader; //Verteks sejder (za prostorne podatke)
    unsigned int fragmentShader; //Fragment sejder (za boje, teksture itd)

    program = glCreateProgram(); //Napravi prazan objedinjeni sejder program

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); //Napravi i kompajliraj vertex sejder
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource); //Napravi i kompajliraj fragment sejder

    //Zakaci verteks i fragment sejdere za objedinjeni program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program); //Povezi ih u jedan objedinjeni sejder program
    glValidateProgram(program); //Izvrsi provjeru novopecenog programa

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); //Slicno kao za sejdere
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    //Posto su kodovi sejdera u objedinjenom sejderu, oni pojedinacni programi nam ne trebaju, pa ih brisemo zarad ustede na memoriji
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}

void setupBoneCursor(GLFWwindow* window) {
    int width, height, channels;
    unsigned char* data = stbi_load("res/bone.png", &width, &height, &channels, 4); // učitaj sliku kosti

    if (data) {
        GLFWimage cursorImage;
        cursorImage.width = width;
        cursorImage.height = height;
        cursorImage.pixels = data;

        boneCursor = glfwCreateCursor(&cursorImage, 0, 0); // postavi gornji levi ugao slike kao žarišnu tačku
        glfwSetCursor(window, boneCursor);

        stbi_image_free(data);
    }
    else {
        std::cerr << "Failed to load cursor image!" << std::endl;
    }
}
