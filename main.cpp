﻿#define _CRT_SECURE_NO_WARNINGS

#define CRES 30 // Circle Resolution = Rezolucija kruga

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include <vector>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <chrono>
#include <thread>

GLFWcursor* boneCursor = nullptr;

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);
void bindTexture(unsigned int shader, unsigned texture);
void fillBaseVAO();
void fillDogVAO();
void fillTreeVAO();
void fillFenceVAO();
void fillWindowVAO();
void fillRoomVAO();
void fillNameVAO();
void fillRoomBgVAO();
void drawBackground(unsigned int srbShader, unsigned int uDarkening);
void drawGrass(unsigned int srbShader);
void drawHouse(unsigned int srbShader);
void drawRoomBg(unsigned int roomBgShader, unsigned int uPulseRoom);
void drawZZZ(unsigned int zzzShader, float* time1, float* time2, float* time3, float* t1, float* t2);
void drawSmoke(unsigned int smokeShader, float* smokeTime);
void drawFood(unsigned int foodShader, unsigned foodTexture);
void drawRoom(unsigned int roomShader, unsigned dogTexture);
void drawWindows(unsigned int windowShader, unsigned int uTransparency, float transparency, unsigned int uDarkeningW);
void drawFence(unsigned int fenceShader, unsigned int uDarkeningF);
void drawName(unsigned int nameShader, unsigned nameTexture);
void drawTree(unsigned int treeShader, unsigned int uWhiteLevel, float whiteLevel, unsigned treeTexture);
void drawDog(unsigned int dogShader, unsigned uXpos, float x_move, float flip, unsigned uFlip, unsigned currentDogTexture);

void setupBoneCursor(GLFWwindow* window);
bool isMouseOverGrass(double mouseX, double mouseY);
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void dogFeeding(float positionX, float positionY);

unsigned int bigVAO;
unsigned int bigVBO;
unsigned int sunVAO;
unsigned int sunVBO;
unsigned int fenceVAO;
unsigned int fenceVBO;
unsigned int treeVAO;
unsigned int treeVBO;
unsigned int dogVAO;
unsigned int dogVBO;
unsigned int windowVAO;
unsigned int windowVBO;
unsigned int roomVAO;
unsigned int roomVBO;
unsigned int nameVAO;
unsigned int nameVBO;
unsigned int roomBgVAO;
unsigned int roomBgVBO;
unsigned int zzzVAO;
unsigned int zzzVBO;
unsigned int smokeVAO;
unsigned int smokeVBO;
unsigned int foodVAO;
unsigned int foodVBO;

bool isFoodPresent = false;                 // Da li je hrana na sceni
float foodPosX = 0.0f, foodPosY = 0.0f;     // Pozicija hrane
bool isNight = false;
bool isEating = false;
bool foodWaiting = false;
float startTimeSun = 0.0;
float startTimeMoon = 0.0;

float sunX = 0.0;
float sunY = 0.0;
float moonX = 0.0;
float moonY = 0.0;

float dogX = -0.35;
float dogY = -0.07;

float eatDogX = -0.35;
float eatDogY = -0.17;

float foodX = 0.0;
float foodY = -0.53;

