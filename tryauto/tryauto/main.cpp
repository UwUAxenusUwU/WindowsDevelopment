#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>

using namespace std;

void SetCursorPosition(int x, int y)
{
	SetCursorPos(x, y);
}

void Click(int x, int y)
{
	SetCursorPosition(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
	this_thread::sleep_for(chrono::milliseconds(rand()%(60-10 + 1)+60));
	mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
}

HBITMAP CaptureScreen(int width, int height)
{
	HDC hScreenDC = GetDC(NULL);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	HBITMAP hBitMap = CreateCompatibleBitmap(hScreenDC, width, height);

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitMap);
	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
	hBitMap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

	DeleteDC(hMemoryDC);
	ReleaseDC(NULL, hScreenDC);

	return hBitMap;
}
POINT FindSnowflake(HBITMAP hBitmap, int width, int height)
{
    BITMAPINFOHEADER bmi = { 0 };
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biWidth = width;
    bmi.biHeight = -height; // ������������� �������� ��� ���������� �����������
    bmi.biPlanes = 1;
    bmi.biBitCount = 32; // ���������� 32 ���� �� �������
    bmi.biCompression = BI_RGB;

    // ����� ��� �������� ��������
    int imageSize = width * height * 4; // 4 ����� �� ������� (32 ����)
    BYTE* pixels = new BYTE[imageSize];

    // �������� ������� �� HBITMAP
    HDC hDC = GetDC(NULL);
    GetDIBits(hDC, hBitmap, 0, height, pixels, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
    ReleaseDC(NULL, hDC);

    int randomChance = rand() % 1000 + 1; //���� �����
    // ��������� �� ���� �������� �����������
    for (int y = 250; y < height-130; y++) 
    {
        if (y <= 400 || y >= 565)
        {
            for (int x = 660; x < width - 700; x++)
            {
                int index = (x + y * width) * 4; // ������ �������
                BYTE blue = pixels[index];
                BYTE green = pixels[index + 1];
                BYTE red = pixels[index + 2];

                // ������� ������ ����� (����� ��������� ��������)
                if ((red >= 150 && red <= 210 && green >= 200 && green <= 250 && blue >= 0 && blue <= 10 && (y > 680 || y < 400)) || (red >= 140 && red <= 150 && green >= 225 && green <= 235 && blue >= 240 && blue <= 250 && y>680))
                {
                    delete[] pixels;
                    return POINT{ x + (rand() % (10 - 2 + 1) + 35) - (rand() % (10 - 2 + 1) + 45), y +(rand() % (10 - 2 + 1) + 5) }; // ��������
                }
                if (red == 131 && green == 135 && blue == 136 && randomChance <= 20)
                {
                    delete[] pixels;
                    return POINT{ x + (rand() % (10 - 2 + 1) + 35) - (rand() % (10 - 2 + 1) + 25), y + (rand() % (10 - 2 + 1) + 35) - +(rand() % (10 - 2 + 1) + 35) }; // �����
                }
                if (red >= 245 && red <= 255 && green >= 160 && green <= 180 && blue >= 170 && blue <= 190)
                {
                    delete[] pixels;
                    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % (1500 - 400 + 1) + 1000));
                    return POINT{ x, y }; // �����
                }
            }
        }
        else 
            continue;
    }

    delete[] pixels;
    return POINT{ -1, -1 }; // ���� ������ �� �������
}

void main()
{
    //auto lastFoundTime = std::chrono::steady_clock::now();
    //const int TIMEOUT_MS = 10000; // ����-��� � n ������
    this_thread::sleep_for(std::chrono::milliseconds(3000));
    while (true) 
    {
        int screenWidth = 1980, screenHeight = 1024;

        // ����������� �������� ������
        HBITMAP hBitmap = CaptureScreen(screenWidth, screenHeight);

        // ���� �������� �� ������
        POINT snowflake = FindSnowflake(hBitmap, screenWidth, screenHeight);

        // ���� �������� �������, ������� �� ���
        if (snowflake.x != -1 && snowflake.y != -1) 
        {
            //cout << "�������� �������! ������ �� �����������: " << snowflake.x << ", " << snowflake.y << endl;
            Click(snowflake.x, snowflake.y);
            auto lastFoundTime = std::chrono::steady_clock::now(); // ����� ���������� ���������� ��������
        }
        //else 
        //{

            //auto currentTime = std::chrono::steady_clock::now();
            //auto timeSinceLastFound = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFoundTime).count();

            //cout << "�������� �� �������" << endl;

            //if (timeSinceLastFound >= TIMEOUT_MS) 
            //{
            //    // ���� ������ ����� n ������, ������� � ������������ �������
            //    cout << "�� �������" << endl;
            //    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % (150 - 40 + 1) + 1000));
            //    Click(955, 600); // ���� � �������
            //    cout << TIMEOUT_MS << endl;
            //    lastFoundTime = currentTime; // ��������� ������, ����� ���� �� �������� ������ ������
            //    timeSinceLastFound = 0;
            //}
            //Click(950+rand()%(30-10)+40, 590+rand() % (30 - 10) + 40);
            //std::this_thread::sleep_for(std::chrono::milliseconds(rand() % (150 - 40 + 1) + 1000));
        //}

        // �������� ����� ��������� ������� ��������
        std::this_thread::sleep_for(std::chrono::milliseconds(rand()%(100-35+1)+40));
        DeleteObject(hBitmap);
    }
}