#define _CRT_SECURE_NO_WARNINGS

#define CRES 30 // Circle Resolution = Rezolucija kruga

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);

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

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }
    unsigned int srbShader = createShader("basic.vert", "srb.frag");
    unsigned int windowShader = createShader("basic.vert", "window.frag");
    unsigned int japShader = createShader("basic.vert", "jap.frag");

    unsigned int VAO[2]; 
    glGenVertexArrays(2, VAO);
    unsigned int VBO[2];
    glGenBuffers(2, VBO);

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

         0.29, -0.27,   1, 1, 1,
         0.41, -0.27,   1, 1, 1,
         0.29, -0.09,   1, 1, 1,
         0.41, -0.09,   1, 1, 1,

         0.58, -0.28,   0, 0, 0,
         0.72, -0.28,   0, 0, 0,
         0.58, -0.08,   0, 0, 0,
         0.72, -0.08,   0, 0, 0,

         0.59, -0.27,   1, 1, 1,
         0.71, -0.27,   1, 1, 1,
         0.59, -0.09,   1, 1, 1,
         0.71, -0.09,   1, 1, 1,

         0.28, 0.02,   0, 0, 0,
         0.42, 0.02,   0, 0, 0,
         0.28, 0.22,   0, 0, 0,
         0.42, 0.22,   0, 0, 0,

         0.29, 0.03,   1, 1, 1,
         0.41, 0.03,   1, 1, 1,
         0.29, 0.21,   1, 1, 1,
         0.41, 0.21,   1, 1, 1,

         0.58, 0.02,   0, 0, 0,
         0.72, 0.02,   0, 0, 0,
         0.58, 0.22,   0, 0, 0,
         0.72, 0.22,   0, 0, 0,

         0.59, 0.03,   1, 1, 1,
         0.71, 0.03,   1, 1, 1,
         0.59, 0.21,   1, 1, 1,
         0.71, 0.21,   1, 1, 1,

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

    //Jap
    /*
    Generisanje tjemena kruga po jednacini za kruznicu, koju cemo crtati sa GL_TRIANGLE_FAN primitivom:
    Trebace nam bar X i Y coordinate, posto je boja u fragment sejderu
    Treba nam 2 * CRES brojeva za X i Y koordinate, gdje je CRES zapravo broj tjemena na kruznici (CRES 6 = sestougao)
    Pored toga nam trebaju jos dva tjemena - centar i ponovljeno tjeme ugla 0 (da bi se krug pravilno zatvorio)
    */
    //float circle[(CRES + 2) * 2];
    //float r = 0.5; //poluprecnik

    //circle[0] = 0; //Centar X0
    //circle[1] = 0; //Centar Y0
    //int i;
    //for (i = 0; i <= CRES; i++)
    //{
    //     
    //    circle[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 360 / CRES)); //Xi (Matematicke funkcije rade sa radijanima)
    //    circle[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
    //}

    //glBindVertexArray(VAO[1]);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int uTransparency = glGetUniformLocation(windowShader, "uTransparency");
    float transparency = 1.0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        glClear(GL_COLOR_BUFFER_BIT);

        //glViewport(0, 0, wWidth / 2, wHeight); // Da crtamo na lijevoj polovini ekrana
        glUseProgram(srbShader);

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            transparency = 0.5;  // Poluprovidno staklo
        }

        // Ako je pritisnuto taster N, postavi providnost na 1.0f
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        {
            transparency = 1.0f;  // Neporvidno staklo
        }


        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 40, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 48, 3);




        glUseProgram(windowShader);

        glBindVertexArray(VAO[0]);
        glUniform1f(uTransparency, transparency);

        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 44, 4);



        //glLineWidth(4.0);
        //GL_LINE_LOOP za linije

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
