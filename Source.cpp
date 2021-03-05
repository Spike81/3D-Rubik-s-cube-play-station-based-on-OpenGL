/**

@Author: Tianlei.Shi
@Date: 6th Jan., 2021
@Description: 
The modelling and simulation of a 3D Rubik's Cube Play Station for CPT205-CW3, 
which uses geometric modeling, texture, keyboard interaction and other graphic technologies, 
as well as C++ technologies such as pseudorandom number generation and time acquisition.

**/

#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include<stdio.h>
#include <time.h>
#define random(a,b) (rand()%(b-a)+a)  // generate random numbers to control the rotation of rubik's cube

time_t second_cube, word_seconds, value_seconds = 0;  // record time to control rubik's cube and screen
int value_word;  // time recoder to control screen
int Cube[6][10];  // cube
int xro[28], yro[28], zro[28];  // layers of cube
int start_flag = 0, first_in = 1, process_flag = 0, forbid = 0, solved = 0;  // control state flag of rubik's cube
int done = 0, di = 1, rot = 0, yes = 1, k = 0;
float angle = 0.0;  // control rotation of cube
int rotate_x = -25, rotate_y = 35;  // view angle
int pause = -1, pressed = 0, solving = 0;  // state flag of rubik's cube
int tmin = 0, tsec = 0, tmil = 0;  // minute, second, millisecond
char tim_record[17] = { '0',' ','0',' ',':',' ','0',' ','0',' ',':',' ','0',' ','0',' ','\0' };  // record time of solving
int dir_y, lay_y, dir_x, lay_x, dir_z, lay_z;  // parameter to control rotation


void selectFont(int size, int charset, const char* face)
{
    /**
     * @brief Sets the font size, color, font, etc to be drawn
     * @param size    size of font
     * @param charset   type of font
     * @param face  font
     */

    glColor3f(1.0, 0.498, 0.314);  // set color of font

    // set font
    HFONT
        hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0, charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
            , DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
    HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
    DeleteObject(hOldFont);
}

void drawString(const char* str)
{
    /**
     * @brief draw words
     * @param str   words that will be drawn
     */

    static int isFirstCall = 1;
    static GLuint lists;

    if (isFirstCall)
    {
        isFirstCall = 0;
        lists = glGenLists(128);
        wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);
    }
    for (; *str != '\0'; ++str)
    {
        glCallList(lists + *str);
    }
}

void rmy(int fr, int le, int ba, int ri, int dir)
{
    /**
     * @brief exchange cube info to ratote
     */

    int temp;
    if (dir == 1)
    {
        temp = Cube[4][ri];
        Cube[4][ri] = Cube[3][ba];
        Cube[3][ba] = Cube[5][le];
        Cube[5][le] = Cube[1][fr];
        Cube[1][fr] = temp;
    }
    else
    {
        temp = Cube[4][ri];
        Cube[4][ri] = Cube[1][fr];
        Cube[1][fr] = Cube[5][le];
        Cube[5][le] = Cube[3][ba];
        Cube[3][ba] = temp;

    }
}

void rmz(int up, int ri, int dw, int le, int dir)
{
    /**
     * @brief exchange cube info to ratote
     */

    int temp;
    if (dir == 1)
    {
        temp = Cube[4][ri];
        Cube[4][ri] = Cube[2][up];
        Cube[2][up] = Cube[5][le];
        Cube[5][le] = Cube[0][dw];
        Cube[0][dw] = temp;
    }
    else
    {
        temp = Cube[4][ri];
        Cube[4][ri] = Cube[0][dw];
        Cube[0][dw] = Cube[5][le];
        Cube[5][le] = Cube[2][up];
        Cube[2][up] = temp;

    }
}

void fr(int face)
{
    /**
     * @brief exchange cube info to rotate
     * @param face    face of cube
     */

    int temp;

    // the even position
    temp = Cube[face][2];
    Cube[face][2] = Cube[face][4];
    Cube[face][4] = Cube[face][8];
    Cube[face][8] = Cube[face][6];
    Cube[face][6] = temp;

    // the odd position
    temp = Cube[face][1];
    Cube[face][1] = Cube[face][7];
    Cube[face][7] = Cube[face][9];
    Cube[face][9] = Cube[face][3];
    Cube[face][3] = temp;
}

