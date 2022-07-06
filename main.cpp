#include "drawelements.h"
#include <iostream>
#include <ctime>
#include <math.h>
#include <string>

using namespace std;

const int TOP = 0;
const int RIGHT = 1;
const int BOTTOM = 2;
const int LEFT = 3;

const int WIDTH_TANK = 60;
const int HEIGHT_TANK = 60;

const int STEP_TANK = 5;
const int STEP_BULLET = 5;

const int WIDTH_OBJECT = 40;
const int HEIGHT_OBJECT = 40;

const int WIDTH_BULLET = 10;
const int HEIGHT_BULLET = 10;

const int WIDTH_WINDOW = 800;
const int HEIGHT_WINDOW = 600;

const int WIDTH_END = 230;
const int HEIGHT_END = 120;

struct Coordinats {
    int x, y;
};

const Coordinats ONE_COORDINATS_LABEL = {10, 140};
const Coordinats TWO_COORDINATS_LABEL = {80, 170};

struct RoutesTank {
    HDC tankTop;
    HDC tankRight;
    HDC tankBottom;
    HDC tankLeft;
};

struct Bullet {
    HDC pictures;
    bool status;
    int route;
    Coordinats coordinats;
};

struct Tank {
    bool status;
    int route;
    Coordinats coordinats;
};

void buttonListener();
Coordinats getNewCoordinatsObject(Coordinats);
void redrawingTank(Tank, RoutesTank);
void redrawingObject(Coordinats, HDC);
bool checkCoordinatsObjectAndTank(Coordinats, Coordinats);
void redrawingScore(int);
void openRegulations();
Coordinats getCoordinatsBullet(Tank);
void redrawingBullet(Bullet);
bool checkCoordinatsBulletAndObject(Coordinats, Coordinats);
void drawTheEnd(HDC);

int main()
{
    srand(time(NULL));

    txCreateWindow(WIDTH_WINDOW, HEIGHT_WINDOW);

    drawPanel(90, HEIGHT_WINDOW);
    drawButton(20, 10, 70, 40, "Start");
    drawButton(20, 60, 70, 90, "Exit");
    drawLabel(10, 110, 80, 140, "Score: ");
    //drawButton(10, 190, 80, 220, "Правила");

    buttonListener();

    return 0;
}


