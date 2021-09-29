// ClassTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;
#pragma region crt
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void GotoXY(short X, short Y)
{
    COORD cp;
    cp.X = X;
    cp.Y = Y;
    SetConsoleCursorPosition(hStdOut, cp);
}

void SetTextColor(short Color)
{
    CONSOLE_SCREEN_BUFFER_INFO inf;
    Color = Color & 0x0F;
    GetConsoleScreenBufferInfo(hStdOut, &inf);
    inf.wAttributes = inf.wAttributes & 0xF0;
    inf.wAttributes = inf.wAttributes | Color;
    SetConsoleTextAttribute(hStdOut, inf.wAttributes);
}

void SetTextBackground(short Color)
{
    CONSOLE_SCREEN_BUFFER_INFO inf;
    Color = Color & 0x0F;
    GetConsoleScreenBufferInfo(hStdOut, &inf);
    inf.wAttributes = inf.wAttributes & 0x0F;
    inf.wAttributes = inf.wAttributes | (Color << 4);
    SetConsoleTextAttribute(hStdOut, inf.wAttributes);
}

void HideCursor()
{
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(hStdOut, &structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &structCursorInfo);
}

#pragma endregion

class SimplePoint
{
    int x;
    int y;
    bool visible; // true - видима , false - скрыта
    static int consoleWidth;
    static int consoleHeight;
public:
    SimplePoint(int x, int y, bool show = true)// : x(x), y(y), visible(show)
    {
        this->x = x;
        this->y = y;
        visible = show;
        InitConsoleSize();
        if (show) Show();
    }

    SimplePoint() : SimplePoint(0, 0, false)
    {

    }

    ~SimplePoint()
    {
        if (visible) Hide();
    }

    void Show()
    {
        SetVisibility(true);
    }

    void Hide()
    {
        SetVisibility(false);
    }

    void SetVisibility(bool visibility)
    {
        Draw(visibility);
        visible = visibility;
    }

    void Move(int newX, int newY)
    {
        bool v = visible;
        if (visible == true) Hide();
        x = newX;
        y = newY;
        if (v) Show();
    }

    void MoveRelative(int dx, int dy)
    {
        Move(x + dx, y + dy);
    }

    bool GetVisibility()
    {
        return visible;
    }


    virtual void Draw(bool show)
    {
        /*if (show)
            SetTextColor(15);
        else
            SetTextColor(0);*/

        show ? SetTextColor(15) : SetTextColor(0);
        if (show)
            PutChar(x, y, '*');
        else
            PutChar(x, y, ' ');
    }

    int GetX()
    {
        return x;
    }

    int GetY()
    {
        return y;
    }

    static void InitConsoleSize()
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hStdOut, &consoleInfo))
        {
            consoleWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
            consoleWidth = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
        }
        else
        {
            consoleWidth = 80;
            consoleWidth = 25;

        }
    }

protected:
    void PutChar(int x, int y, char c)
    {
        if (x < 0 || x > consoleWidth
            || y < 0 || y > consoleHeight)
            return;
        GotoXY(x, y);
        cout << c;
    }


};

int SimplePoint::consoleWidth = 80;
int SimplePoint::consoleHeight = 25;

class ColorPoint : public SimplePoint
{
    int color;

public:
    ColorPoint(int x, int y, int color = 15,
        bool show = true)
    {
        Move(x, y);
        this->color = color;
        if (show)
            Show();
    }

    void Draw(bool show) override
    {
        show ? SetTextColor(color) : SetTextColor(0);
        if (show)
            PutChar(GetX(), GetY(), '*');
        else
            PutChar(GetX(), GetY(), ' ');
    }

    int GetColor()
    {
        return color;
    }

    void SetColor(int newColor)
    {
        color = newColor;
        if (GetVisibility()) Draw(true);
    }
};


class MyRectangle : ColorPoint
{
    int width, height;
public:
    MyRectangle(int x, int y, int width,
        int height, int color = 15,
        bool show = false) : ColorPoint(x, y, color, false)
    {
        this->width = width;
        this->height = height;
        SetColor(color);
        SetVisibility(show);
    }

    ~MyRectangle()
    {
        if (GetVisibility()) Hide();
    }

    void Draw(bool show) override
    {
        if (show)
            DrawRectangle(GetX(), GetY(), width, height, GetColor());
        else
            DrawRectangle(GetX(), GetY(), width, height, 0);
    }
    
    int getWidth() 
    {
        return width;
    }

    int getHeight()
    {
        return height;
    }

    void Grow(int width, int height)
    {
        if (GetVisibility()) Hide();

        this->width = width;
        this->height = height;

        //GetVisibility() ? 
        
    }

private:
    void DrawRectangle(int x, int y, int width, int height, int color)
    {
        GotoXY(x, y);
        SetTextColor(color);

        for (int i = 0; i < height; i++)
        {
            GotoXY(x, y + i);

            for (int k = 0; k < width; k++)
            {
                if ((i == 0 && k == 0) || (i == 0 && k == width - 1) || (i == height - 1 && k == 0) || (i == height - 1 && k == width - 1))
                {
                    cout << "*";
                }
                else if (i == 0 || i == height - 1)
                {
                    cout << "**";
                }
                else if (k == 0 || k == width - 1)
                {
                    cout << "*";
                }
                else
                {
                    cout << "  ";
                }
            }
        }

    }
};



int main()
{
    SimplePoint::InitConsoleSize();
    ColorPoint point(10, 10, 12);
    MyRectangle rect(11, 11, 5, 5, 11, true);
    rect.Grow()

        
    _getch();
    

}