void fran(int face)
{
    /**
     * @brief exchange cube info to ratote
     * @param face    face of cube
     */

    int temp;

    // the even position
    temp = Cube[face][2];
    Cube[face][2] = Cube[face][6];
    Cube[face][6] = Cube[face][8];
    Cube[face][8] = Cube[face][4];
    Cube[face][4] = temp;

    // the odd position
    temp = Cube[face][1];
    Cube[face][1] = Cube[face][3];
    Cube[face][3] = Cube[face][9];
    Cube[face][9] = Cube[face][7];
    Cube[face][7] = temp;
}

void rotation_x_layer(int lay, int dir)
{
    /**
     * @brief control rotation of x axis
     * @param lay    layer number (1, 2, 3)
     * @param dir   direction of rotation (-1, 1)
     */

     // rotate corresponding layer according lay
    int i, j, temp, temp_1, temp_2, temp_3;
    if (dir == -1)
    {
        temp = lay;
        temp_1 = Cube[0][lay];
        temp_2 = Cube[0][lay + 3];
        temp_3 = Cube[0][lay + 6];
        for (j = 1; j < 4; j++)
            for (i = 0, lay = temp; i < 3; i++, lay += 3)
                Cube[j - 1][lay] = Cube[j][lay];
        lay = temp;
        Cube[3][lay] = temp_1;
        Cube[3][lay + 3] = temp_2;
        Cube[3][lay + 6] = temp_3;
        if (temp == 3)
            fran(4);
        if (temp == 1)
            fran(5);

    }
    else
    {
        temp = lay;
        temp_1 = Cube[3][lay];
        temp_2 = Cube[3][lay + 3];
        temp_3 = Cube[3][lay + 6];
        for (j = 3; j > 0; j--)
            for (i = 0, lay = temp; i < 3; i++, lay += 3)
                Cube[j][lay] = Cube[j - 1][lay];
        lay = temp;
        Cube[0][lay] = temp_1;
        Cube[0][lay + 3] = temp_2;
        Cube[0][lay + 6] = temp_3;
        if (temp == 3)
            fr(4);
        if (temp == 1)
            fr(5);
    }
}

void rotation_y_layer(int lay, int dir)
{
    /**
     * @brief control rotation of y axis
     * @param lay    layer number (1, 2, 3)
     * @param dir   direction of rotation (-1, 1)
     */

     // rotate corresponding layer according lay
    if (lay == 1)
    {
        rmy(1, 3, 9, 1, dir);
        rmy(2, 2, 8, 2, dir);
        rmy(3, 1, 7, 3, dir);
        if (dir == 1)
            fr(2);
        if (dir == -1)
            fran(2);
    }
    if (lay == 2)
    {
        rmy(4, 6, 6, 4, dir);
        rmy(5, 5, 5, 5, dir);
        rmy(6, 4, 4, 6, dir);
    }
    if (lay == 3)
    {
        rmy(7, 9, 3, 7, dir);
        rmy(8, 8, 2, 8, dir);
        rmy(9, 7, 1, 9, dir);
        if (dir == 1)
            fran(0);
        else
            fr(0);
    }
}

void rotation_z_layer(int lay, int dir)
{
    /**
     * @brief control rotation of z axis
     * @param lay    layer number (1, 2, 3)
     * @param dir   direction of rotation (-1, 1)
     */

    // rotate corresponding layer according lay
    if (lay == 1)
    {
        rmz(7, 1, 3, 7, dir);
        rmz(8, 4, 2, 4, dir);
        rmz(9, 7, 1, 1, dir);
        if (dir == 1)
            fr(1);
        else
            fran(1);
    }
    if (lay == 2)
    {
        rmz(4, 2, 6, 8, dir);
        rmz(5, 5, 5, 5, dir);
        rmz(6, 8, 4, 2, dir);
    }
    if (lay == 3)
    {
        rmz(1, 3, 9, 9, dir);
        rmz(2, 6, 8, 6, dir);
        rmz(3, 9, 7, 3, dir);
        if (dir == 1)
            fran(3);
        else
            fr(3);
    }
}