void buttonListener()
{
    RoutesTank routesTank = {txLoadImage("pictures/tanks/top_tank.bmp"), txLoadImage("pictures/tanks/right_tank.bmp"), txLoadImage("pictures/tanks/bottom_tank.bmp"),
                            txLoadImage("pictures/tanks/left_tank.bmp")};
    HDC object = txLoadImage("pictures/object.bmp");
    HDC theEnd = txLoadImage("pictures/the_end.bmp");
    //drawImage(200, 200, WIDTH_END, HEIGHT_END, theEnd);

    Tank tank = {false, TOP, {355, 300}};

    Bullet bullet;
    bullet.pictures = txLoadImage("pictures/bullet.bmp");
    bullet.status = false;

    Coordinats coordObject;

    int score = 0;

    while(true) {
        if(bullet.status) {
            cleanField(bullet.coordinats.x, bullet.coordinats.y, bullet.coordinats.x + WIDTH_BULLET, bullet.coordinats.y + HEIGHT_BULLET);

            switch(bullet.route) {
            case TOP:
                if(bullet.coordinats.y - STEP_BULLET > 0)
                    bullet.coordinats.y -= STEP_BULLET;

                else
                    bullet.status = false;

                break;
            case RIGHT:
                if(bullet.coordinats.x + STEP_BULLET < WIDTH_WINDOW)
                    bullet.coordinats.x += STEP_BULLET;

                else
                    bullet.status = false;

                    break;

            case BOTTOM:
                if(bullet.coordinats.y + STEP_BULLET < HEIGHT_WINDOW)
                    bullet.coordinats.y += STEP_BULLET;

                else
                    bullet.status = false;

                break;

            case LEFT:
                if(bullet.coordinats.x - STEP_BULLET > 90)
                    bullet.coordinats.x -= STEP_BULLET;

                else
                    bullet.status = false;

                    break;
            }

            //cleanField(bullet.coordinats.x, bullet.coordinats.y, bullet.coordinats.x + WIDTH_BULLET, bullet.coordinats.y + HEIGHT_BULLET);

            if(checkCoordinatsBulletAndObject(bullet.coordinats, coordObject)) {
                score++;
                bullet.status = false;
                redrawingScore(score);
                cleanField(coordObject.x, coordObject.y, coordObject.x + WIDTH_OBJECT, coordObject.y + HEIGHT_OBJECT);
                coordObject = getNewCoordinatsObject(tank.coordinats);
                redrawingObject(coordObject, object);
            }

            if(bullet.status)
                redrawingBullet(bullet);

            txSleep(5);
        }

        if(txMouseButtons() == 1)  //click left button on mouse
        {
            Coordinats posMouse = {txMouseX(), txMouseY()};

            if((posMouse.x >= 20 && posMouse.x <= 70) && (posMouse.y >= 60 && posMouse.y <= 90)) { //click button exit
                if(tank.status)
                    cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);

                break;
            }

            else if((posMouse.x >= 20 && posMouse.x <= 70) && (posMouse.y >= 10 && posMouse.y <= 40) && !tank.status) { //click button start
                tank.status = true;
                redrawingTank(tank, routesTank);
                coordObject = getNewCoordinatsObject(tank.coordinats);
                redrawingObject(coordObject, object);

                char scoreChars[7];
                sprintf(scoreChars, "%d", score);

                drawLabel(ONE_COORDINATS_LABEL.x, ONE_COORDINATS_LABEL.y, TWO_COORDINATS_LABEL.x, TWO_COORDINATS_LABEL.y, scoreChars);
            }
        }

        if(GetAsyncKeyState(38) && tank.status) { //top
            if((tank.route == TOP) && (tank.coordinats.y - STEP_TANK) >= 0) {
                cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);
                tank.coordinats.y -= STEP_TANK;
            }

            else if((tank.route == RIGHT) && (tank.coordinats.x + WIDTH_TANK + STEP_TANK) <= 800) {
                cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);
                tank.coordinats.x += STEP_TANK;
            }

            else if((tank.route == BOTTOM) && (tank.coordinats.y + WIDTH_TANK + STEP_TANK) <= 600) {
                cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);
                tank.coordinats.y += STEP_TANK;
            }

            else if((tank.route == LEFT) && (tank.coordinats.x - STEP_TANK) >= 90) {
                cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);
                tank.coordinats.x -= STEP_TANK;
            }

            if(checkCoordinatsObjectAndTank(coordObject, tank.coordinats)) {
                score = 0;
                redrawingScore(score);
                cleanField(coordObject.x, coordObject.y, coordObject.x + WIDTH_OBJECT, coordObject.y + HEIGHT_OBJECT);
                drawTheEnd(theEnd);
                coordObject = getNewCoordinatsObject(tank.coordinats);
                redrawingObject(coordObject, object);
            }

            redrawingTank(tank, routesTank);

            txSleep(5);
        }

        else if(GetAsyncKeyState(40) && tank.status) { //bottom
            if((tank.route == TOP) && (tank.coordinats.y + HEIGHT_TANK + STEP_TANK) <= 600) {
                cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);
                tank.coordinats.y += STEP_TANK;
            }

            else if((tank.route == RIGHT) && (tank.coordinats.x - STEP_TANK) >= 90) {
                cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);
                tank.coordinats.x -= STEP_TANK;
            }

            else if((tank.route == BOTTOM) && (tank.coordinats.y - STEP_TANK) >= 0) {
                cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);
                tank.coordinats.y -= STEP_TANK;
            }

            else if((tank.route == LEFT) && (tank.coordinats.x + STEP_TANK + WIDTH_TANK) <= 800) {
                cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);
                tank.coordinats.x += STEP_TANK;
            }

            if(checkCoordinatsObjectAndTank(coordObject, tank.coordinats)) {
                score = 0;
                redrawingScore(score);
                cleanField(coordObject.x, coordObject.y, coordObject.x + WIDTH_OBJECT, coordObject.y + HEIGHT_OBJECT);
                drawTheEnd(theEnd);
                coordObject = getNewCoordinatsObject(tank.coordinats);
                redrawingObject(coordObject, object);
            }

            redrawingTank(tank, routesTank);

            txSleep(5);
        }

        else if(GetAsyncKeyState(39) && tank.status) { //right
            tank.route++;

            if(tank.route == 4)
                tank.route = TOP;

            cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);
            redrawingTank(tank, routesTank);

            txSleep(100);
        }

        else if(GetAsyncKeyState(37) && tank.status) { //left
            tank.route--;

            if(tank.route == -1)
                tank.route = LEFT;

            cleanField(tank.coordinats.x, tank.coordinats.y, tank.coordinats.x + WIDTH_TANK, tank.coordinats.y + HEIGHT_TANK);
            redrawingTank(tank, routesTank);

            txSleep(100);
        }

        else if(GetAsyncKeyState(32) && tank.status && !bullet.status) {
            if(!bullet.status) {
                bullet.status = true;
                bullet.route = tank.route;
                bullet.coordinats = getCoordinatsBullet(tank);

                if(checkCoordinatsBulletAndObject(bullet.coordinats, coordObject)) {
                    score++;
                    bullet.status = false;
                    redrawingScore(score);
                    cleanField(coordObject.x, coordObject.y, coordObject.x + WIDTH_OBJECT, coordObject.y + HEIGHT_OBJECT);
                    coordObject = getNewCoordinatsObject(tank.coordinats);
                    redrawingObject(coordObject, object);
                }
            }

            txSleep(5);
        }
    }
}

