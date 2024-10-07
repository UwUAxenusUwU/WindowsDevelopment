#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <chrono>

void ClickWindow(HWND hwnd, int x, int y) {
    // Координаты клика передаются в LPARAM (x и y).
    LPARAM lParam = MAKELPARAM(x, y);
    // Симулируем нажатие левой кнопки мыши
    SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % (60 - 10 + 1) + 60));
    // Симулируем отпускание левой кнопки мыши
    SendMessage(hwnd, WM_LBUTTONUP, 0, lParam);
}
HWND GetWindowByName(const std::wstring& windowName) 
{
    HWND hwnd = FindWindow(NULL, windowName.c_str());
    if (hwnd == NULL) {
        std::wcout << L"Не удалось найти окно: " << windowName << std::endl;
    }
    return hwnd;
}

HBITMAP CaptureWindow(HWND hwnd, int width, int height) {
    HDC hWindowDC = GetDC(hwnd);  // Получаем DC окна
    HDC hMemoryDC = CreateCompatibleDC(hWindowDC);

    HBITMAP hBitMap = CreateCompatibleBitmap(hWindowDC, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitMap);

    // Захватываем содержимое окна
    BitBlt(hMemoryDC, 0, 0, width, height, hWindowDC, 0, 0, SRCCOPY);

    hBitMap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

    DeleteDC(hMemoryDC);
    ReleaseDC(hwnd, hWindowDC);

    return hBitMap;
}

POINT FindSnowflake(HBITMAP hbmWindow, int width, int height)
{
    BITMAPINFOHEADER bmi = { 0 };
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biWidth = width;
    bmi.biHeight = -height; // отрицательное значение для переворота изображения
    bmi.biPlanes = 1;
    bmi.biBitCount = 32; // Используем 32 бита на пиксель
    bmi.biCompression = BI_RGB;

    // Буфер для хранения пикселей
    int imageSize = width * height * 4; // 4 байта на пиксель (32 бита)
    BYTE* pixels = new BYTE[imageSize];

    // Получаем пиксели из HBITMAP
    HDC hDC = GetDC(NULL);
    GetDIBits(hDC, hbmWindow, 0, height, pixels, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
    ReleaseDC(NULL, hDC);

    int randomChance = rand() % 1000 + 1; //шанс бомбы

    // Пробегаем по всем пикселям изображения
    for (int y = 250; y < height - 130; y++)
    {
        if (y <= 400 || y >= 565)
        {
            for (int x = 660; x < width - 700; x++)
            {
                int index = (x + y * width) * 4; // Индекс пикселя
                BYTE blue = pixels[index];
                BYTE green = pixels[index + 1];
                BYTE red = pixels[index + 2];

                // Условие поиска цвета (можно настроить диапазон)
                if ((red >= 150 && red <= 210 && green >= 200 && green <= 250 && blue >= 0 && blue <= 10 && (y > 680 || y < 400)) || (red >= 140 && red <= 150 && green >= 225 && green <= 235 && blue >= 240 && blue <= 250 && y > 680))
                {
                    delete[] pixels;
                    return POINT{ x + (rand() % (10 - 2 + 1) + 35) - (rand() % (10 - 2 + 1) + 45), y + (rand() % (10 - 2 + 1) + 5) }; // снежинка
                }
                if (red == 131 && green == 135 && blue == 136 && randomChance <= 20)
                {
                    delete[] pixels;
                    return POINT{ x + (rand() % (10 - 2 + 1) + 35) - (rand() % (10 - 2 + 1) + 25), y + (rand() % (10 - 2 + 1) + 35) - +(rand() % (10 - 2 + 1) + 35) }; // бомба
                }
                if (red >= 245 && red <= 255 && green >= 160 && green <= 180 && blue >= 170 && blue <= 190)
                {
                    delete[] pixels;
                    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % (1500 - 400 + 1) + 1000));
                    return POINT{ x, y }; // билет
                }
            }
        }
        else
            continue;
    }

    delete[] pixels;
    return POINT{ -1, -1 }; // Если ничего не найдено
}

int main() 
{
    std::wstring windowName = L"NoxPlayer1"; // window name
    HWND hwnd = GetWindowByName(windowName);

    if (hwnd == NULL) {
        std::wcout << L"Не удалось найти окно" << std::endl;
        return -1;
    }

    int screenWidth = 1980, screenHeight = 1024;

    // Задержка перед началом работы
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    while (true) {
        // Захватываем содержимое окна
        HBITMAP hBitmap = CaptureWindow(hwnd, screenWidth, screenHeight);

        // Ищем снежинку на экране
        POINT snowflake = FindSnowflake(hBitmap, screenWidth, screenHeight);

        // Если снежинка найдена, кликаем по ней
        if (snowflake.x != -1 && snowflake.y != -1) {
            ClickWindow(hwnd, snowflake.x, snowflake.y);
        }

        // Задержка перед следующим поиском снежинки
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % (100 - 35 + 1) + 40));

        DeleteObject(hBitmap);
    }

    return 0;
}