void change_point(int w, int h)
{
    /**
     * @brief change the view point
     * @param w parameter of changing wigth
     * @param h parameter of changing hight
     */

    glViewport(0, 0, w, h);
}

void scramble(void)
{
    /**
     * @brief Random scrambles Rubik's Cube
     */

    int i;
    for (i = 0; i < 20; i++)  // scrambles 20 times
    {
        switch (rand() % 19)  // generates a random number less than 19
        {
            // according to the generated random number, to carry out the corresponding rotation
            case 1:rotation_x_layer(3, -1); break;
            case 2:rotation_x_layer(2, -1); break;
            case 3:rotation_x_layer(1, -1); break;
            case 4:rotation_x_layer(1, 1); break;
            case 5:rotation_x_layer(3, 1); break;
            case 6:rotation_x_layer(2, 1); break;

            case 7:rotation_y_layer(3, -1); break;
            case 8:rotation_y_layer(2, -1); break;
            case 9:rotation_y_layer(1, -1); break;
            case 10:rotation_y_layer(1, 1); break;
            case 11:rotation_y_layer(3, 1); break;
            case 12:rotation_y_layer(2, 1); break;

            case 13:rotation_z_layer(3, -1); break;
            case 14:rotation_z_layer(2, -1); break;
            case 15:rotation_z_layer(1, -1); break;
            case 16:rotation_z_layer(1, 1); break;
            case 17:rotation_z_layer(3, 1); break;
            case 18:rotation_z_layer(2, 1); break;

            default:break;
        }
    }
}

void fin_y(int fa)
{
    /**
     * @brief control single layer rotation of y axis
     * @param fa    factor of rotation
     */

    // calculate result according to factor
    dir_y = di = 2 * (fa / 6) - 1;
    lay_y = (fa - 1) % 3 + 1;

    // update cube according to result
    int i;
    for (i = 1; i < 10; i++)
        yro[(lay_y - 1) * 9 + i] = 1;
    rot = 1;
}

void fin_x(int fa)
{
    /**
     * @brief control single layer rotation of x axis
     * @param fa    factor of rotation
     */

     // calculate result according to factor
    dir_x = di = 2 * (fa / 6) - 1;
    lay_x = (fa - 1) % 3 + 1;

    // update cube according to result
    if (lay_x == 1)
    {
        xro[7] = 1;
        xro[8] = 1;
        xro[9] = 1;
        xro[16] = 1;
        xro[17] = 1;
        xro[18] = 1;
        xro[25] = 1;
        xro[26] = 1;
        xro[27] = 1;
    }
    if (lay_x == 2)
    {
        xro[4] = 1;
        xro[5] = 1;
        xro[6] = 1;
        xro[13] = 1;
        xro[14] = 1;
        xro[15] = 1;
        xro[22] = 1;
        xro[23] = 1;
        xro[24] = 1;
    }
    if (lay_x == 3)
    {
        xro[1] = 1;
        xro[2] = 1;
        xro[3] = 1;
        xro[10] = 1;
        xro[11] = 1;
        xro[12] = 1;
        xro[19] = 1;
        xro[20] = 1;
        xro[21] = 1;
    }

    rot = 2;
}

void fin_z(int fa)
{
    /**
     * @brief control single layer rotation of z axis
     * @param fa    factor of rotation
     */

    // calculate result according to factor
    dir_z = -1 * (di = 2 * (fa / 6) - 1);
    lay_z = (fa - 1) % 3 + 1;

    // update cube according to result
    if (lay_z == 1)
    {
        zro[9] = 1;
        zro[6] = 1;
        zro[3] = 1;
        zro[18] = 1;
        zro[15] = 1;
        zro[12] = 1;
        zro[21] = 1;
        zro[24] = 1;
        zro[27] = 1;
    }
    if (lay_z == 2)
    {
        zro[2] = 1;
        zro[5] = 1;
        zro[8] = 1;
        zro[11] = 1;
        zro[14] = 1;
        zro[17] = 1;
        zro[20] = 1;
        zro[23] = 1;
        zro[26] = 1;
    }
    if (lay_z == 3)
    {
        zro[1] = 1;
        zro[4] = 1;
        zro[7] = 1;
        zro[10] = 1;
        zro[13] = 1;
        zro[16] = 1;
        zro[19] = 1;
        zro[22] = 1;
        zro[25] = 1;
    }

    rot = 3;
}