Coordinats getCoordinatsBullet(Tank tank)
{
    Coordinats coordinatsBullet;
    int center = (WIDTH_TANK - WIDTH_BULLET) / 2;  //column pixels for tank's center

    switch(tank.route) {
    case TOP:
        coordinatsBullet.x = tank.coordinats.x + center;
        coordinatsBullet.y = tank.coordinats.y - WIDTH_BULLET;
        break;
    case RIGHT:
        coordinatsBullet.x = tank.coordinats.x + WIDTH_TANK;
        coordinatsBullet.y = tank.coordinats.y + center;
        break;
    case BOTTOM:
        coordinatsBullet.x = tank.coordinats.x + center;
        coordinatsBullet.y = tank.coordinats.y + HEIGHT_TANK;
        break;
    case LEFT:
        coordinatsBullet.x = tank.coordinats.x - WIDTH_BULLET;
        coordinatsBullet.y = tank.coordinats.y + center;
        break;
    }

    return coordinatsBullet;
}

void drawTheEnd(HDC pictures) {
    drawImage(300, 200, WIDTH_END, HEIGHT_END, pictures);
    txSleep(2000);
    cleanField(300, 200, 300 + WIDTH_END, 200 + HEIGHT_END);
}

void redrawingBullet(Bullet bullet) {
    drawImage(bullet.coordinats.x, bullet.coordinats.y, WIDTH_BULLET, HEIGHT_BULLET, bullet.pictures);
}

void redrawingScore(int score) {

    char scoreChars[7];
    sprintf(scoreChars, "%d", score);
    setColor(255, 255, 255);
    txRectangle(ONE_COORDINATS_LABEL.x, ONE_COORDINATS_LABEL.y, TWO_COORDINATS_LABEL.x, TWO_COORDINATS_LABEL.y);
    drawLabel(ONE_COORDINATS_LABEL.x, ONE_COORDINATS_LABEL.y, TWO_COORDINATS_LABEL.x, TWO_COORDINATS_LABEL.y, scoreChars);
}

void redrawingObject(Coordinats coordinats, HDC object) {
    drawImage(coordinats.x, coordinats.y, WIDTH_OBJECT, HEIGHT_OBJECT, object);
}