float feedingTime = 0;

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
    glfwSetMouseButtonCallback(window, mouseClickCallback);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    unsigned int srbShader = createShader("basic.vert", "srb.frag");
    unsigned int windowShader = createShader("basic.vert", "window.frag");
    unsigned int japShader = createShader("sun.vert", "jap.frag");
    unsigned int fenceShader = createShader("basic.vert", "fence.frag");
    unsigned int treeShader = createShader("tex.vert", "tree.frag");
    unsigned int dogShader = createShader("moving.vert", "tree.frag");
    unsigned int roomShader = createShader("tex.vert", "tree.frag");
    unsigned int nameShader = createShader("tex.vert", "tree.frag");
    unsigned int moonShader = createShader("sun.vert", "moon.frag");
    unsigned int roomBgShader = createShader("basic.vert", "roombg.frag");
    unsigned int zzzShader = createShader("zzz.vert", "disappear.frag");
    unsigned int smokeShader = createShader("smoke.vert", "smoking.frag");
    unsigned int foodShader = createShader("tex.vert", "tree.frag");



    glGenVertexArrays(1, &bigVAO);
    glGenBuffers(1, &bigVBO);
    glGenVertexArrays(1, &sunVAO);
    glGenBuffers(1, &sunVBO);
    glGenVertexArrays(1, &fenceVAO);
    glGenBuffers(1, &fenceVBO);
    glGenVertexArrays(1, &treeVAO);
    glGenBuffers(1, &treeVBO);
    glGenVertexArrays(1, &dogVAO);
    glGenBuffers(1, &dogVBO);
    glGenVertexArrays(1, &windowVAO);
    glGenBuffers(1, &windowVBO);
    glGenVertexArrays(1, &roomVAO);
    glGenBuffers(1, &roomVBO);
    glGenVertexArrays(1, &nameVAO);
    glGenBuffers(1, &nameVBO);
    glGenVertexArrays(1, &roomBgVAO);
    glGenBuffers(1, &roomBgVBO);
    glGenVertexArrays(1, &zzzVAO);
    glGenBuffers(1, &zzzVBO);
    glGenVertexArrays(1, &smokeVAO);
    glGenBuffers(1, &smokeVBO);
    glGenVertexArrays(1, &foodVAO);
    glGenBuffers(1, &foodVBO);

    fillBaseVAO();
    
    float aspectRatio = (float)wWidth / wHeight;

    float circle[(CRES + 2) * 2];
    float r = 0.1;

    circle[0] = 0.0; // Centar X
    circle[1] = 0.0; // Centar Y

    for (int i = 0; i <= CRES; i++)
    {
        circle[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 360 / CRES)) / aspectRatio;
        circle[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 360 / CRES)) + 0.0;
    }

    glBindVertexArray(sunVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    fillWindowVAO();
    fillFenceVAO();
    fillTreeVAO();
    fillDogVAO();
    fillRoomVAO();
    fillNameVAO();
    fillRoomBgVAO();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Teksture
    unsigned treeTexture = loadImageToTexture("res/sljiva.png");
    bindTexture(treeShader, treeTexture);

    unsigned dogTexture = loadImageToTexture("res/plutonTrci.png");
    unsigned dogLayTexture = loadImageToTexture("res/plutonLezi.png");
    unsigned dogSitTexture = loadImageToTexture("res/plutonSedi.png");
    bindTexture(dogShader, dogTexture);
    bindTexture(dogShader, dogLayTexture);
    bindTexture(dogShader, dogSitTexture);

    unsigned margeTexture = loadImageToTexture("res/marge.png");
    unsigned homerTexture = loadImageToTexture("res/homer.png");

    bindTexture(roomShader, margeTexture);
    bindTexture(roomShader, homerTexture);


    unsigned nameTexture = loadImageToTexture("res/aleksa2.png");
    bindTexture(nameShader, nameTexture);

    unsigned foodTexture = loadImageToTexture("res/dogfood.png");
    bindTexture(foodShader, foodTexture);


    //Uniforme
    unsigned int uDarkening = glGetUniformLocation(srbShader, "uDarkening");
    unsigned int uDarkeningW = glGetUniformLocation(windowShader, "uDarkening");
    unsigned int uDarkeningF = glGetUniformLocation(fenceShader, "uDarkening");

    unsigned int uPulseRoom = glGetUniformLocation(roomBgShader, "uPulse");

    unsigned int uXpos = glGetUniformLocation(dogShader, "uXpos");
    unsigned int uFlip = glGetUniformLocation(dogShader, "uFlip");

    unsigned int uPosLocSun = glGetUniformLocation(japShader, "uPos");

    unsigned int uWhiteLevel = glGetUniformLocation(treeShader, "uWhiteLevel");

    unsigned int uTransparency = glGetUniformLocation(windowShader, "uTransparency");
    unsigned int uPulse = glGetUniformLocation(japShader, "uPulse");
    float transparency = 1.0;

    float x_move = 0;
    float flip = 1.0;

    float whiteLevel = 0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float rrr = 0.9;                                //Poluprecnik kruznice po kojoj se sunce i mesec krecu
    float rotationSpeed = 0.8;                      //brzina rotacije sunca/meseca
    bool first = true;
    glClearColor(1.0, 1.0, 1.0, 1.0);
    startTimeSun = glfwGetTime() + 0.5;
    startTimeMoon = glfwGetTime() + 4;

    //startTime = -1 * startTime;
    bool isRotating = false;
    float t1 = 0.0f;
    float t2 = 0.0f;
    float t3 = 0.0f;

    float broj = 0;
    float time1 = glfwGetTime();
    float time2 = glfwGetTime() + 2;
    float time3 = glfwGetTime() + 4;

    float smokeTime = 0.0f;
    float particleLife = 3.0f;                      // Svaka čestica živi 3 sekunde

    bool reachedFood = false;
    bool finishedEating = false;

    double lastFrameTime = 0.0;
    const double targetFrameRate = 1.0 / 60.0;
    double currentTime = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        if (currentTime - lastFrameTime >= targetFrameRate) {
            lastFrameTime = currentTime;
            // ovde metis kod
            unsigned currentDogTexture = dogSitTexture;

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

            glClear(GL_COLOR_BUFFER_BIT);

            if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
            {
                transparency = 0.3;  // Poluprovidno staklo
            }

            if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
            {
                transparency = 1.0f;  // Neporvidno staklo
            }
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                whiteLevel += 0.003f; // Povećaj nivo krečenja
                if (whiteLevel > 0.38f) whiteLevel = 0.38f;
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                whiteLevel -= 0.003f; // Smanji nivo krečenja
                if (whiteLevel < 0.0f) whiteLevel = 0.0f; // Ograniči ispod stabla
            }

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                if (!isNight && !isEating) {
                    x_move += 0.003;
                    currentDogTexture = dogTexture;
                    if (x_move > 0.6) {
                        x_move = 0.6;
                    }
                    else {
                        dogX += 0.003;
                        eatDogX += 0.003;
                    }
                    flip = 1.0;
                }
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                if (!isNight && !isEating) {
                    x_move -= 0.003;
                    currentDogTexture = dogTexture;
                    if (x_move < -0.45) {
                        x_move = -0.45;
                    }
                    else {
                        dogX -= 0.003;
                        eatDogX -= 0.003;
                    }
                    flip = -1.0;
                }
            }
            if (!isNight && foodWaiting) {
                isEating = true;
                feedingTime = glfwGetTime() + 7;
            }
            if (isEating)
            {
                if (reachedFood) {
                    if (glfwGetTime() > feedingTime) {
                        finishedEating = true;
                        if (eatDogX > dogX) {
                            flip = -1;
                            x_move -= 0.003;
                            currentDogTexture = dogTexture;
                            eatDogX -= 0.003;
                            if (eatDogX <= dogX) {
                                isEating = false;
                                reachedFood = false;
                            }
                        }
                        else if (eatDogX < dogX) {
                            flip = 1;
                            x_move += 0.003;
                            eatDogX += 0.003;
                            currentDogTexture = dogTexture;
                            if (eatDogX >= dogX) {
                                isEating = false;
                                reachedFood = false;
                            }
                        }
                    }
                    else {
                        currentDogTexture = dogSitTexture;
                        foodWaiting = false;
                    }
                }
                else {
                    finishedEating = false;
                    if (eatDogX - 0.15 > foodX) {
                        flip = -1;
                        x_move -= 0.003;
                        eatDogX -= 0.003;
                        currentDogTexture = dogTexture;
                        if (eatDogX - 0.15 < foodX) {
                            reachedFood = true;
                        }
                    }
                    else if (eatDogX - 0.15 < foodX) {
                        flip = 1;
                        x_move += 0.003;
                        eatDogX += 0.003;
                        currentDogTexture = dogTexture;
                        if (eatDogX - 0.15 > foodX) {
                            reachedFood = true;
                        }
                    }
                }
            }

            if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            {
                if (!isNight) {
                    isNight = true;
                    isRotating = true;
                }
            }
            if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
            {
                if (isNight) {
                    isNight = false;
                    isRotating = true;
                }
            }


            drawBackground(srbShader, uDarkening);

            glUseProgram(japShader);
            glBindVertexArray(sunVAO);
            float minScale = 0.3f;
            float maxScale = 0.5f;
            float oscillation = minScale + (maxScale - minScale) * (0.5f * (1.0f + sin(glfwGetTime())));
            glUniform1f(uPulse, oscillation);
            glUniform2f(uPosLocSun, rrr * cos(startTimeSun), rrr * sin(startTimeSun));
            sunX = rrr * cos(startTimeSun);
            sunY = rrr * sin(startTimeSun);
            if (isRotating) {
                if (isNight) {
                    startTimeSun += 0.03;
                    sunX = rrr * cos(startTimeSun);
                    sunY = rrr * sin(startTimeSun);

                    glUniform2f(uPosLocSun, rrr * cos(startTimeSun), rrr * (sin(startTimeSun)));
                    if (sunX < -0.5 && sunY < -0.7) {
                        isRotating = false;
                    }
                }
                else {
                    startTimeSun += 0.03;
                    sunX = rrr * cos(startTimeSun);
                    sunY = rrr * sin(startTimeSun);

                    glUniform2f(uPosLocSun, rrr * cos(startTimeSun), rrr * (sin(startTimeSun)));
                    if (sunX > -0.3 && sunY > 0.8) {
                        isRotating = false;
                    }
                }
            }
            glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));

            glUseProgram(moonShader);
            glBindVertexArray(sunVAO);
            glUniform2f(uPosLocSun, rrr * cos(startTimeMoon), rrr * sin(startTimeMoon));
            moonX = rrr * cos(startTimeMoon);
            moonY = rrr * sin(startTimeMoon);
            if (isRotating) {
                if (isNight) {
                    startTimeMoon += 0.03;
                    moonX = rrr * cos(startTimeMoon);
                    moonY = rrr * sin(startTimeMoon);

                    glUniform2f(uPosLocSun, rrr * cos(startTimeMoon), rrr * (sin(startTimeMoon)));
                }
                else {
                    startTimeMoon += 0.03;
                    moonX = rrr * cos(startTimeMoon);
                    moonY = rrr * sin(startTimeMoon);

                    glUniform2f(uPosLocSun, rrr * cos(startTimeMoon), rrr * (sin(startTimeMoon)));
                }
            }
            glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));


            drawGrass(srbShader);

            drawHouse(srbShader);

            drawRoomBg(roomBgShader, uPulseRoom);

            if (isNight) {
                drawRoom(roomShader, homerTexture);
            }
            else {
                drawRoom(roomShader, margeTexture);
            }

            drawWindows(windowShader, uTransparency, transparency, uDarkeningW);

            drawFence(fenceShader, uDarkeningF);

            drawName(nameShader, nameTexture);

            drawTree(treeShader, uWhiteLevel, whiteLevel, treeTexture);

            if (isNight && !isEating) {
                currentDogTexture = dogLayTexture;
            }

            drawDog(dogShader, uXpos, x_move, flip, uFlip, currentDogTexture);

            if (isNight && !isEating) {
                drawZZZ(zzzShader, &time1, &time2, &time3, &t1, &t2);
            }

            drawSmoke(smokeShader, &smokeTime);

            if ((isEating && !finishedEating) || foodWaiting) {
                drawFood(foodShader, foodTexture);
            }

            glUseProgram(0);
            glBindVertexArray(0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
    }
    glDeleteTextures(1, &treeTexture);
    glDeleteTextures(1, &dogTexture);
    glDeleteTextures(1, &dogLayTexture);
    glDeleteTextures(1, &dogSitTexture);
    glDeleteTextures(1, &nameTexture);
    glDeleteTextures(1, &margeTexture);
    glDeleteTextures(1, &homerTexture);
    glDeleteTextures(1, &foodTexture);

    glDeleteBuffers(1, &bigVBO);
    glDeleteVertexArrays(1, &bigVAO);
    glDeleteBuffers(1, &sunVBO);
    glDeleteVertexArrays(1, &sunVAO);
    glDeleteBuffers(1, &fenceVBO);
    glDeleteVertexArrays(1, &fenceVAO);
    glDeleteBuffers(1, &treeVBO);
    glDeleteVertexArrays(1, &treeVAO);
    glDeleteBuffers(1, &dogVBO);
    glDeleteVertexArrays(1, &dogVAO);
    glDeleteBuffers(1, &windowVBO);
    glDeleteVertexArrays(1, &windowVAO);
    glDeleteBuffers(1, &roomVBO);
    glDeleteVertexArrays(1, &roomVAO);
    glDeleteBuffers(1, &nameVBO);
    glDeleteVertexArrays(1, &nameVAO);
    glDeleteBuffers(1, &roomBgVBO);
    glDeleteVertexArrays(1, &roomBgVAO);
    glDeleteBuffers(1, &zzzVBO);
    glDeleteVertexArrays(1, &zzzVAO);
    glDeleteBuffers(1, &smokeVBO);
    glDeleteVertexArrays(1, &smokeVAO);
    glDeleteBuffers(1, &foodVBO);
    glDeleteVertexArrays(1, &foodVAO);

    glDeleteProgram(srbShader);
    glDeleteProgram(windowShader);
    glDeleteProgram(japShader);
    glDeleteProgram(fenceShader);
    glDeleteProgram(treeShader);
    glDeleteProgram(dogShader);
    glDeleteProgram(roomShader);
    glDeleteProgram(nameShader);
    glDeleteProgram(moonShader);
    glDeleteProgram(roomBgShader);
    glDeleteProgram(zzzShader);
    glDeleteProgram(smokeShader);
    glDeleteProgram(foodShader);

    glfwTerminate();
    return 0;
}