void wholr(int ax, int dir)
{
    /**
     * @brief update rotation parameter according to ax and dir
     * @param ax   factor to calculate rotation parameter, (0, 1, 2)
     * @param dir   direction of rotation parameter, (-1, 1)
     */

    int i;

    di = dir;
    for (i = 0; i < 28; i++)
    {
        // judge the axis
        if (ax == 1)
            xro[i] = 1;
        else if (ax == 0)
            yro[i] = 1;
        else
            zro[i] = 1;
    }

    rot = 10 + 5 * ax + dir;  // update rotation parameter
}

void keyboard(unsigned char key, int x, int y)
{
    /**
     * @brief interaction by keyboard
     * @param key   key that pressed
     */

    int i, j;
    pressed = 1;

    if (start_flag == 1)  // valid if the game starts
    {

        if (yes && pause != 1 && !solved)  // is solved, cannot rotation
        {
            // interaction according keys
            switch (key)
            {
                // rotate single layer
                case 'x':fin_x(1); yes = 0; break;
                case 'c':fin_x(2); yes = 0; break;
                case 'v':fin_x(3); yes = 0; break;
                case 'w':fin_x(7); yes = 0; break;
                case 'e':fin_x(8); yes = 0; break;
                case 'r':fin_x(9); yes = 0; break;
                case 't':fin_y(1); yes = 0; break;
                case 'g':fin_y(2); yes = 0; break;
                case 'b':fin_y(3); yes = 0; break;
                case 'q':fin_y(7); yes = 0; break;
                case 'a':fin_y(8); yes = 0; break;
                case 'z':fin_y(9); yes = 0; break;
                case 'm':fin_z(1); yes = 0; break;
                case 'j':fin_z(2); yes = 0; break;
                case 'u':fin_z(3); yes = 0; break;
                case 'n':fin_z(7); yes = 0; break;
                case 'h':fin_z(8); yes = 0; break;
                case 'y':fin_z(9); yes = 0; break;

                default:break;
            }
        }

        // whatever solve, below key can use
        switch (key)
        {
            // rotate whole cube
            case 's':wholr(2, 1); yes = 0; break;
            case 'f':wholr(2, -1); yes = 0; break;

            case 'i':
                // scramble cube
                scramble();
                pause = -1;
                solving = 1;
                pressed = 0;
                process_flag = 1;

                // time to 0
                tmin = 0; 
                tmil = 0; 
                tsec = 0;

                // update state flag
                solved = 0;
                forbid = 0;
                break;

            case 'p':
                // pause
                if (solving)
                    pause = (-1) * pause;
                break;

            case 'o':
                // solve
                pressed = 0;
                solving = 0;
                pause = -1;
                for (i = 0; i < 6; i++)
                    for (j = 0; j < 10; j++)
                        Cube[i][j] = i;
                for (i = 0; i < 28; i++)
                {
                    xro[i] = 0;
                    yro[i] = 0;
                    zro[i] = 0;
                }

                // time to 0
                tmin = 0; 
                tmil = 0; 
                tsec = 0;
                break;
        }
    }

    // whatever any time, below keys can use
    switch (key)
    {
        case 27:exit(0); break;  // Esc to exit

        case 13:
            // Enter to start or end
            if (start_flag == 1)  // if start, end
            {
                start_flag = 0;
                pressed = 0;
                solving = 0;
                pause = -1;
                for (i = 0; i < 6; i++)
                    for (j = 0; j < 10; j++)
                        Cube[i][j] = i;
                for (i = 0; i < 28; i++)
                {
                    xro[i] = 0;
                    yro[i] = 0;
                    zro[i] = 0;
                }

                // time to 0
                tmin = 0; 
                tmil = 0; 
                tsec = 0;
                
                // update state flag
                solved = 0;
                process_flag = 0;
                forbid = 0;
            }
            else if (start_flag == 0)  // if end, start
                start_flag = 1;
            break;
    }

}