void redrawingTank(Tank tank, RoutesTank routesTank)
{
    switch(tank.route) {
    case TOP:
        drawImage(tank.coordinats.x, tank.coordinats.y, WIDTH_TANK, HEIGHT_TANK, routesTank.tankTop);
        break;
    case RIGHT:
        drawImage(tank.coordinats.x, tank.coordinats.y, WIDTH_TANK, HEIGHT_TANK, routesTank.tankRight);
        break;
    case BOTTOM:
        drawImage(tank.coordinats.x, tank.coordinats.y, WIDTH_TANK, HEIGHT_TANK, routesTank.tankBottom);
        break;
    case LEFT:
        drawImage(tank.coordinats.x, tank.coordinats.y, WIDTH_TANK, HEIGHT_TANK, routesTank.tankLeft);
        break;
    }
}

bool checkCoordinatsBulletAndObject(Coordinats coordinatsBullet, Coordinats coordinatsObject)
{
    int x1 = coordinatsBullet.x, y1 = coordinatsBullet.y, x2 = coordinatsObject.x, y2 = coordinatsObject.y;

    if(((x1 >= x2 && x1 <= x2 + WIDTH_OBJECT) && (y1 >= y2 && y1 <= y2 + HEIGHT_OBJECT)) ||
       ((x1 + WIDTH_BULLET >= x2 && x1 + WIDTH_BULLET <= x2 + WIDTH_OBJECT) && (y1 >= y2 && y1 <= y2 + HEIGHT_OBJECT)) ||
       ((x1 >= x2 && x1 <= x2 + WIDTH_OBJECT) && (y1 + HEIGHT_BULLET >= y2 && y1 + HEIGHT_BULLET <= y2 + HEIGHT_OBJECT)) ||
       ((x1 + WIDTH_BULLET >= x2 && x1 + WIDTH_BULLET <= x2 + WIDTH_OBJECT) && (y1 + HEIGHT_BULLET >= y2 && y1 + HEIGHT_BULLET <= y2 + HEIGHT_OBJECT)))
        return true;
    else
        return false;
}

bool checkCoordinatsObjectAndTank(Coordinats coordObject, Coordinats coordinats)
{
    int x1 = coordObject.x, y1 = coordObject.y, x2 = coordinats.x, y2 = coordinats.y;

    if(((x1 >= x2 && x1 <= x2 + WIDTH_TANK) && (y1 >= y2 && y1 <= y2 + HEIGHT_TANK)) ||
       ((x1 + WIDTH_OBJECT >= x2 && x1 + WIDTH_OBJECT <= x2 + WIDTH_TANK) && (y1 >= y2 && y1 <= y2 + HEIGHT_TANK)) ||
       ((x1 >= x2 && x1 <= x2 + WIDTH_TANK) && (y1 + HEIGHT_OBJECT >= y2 && y1 + HEIGHT_OBJECT <= y2 + HEIGHT_TANK)) ||
       ((x1 + WIDTH_OBJECT >= x2 && x1 + WIDTH_OBJECT <= x2 + WIDTH_TANK) && (y1 + HEIGHT_OBJECT >= y2 && y1 + HEIGHT_OBJECT <= y2 + HEIGHT_TANK)))
        return true;
    else
        return false;
}

Coordinats getNewCoordinatsObject(Coordinats coordinatsTank)
{
    Coordinats coordinats;
    int distance;

    do {
        coordinats.x = 90 + rand() % (WIDTH_WINDOW - WIDTH_OBJECT - 90);
        coordinats.y = 1 + rand() % (HEIGHT_WINDOW - HEIGHT_OBJECT);
        distance = (int) sqrt(pow((double) (((coordinats.x * 2 + WIDTH_OBJECT) / 2) - ((coordinatsTank.x * 2 + WIDTH_TANK) / 2)), 2) + pow((double) (((coordinats.y * 2 + HEIGHT_OBJECT) / 2) - ((coordinatsTank.y * 2 + HEIGHT_TANK) / 2)), 2));
    }
    while(distance < 50);

    return coordinats;
}