void drawBackground(unsigned int srbShader, unsigned int uDarkening) {
    glUseProgram(srbShader);
    glBindVertexArray(bigVAO);
    if (isNight) {
        glUniform1f(uDarkening, 0.3);
    }
    else {
        glUniform1f(uDarkening, 1);
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void drawGrass(unsigned int srbShader) {
    glUseProgram(srbShader);
    glBindVertexArray(bigVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
}

void drawHouse(unsigned int srbShader) {
    glUseProgram(srbShader);
    glBindVertexArray(bigVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 32, 3);
    glDrawArrays(GL_TRIANGLE_STRIP, 35, 4);

}

void drawRoomBg(unsigned int roomBgShader, unsigned int uPulseRoom) {
    glUseProgram(roomBgShader);
    glBindVertexArray(roomBgVAO);
    if (isNight) {
        float minScale = 0.66f;
        float maxScale = 1.0f;
        float oscillation = minScale + (maxScale - minScale) * (0.5f * (1.0f + sin(glfwGetTime())));
        glUniform1f(uPulseRoom, oscillation);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    }
    else {
        glUniform1f(uPulseRoom, 0.8);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    }
}

void drawRoom(unsigned int roomShader, unsigned dogTexture) {
    glUseProgram(roomShader);
    glBindVertexArray(roomVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, dogTexture);
    if (isNight) {
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    }
    else {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawWindows(unsigned int windowShader, unsigned int uTransparency, float transparency, unsigned int uDarkeningW) {
    glUseProgram(windowShader);
    glBindVertexArray(windowVAO);
    if (isNight) {
        glUniform1f(uTransparency, 0.4);
        glUniform1f(uDarkeningW, 0.4);
    }
    else {
        glUniform1f(uTransparency, transparency);
        glUniform1f(uDarkeningW, 1);
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    //glLineWidth(4.0);
    //GL_LINE_LOOP za linije
}

void drawName(unsigned int nameShader, unsigned nameTexture) {
    glUseProgram(nameShader);
    glBindVertexArray(nameVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, nameTexture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawFence(unsigned int fenceShader, unsigned int uDarkeningF) {
    glUseProgram(fenceShader);
    glBindVertexArray(fenceVAO);
    if (isNight) {
        glUniform1f(uDarkeningF, 0.5);
    }
    else {
        glUniform1f(uDarkeningF, 1);
    }
    int i = 0;
    while (i < 34) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
        i++;
    }
}

void drawTree(unsigned int treeShader, unsigned int uWhiteLevel, float whiteLevel, unsigned treeTexture) {
    glUseProgram(treeShader);
    glBindVertexArray(treeVAO);
    glUniform1f(uWhiteLevel, whiteLevel);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, treeTexture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawDog(unsigned int dogShader, unsigned uXpos, float x_move, float flip, unsigned uFlip, unsigned currentDogTexture) {
    glUseProgram(dogShader);
    glUniform1f(uXpos, x_move);
    glUniform1f(uFlip, flip);
    glBindVertexArray(dogVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, currentDogTexture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawZZZ(unsigned int zzzShader, float* time1, float* time2, float* time3, float* t1, float* t2) {
    float zzz_tacke[] = {
        dogX-0.08, dogY - 0.06,             //gore levo
        dogX - 0.06, dogY - 0.06,           //gore desno
        dogX - 0.08, dogY-0.08,             //dole levo
        dogX - 0.06, dogY-0.08,             //dole desno
    };
    glBindVertexArray(zzzVAO);
    glBindBuffer(GL_ARRAY_BUFFER, zzzVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zzz_tacke), zzz_tacke, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(zzzShader);
    //time += 0.0001;
    glBindVertexArray(zzzVAO);
    if (*time1 <= glfwGetTime()) {
        *t1 += 0.02;
        glUniform1f(glGetUniformLocation(zzzShader, "uTime"), *t1);
        glLineWidth(2);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    if (*time2 <= glfwGetTime()) {
        *t2 += 0.02;
        glUniform1f(glGetUniformLocation(zzzShader, "uTime"), *t2);
        glLineWidth(2);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    if (*time3 <= glfwGetTime()) {
        *time1 = glfwGetTime();
        *t1 = 0;
        *time2 = glfwGetTime() + 2;
        *t2 = 0;
        *time3 = glfwGetTime() + 4;
    }
}

void drawSmoke(unsigned int smokeShader, float* smokeTime) {
    const int numInstances = 12;  // Broj oblaka istovremeno
    float circle[(CRES + 2) * 2];
    float r = 0.2;

    circle[0] = 0.0; // Centar X
    circle[1] = 0.0; // Centar Y

    for (int i = 0; i <= CRES; i++) {
        circle[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 360 / CRES));
        circle[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 360 / CRES));
    }

    glBindVertexArray(smokeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, smokeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float particleLife = 3.0f;

    glUseProgram(smokeShader);
    glBindVertexArray(smokeVAO);
    *smokeTime += 0.02;

    glUniform1f(glGetUniformLocation(smokeShader, "uTime"), *smokeTime);
    glUniform1f(glGetUniformLocation(smokeShader, "uParticleLife"), particleLife);
    glUniform2f(glGetUniformLocation(smokeShader, "uStartPosition"), 0.66, 0.57);

    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, CRES + 2, numInstances);
}

void drawFood(unsigned int foodShader, unsigned foodTexture) {
    float food_vertices[] =
    {
        foodX, foodY + 0.13,         0.0, 1.0,                  //gore levo
        foodX + 0.12, foodY + 0.13,   1.0, 1.0,                 //gore desno
        foodX, foodY - 0.08,               0.0, 0.0,            //dole levo
        foodX + 0.12, foodY - 0.08,         1.0, 0.0,           //dole desno
    };

    /*std::cout << foodX;
    std::cout << "\n";
    std::cout << foodY;*/

    glBindVertexArray(foodVAO);
    glBindBuffer(GL_ARRAY_BUFFER, foodVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(food_vertices), food_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(foodShader);
    glBindVertexArray(foodVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, foodTexture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void fillBaseVAO() {
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

         0.62, 0.57,    0.32, 0.32, 0.32,
         0.62, 0.4,    0.32, 0.32, 0.32,
         0.7, 0.57,     0.32, 0.32, 0.32,
         0.7, 0.4,     0.32, 0.32, 0.32,
    };

    glBindVertexArray(bigVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bigVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
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

    glBindVertexArray(windowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(window_vertices), window_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void fillRoomVAO() {
    float room_vertices[] =
    {
         0.29, 0.03,   0.0, 0.0,
         0.41, 0.03,   1.0, 0.0,
         0.29, 0.21,   0.0, 1.0,
         0.41, 0.21,   1.0, 1.0,

         0.59, -0.27,  0.0, 0.0, 
         0.71, -0.27,  1.0, 0.0,
         0.59, -0.09,  0.0, 1.0,
         0.71, -0.09,  1.0, 1.0,
    };

    glBindVertexArray(roomVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roomVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(room_vertices), room_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void fillRoomBgVAO() {
    float room_vertices[] =
    {
         0.29, -0.27,   
         0.41, -0.27,
         0.29, -0.09,
         0.41, -0.09,   

         0.59, -0.27,  
         0.71, -0.27, 
         0.59, -0.09, 
         0.71, -0.09,

         0.29, 0.03,
         0.41, 0.03,
         0.29, 0.21,
         0.41, 0.21,

         0.59, 0.03,    
         0.71, 0.03, 
         0.59, 0.21,    
         0.71, 0.21,
    };

    glBindVertexArray(roomBgVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roomBgVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(room_vertices), room_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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
    glBindVertexArray(fenceVAO);
    glBindBuffer(GL_ARRAY_BUFFER, fenceVBO);
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
    glBindVertexArray(treeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, treeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tree_vertices), tree_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void fillNameVAO() {
    float name_vertices[] =
    {   //X    Y              S    T 
        -1.0, -0.72,         0.0, 1.0,//gore levo
        -0.5, -0.72,         1.0, 1.0, //gore desno
        -1.0, -0.83,         0.0, 0.0, //dole levo
        -0.5, -0.83,         1.0, 0.0, //dole desno
    };
    glBindVertexArray(nameVAO);
    glBindBuffer(GL_ARRAY_BUFFER, nameVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(name_vertices), name_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void fillDogVAO() {
    float dog_vertices[] =
    {   //X    Y            S    T 
        -0.35, -0.17,        1.0, 1.0,//gore desno
        -0.55, -0.17,        0.0, 1.0, //gore levo
        -0.35, -0.53,       1.0, 0.0, //dole desno
        -0.55, -0.53,       0.0, 0.0, //dole levo
    };

    glBindVertexArray(dogVAO);
    glBindBuffer(GL_ARRAY_BUFFER, dogVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dog_vertices), dog_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void bindTexture(unsigned int shader, unsigned texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(texture);
    unsigned uTexLoc2 = glGetUniformLocation(shader, "uTex");
    glUniform1i(uTexLoc2, 0);
    glUseProgram(0);
}

void dogFeeding(float positionX, float positionY) {
    if (!isEating){
        if (!isNight) {
            foodX = positionX;
            isEating = true;
            feedingTime = glfwGetTime() + 7;
        }
        else if (!foodWaiting) {
            foodX = positionX;
            foodWaiting = true;
        }
    }
}

bool isMouseOverGrass(double mouseX, double mouseY) {
    float grassX = -1.0, grassY = -1.0;             // Donji levi ugao trave
    float grassWidth = 1.15f, grassHeight = 1.25f;  // Dimenzije trave

    return mouseX >= grassX && mouseX <= grassX + grassWidth &&
        mouseY >= grassY && mouseY <= grassY + grassHeight;
}

void mouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizacija kursora u OpenGL koordinatni sistem
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        mouseX = (mouseX / windowWidth) * 2.0 - 1.0;
        mouseY = -((mouseY / windowHeight) * 2.0 - 1.0); // Inverzija osi Y

        if (isMouseOverGrass(mouseX, mouseY)) {
            isFoodPresent = true; // Hrana se sada pojavljuje
            foodPosX = mouseX;
            foodPosY = mouseY;
            dogFeeding(foodPosX, foodPosY);
        }
    }
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
    unsigned char* data = stbi_load("res/bone.png", &width, &height, &channels, 4);

    if (data) {
        GLFWimage cursorImage;
        cursorImage.width = width;
        cursorImage.height = height;
        cursorImage.pixels = data;

        boneCursor = glfwCreateCursor(&cursorImage, 0, 0);
        glfwSetCursor(window, boneCursor);

        stbi_image_free(data);
    }
    else {
        std::cerr << "Failed to load cursor image!" << std::endl;
    }
}