void special_key(int key, int x, int y)
{
    /**
     * @brief interaction with special key, to rotate whole cube
     * @param key   key that pressed
     */

    if (start_flag == 1)  // judge if game start
    {
        if (yes)
        {
            // interaction according key
            if (key == GLUT_KEY_RIGHT) {
                wholr(0, -1);
            }

            else if (key == GLUT_KEY_LEFT) {
                wholr(0, 1);
            }

            else if (key == GLUT_KEY_UP) {
                wholr(1, 1);
            }

            else if (key == GLUT_KEY_DOWN) {
                wholr(1, -1);
            }
        }
    }
}

void timer(int t)
{
    /**
     * @brief control time and loop to draw
     */

    second_cube = time(NULL);  // record time, one change per second

    // random rotate cube
    if (start_flag == 0 && second_cube > value_seconds)
    {
        int radom = random(1, 6);

        switch (radom)
        {
        case 1: wholr(0, -1); break;
        case 2: wholr(0, 1); break;
        case 3: wholr(1, 1); break;
        case 4: wholr(1, -1); break;
        case 5: wholr(2, 1); break;
        case 6: wholr(2, -1); break;
        }
        value_seconds = second_cube;  // update time
    }

    // loop call
    glutPostRedisplay();
    glutTimerFunc(15, timer, 1);
}

void setcol(int f)
{
    /**
     * @brief set color for each face
     * @param l   layer that need to color
     */

    switch (f)
    {
        // according f, set color
        case 0:glColor3f(1.0, 1.0, 1.0); break;
        case 1:glColor3f(0.0, 0.0, 1.0); break;
        case 2:glColor3f(1.0, 1.0, 0.0); break;
        case 3:glColor3f(0.0, 1.0, 0.0); break;
        case 4:glColor3f(1.0, 0.0, 0.0); break;
        case 5:glColor3f(1.0, 0.5, 0.0); break;
        case 6:glColor3f(0.1, 0.0, 0.0); break;
        default:break;
    }
}
//___________________________________________________ ____(SINGLE) CUBE
void mycube(int pic, float siz, float x, float y, float z, int d, int f, int u, int b, int r, int l)
{
    /**
     * @brief The small cubes are constructed based on the parameters passed in
     */

    siz /= 2;

    glPushMatrix();

    // set the rotation
    if (yro[pic])
        glRotatef(angle * di, 0.0, 1.0, 0.0);
    if (xro[pic])
        glRotatef(angle * di, 1.0, 0.0, 0.0);
    if (zro[pic])
        glRotatef(angle * di, 0.0, 0.0, 1.0);

    glTranslatef(x, y, z);

    // modelling 6 face of single cube
    glBegin(GL_POLYGON);
    setcol(d);  // set color
    glVertex3f(siz, -siz, siz);
    glVertex3f(siz, -siz, -siz);
    glVertex3f(-siz, -siz, -siz);
    glVertex3f(-siz, -siz, siz);
    glEnd();

    glBegin(GL_POLYGON);
    setcol(b);
    glVertex3f(siz, siz, siz);
    glVertex3f(siz, -siz, siz);
    glVertex3f(-siz, -siz, siz);
    glVertex3f(-siz, siz, siz);
    glEnd();

    glBegin(GL_POLYGON);
    setcol(u);
    glVertex3f(siz, siz, siz);
    glVertex3f(siz, siz, -siz);
    glVertex3f(-siz, siz, -siz);
    glVertex3f(-siz, siz, siz);
    glEnd();

    glBegin(GL_POLYGON);
    setcol(f);
    glVertex3f(siz, siz, -siz);
    glVertex3f(siz, -siz, -siz);
    glVertex3f(-siz, -siz, -siz);
    glVertex3f(-siz, siz, -siz);
    glEnd();

    glBegin(GL_POLYGON);
    setcol(r);
    glVertex3f(siz, siz, siz);
    glVertex3f(siz, -siz, siz);
    glVertex3f(siz, -siz, -siz);
    glVertex3f(siz, siz, -siz);
    glEnd();

    glBegin(GL_POLYGON);
    setcol(l);
    glVertex3f(-siz, siz, siz);
    glVertex3f(-siz, -siz, siz);
    glVertex3f(-siz, -siz, -siz);
    glVertex3f(-siz, siz, -siz);
    glEnd();

    glPopMatrix();
}

