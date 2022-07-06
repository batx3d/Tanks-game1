#include "TXLib.h"

void setColor(int r, int g, int b) {
    txSetColor(RGB(r, g, b));
    txSetFillColor(RGB(r, g, b));
}

void drawButton(int x, int y, int x2, int y2, char nameButton[])
{
    setColor(105, 105, 105);  //gray
    txRectangle(x, y, x2, y2);
    setColor(128, 0, 0);
    txDrawText(x, y, x2, y2, nameButton);
}

void drawPanel(int width, int height)
{
    setColor(255, 255, 255);
    txRectangle(0, 0, width, height);
}

void drawImage(int x, int y, int width, int height, HDC image) {
    txBitBlt(txDC(), x, y, width, height, image);
}

void cleanField(int x, int y, int x2, int y2) {
    setColor(0, 0, 0);
    txRectangle(x, y, x2, y2);
}

void drawLabel(int x, int y, int x2, int y2, char text[])
{
    setColor(255, 255, 255);  //white
    txRectangle(x, y, x2, y2);
    setColor(0, 0, 0);
    txDrawText(x, y, x2, y2, text);
}