void rubic(float siz, float x)
{
    /**
     * @brief use 26 small cubes to build Rubik's Cube
     */

    GLfloat diffuse[] = { 1.0f, 0.5f, 0.2f, 1.0f, };
    GLfloat ambient[] = { 1.0f, 0.0f, 0.0f, 1.0f, };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f, };

    // the top layer, 9 cubes
    mycube(1, siz, x, x, x, 6, 6, Cube[2][3], Cube[3][9], Cube[4][3], 6);
    mycube(2, siz, x, x, 0, 6, 6, Cube[2][6], 6, Cube[4][2], 6);
    mycube(3, siz, x, x, -x, 6, Cube[1][3], Cube[2][9], 6, Cube[4][1], 6);

    mycube(4, siz, 0, x, x, 6, 6, Cube[2][2], Cube[3][8], 6, 6);
    mycube(5, siz, 0, x, 0, 6, 6, Cube[2][5], 6, 6, 6);
    mycube(6, siz, 0, x, -x, 6, Cube[1][2], Cube[2][8], 6, 6, 6);

    mycube(7, siz, -x, x, x, 6, 6, Cube[2][1], Cube[3][7], 6, Cube[5][3]);
    mycube(8, siz, -x, x, 0, 6, 6, Cube[2][4], 6, 6, Cube[5][2]);
    mycube(9, siz, -x, x, -x, 6, Cube[1][1], Cube[2][7], 6, 6, Cube[5][1]);

    // the middle layer, 8 cubes, the central cube (14) no need to modelling
    mycube(10, siz, x, 0, x, 6, 6, 6, Cube[3][6], Cube[4][6], 6);
    mycube(11, siz, x, 0, 0, 6, 6, 6, 6, Cube[4][5], 6);
    mycube(12, siz, x, 0, -x, 6, Cube[1][6], 6, 6, Cube[4][4], 6);

    mycube(13, siz, 0, 0, x, 6, 6, 6, Cube[3][5], 6, 6);
    mycube(15, siz, 0, 0, -x, 6, Cube[1][5], 6, 6, 6, 6);

    mycube(16, siz, -x, 0, x, 6, 6, 6, Cube[3][4], 6, Cube[5][6]);
    mycube(17, siz, -x, 0, 0, 6, 6, 6, 6, 6, Cube[5][5]);
    mycube(18, siz, -x, 0, -x, 6, Cube[1][4], 6, 6, 6, Cube[5][4]);
    
    // the bottom layer, 9 cubes
    mycube(19, siz, x, -x, x, Cube[0][9], 6, 6, Cube[3][3], Cube[4][9], 6);
    mycube(20, siz, x, -x, 0, Cube[0][6], 6, 6, 6, Cube[4][8], 6);
    mycube(21, siz, x, -x, -x, Cube[0][3], Cube[1][9], 6, 6, Cube[4][7], 6);

    mycube(22, siz, 0, -x, x, Cube[0][8], 6, 6, Cube[3][2], 6, 6);
    mycube(23, siz, 0, -x, 0, Cube[0][5], 6, 6, 6, 6, 6);
    mycube(24, siz, 0, -x, -x, Cube[0][2], Cube[1][8], 6, 6, 6, 6);

    mycube(25, siz, -x, -x, x, Cube[0][7], 6, 6, Cube[3][1], 6, Cube[5][9]);
    mycube(26, siz, -x, -x, 0, Cube[0][4], 6, 6, 6, 6, Cube[5][8]);
    mycube(27, siz, -x, -x, -x, Cube[0][1], Cube[1][7], 6, 6, 6, Cube[5][7]);
}

void drawboard(void)
{
    /**
     * @brief draw screen by lines
     */

    glLoadIdentity();
    glColor3f(0.827, 0.827, 0.827);  // set screen color
    glBegin(GL_LINES);  // draw by lines

    // draw lines
    for (float x = -0.7; x <= 0.7; x+=0.002)
    {
        glVertex3f(x, 0.8, 0);
        glVertex3f(x, 0.5, 0);
    }

    glEnd();
}

void drawWord(void)
{
    /**
     * @brief draw words on screen
     */

    glLoadIdentity();

    int i, j, flag = 0;

    if (start_flag == 0)  // is game start?
    {
        // if not start yet
        if (first_in == 1)  // record the first-in time
        {
            word_seconds = time(NULL);
            value_word = word_seconds;
            first_in = 0;
        }

        time_t temp_time = time(NULL);

        // there are 4 statements, each displayed 3 second, and loop
        if (temp_time <= word_seconds + 3)
        {
            selectFont(50, ANSI_CHARSET, "Tahoma");
            glRasterPos3f(-0.4, 0.6, 0);
            drawString("Welcome to try");
        }
        else if (temp_time > word_seconds + 3 && temp_time <= word_seconds + 6)
        {
            selectFont(50, ANSI_CHARSET, "Tahoma");
            glRasterPos2f(-0.4, 0.6);
            drawString("Rubik's Cube !!!");
        }
        else if (temp_time > word_seconds + 6 && temp_time <= word_seconds + 9)
        {
            selectFont(50, ANSI_CHARSET, "Tahoma");
            glRasterPos2f(-0.5, 0.6);
            drawString("One dollar per play");
        }
        else if (temp_time > word_seconds + 9 && temp_time <= word_seconds + 12)
        {
            selectFont(50, ANSI_CHARSET, "Tahoma");
            glRasterPos2f(-0.6, 0.6);
            drawString("Press ENTER to start !!!");
        }
        else if (temp_time > word_seconds + 15)  // wait 3 second, start next loop
        {
            first_in = 1;
        }
    }
    else  // if game start
    {
        // if game in pause state
        if (pause == 1)
        {
            selectFont(50, ANSI_CHARSET, "Tahoma");
            glRasterPos2f(-0.5, 0.6);
            drawString("Press p to continue");
        }
        else 
        {
            // record the time to solve
            if (pressed && solving) 
            {
                tmin += 2;
                tsec += tmin / 100;
                tmil += tsec / 60;
                tmin %= 100;
                tsec %= 60;
                tim_record[2] = 48 + tmil;
                tim_record[6] = 48 + tsec / 10;
                tim_record[8] = 48 + tsec % 10;
                tim_record[12] = 48 + tmin / 10;
                tim_record[14] = 48 + tmin % 10;
            }
            
            // control print
            if (process_flag == 0)
            {
                selectFont(50, ANSI_CHARSET, "Tahoma");
                glRasterPos2f(-0.5, 0.6);
                drawString("Press i to Scramble");
            }

            // print time when playing
            if (process_flag == 1 && forbid == 0)
            {
                selectFont(50, ANSI_CHARSET, "Tahoma");
                glRasterPos2f(-0.35, 0.6);
                drawString(tim_record);
            }
        }

        // judge if solved
        for (i = 0; i < 6; i++)
            for (j = 1; j < 10; j++)
            {
                if (Cube[i][5] != Cube[i][j])
                {
                    flag = 1;
                }
            }

        // print solved and time
        if (!flag && process_flag == 1) {
            pressed = 0;
            solving = 0;
            pause = -1;

            selectFont(50, ANSI_CHARSET, "Tahoma");
            glRasterPos2f(-0.6, 0.6);
            drawString("Solved");

            glRasterPos2f(-0.15, 0.6);
            drawString(tim_record);

            // update control flag
            solved = 1;
            forbid = 1;
        }
    }
}

void draw_play_station(void)
{
    /**
     * @brief draw all objects in this project
     */

    int i, j;
    float siz = 0.2, x, sp = .02;  // size of cube
    x = siz + sp;

    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // set the view point
    glRotatef(rotate_x, 1.0, 0.0, 0.0);
    glRotatef(rotate_y, 0.0, 1.0, 0.0);

    glTranslatef(0.0, -0.3, 0.0);  // translation

    // control the rotation
    if (rot)
    {
        angle += 10;
        if (angle > 90)
        {
            angle = 0;
            for (i = 0; i < 28; i++)
            {
                xro[i] = 0;
                yro[i] = 0;
                zro[i] = 0;
            }

            // according to the rotation parameter to control the rotation of the Rubik's cube
            if (rot == 1)
                rotation_y_layer(lay_y, dir_y);
            if (rot == 2)
                rotation_x_layer(lay_x, dir_x);
            if (rot == 3)
                rotation_z_layer(lay_z, dir_z);
            if (rot == 9)
            {
                rotation_y_layer(1, -1);
                rotation_y_layer(2, -1);
                rotation_y_layer(3, -1);
            }
            if (rot == 11)
            {
                rotation_y_layer(1, 1);
                rotation_y_layer(2, 1);
                rotation_y_layer(3, 1);
            }
            if (rot == 14)
            {
                rotation_x_layer(1, -1);
                rotation_x_layer(2, -1);
                rotation_x_layer(3, -1);
            }
            if (rot == 16)
            {
                rotation_x_layer(1, 1);
                rotation_x_layer(2, 1);
                rotation_x_layer(3, 1);
            }
            if (rot == 19)
            {
                rotation_z_layer(1, 1);
                rotation_z_layer(2, 1);
                rotation_z_layer(3, 1);
            }
            if (rot == 21)
            {
                rotation_z_layer(1, -1);
                rotation_z_layer(2, -1);
                rotation_z_layer(3, -1);
            }

            // update process control parameter
            rot = 0;
            yes = 1;
        }
    }

    rubic(siz, x);  // re-draw the cube

    drawWord();  // draw word in screen
    drawboard();  // draw screen

    glFlush();
    glutSwapBuffers();  // display
}

void init(void)
{
    /**
     * @brief Initialize lighting and other settings
     */

    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);

    glShadeModel(GL_SMOOTH);   // set shade model

    // preset light and materials
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // set diffuse
    GLfloat light_ambient[] = { 0.0f, 0.5f, 0.5f, 1.0f };   // set ambient
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    // set material reflective
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };  // specular reflection parameter
    GLfloat mat_shininess[] = { 50.0 };               // specular parameter

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // make the light source effective
    glEnable(GL_LIGHT0);    // activate the light symbol constant

    // activate depth test
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv)
{
    /**
     * @brief main function, to start of whole program
     */

    // add colour for each layer of cube
    int i, j;
    for (i = 0; i < 6; i++)
        for (j = 0; j < 10; j++)
            Cube[i][j] = i;
    for (i = 0; i < 28; i++)
    {
        xro[i] = 0;
        yro[i] = 0;
        zro[i] = 0;
    }

    // print statements on console
    printf("\n\n\nInstruction guide:\n");
    printf("\n\n\tStart/End: ENTER\t\tScramble: i\t\tPause/Continue: p\t\tReset/Solve: o\n\n\n\n\t\tw\te\tr\n\n\n\tq\t\t\t\tt\t\ty\tu\t\t\t¡ü\n\n\n\ta\t\t\t\tg\t\th\tj\t\ts\t\tf\n\n\n\tz\t\t\t\tb\t\tn\tm\t\t¡û\t¡ý\t¡ú\n\n\n\t\tx\tc\tv\n\n\n");
    
    // start whole progarm
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);  // size of window
    glutInitWindowPosition(660, 0);  // position of window
    glutCreateWindow("3D Rubik's cube play station");  // name of window

    glClearColor(0.69, 0.878, 0.9, 0.1);  // colour of background
   
    glEnable(GL_DEPTH_TEST);  // open the depth test

    // call methods
    glutDisplayFunc(draw_play_station);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(100, timer, 1);
    glutSpecialFunc(special_key);
    glutReshapeFunc(change_point);
    init();

    glutMainLoop();  // loop to draw
    return 0;
}
