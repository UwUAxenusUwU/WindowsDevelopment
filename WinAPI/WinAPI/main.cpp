#include<windows.h>
#include<iostream>
#include"resource.h"
#include"commctrl.h"

CONST CHAR g_sz_INVENTATION[] = "Enter your name";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//MessageBox(NULL, "Hello, World!\nThis is message window, babe!", "Hello, WinAPI!", MB_YESNOCANCEL | MB_ICONQUESTION);
	//std::cout << typeid(char).name() << std::endl;
	//std::cout << typeid(wchar_t).name() << std::endl;

	//hIncstance - это *.exe модуль нашей программы
	//hPrevInst - этот параметр использовался в комбинированном коде IA-16, IA-32 (win98) на сегодня уже не актуален, оставлен для обратной совместисмости;
	//lpCmdLine - long pointer to command prompt - это командная строка, передаётся в функцию WinMain при запуске программы 
	//  lpCmdLine представляет собой массив строк, нулевым элементом которого всегда является полный путь к exe файлу

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//hwnd - handler to window (дискриптор окна)
	switch (uMsg)
	{
		//WM_ - windows message
	case WM_INITDIALOG:		//отрабатывает только 1 раз при создании окна
		{//H - Handler (обработчик, дискриптор)
			HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_PINKY_PIE));
			SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
			//SendMessage(окно, сообщение, параметры сообщения);

			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_INVENTATION);
			
			//CONST INT TIP_SIZE = 20;
			//wchar_t tip[TIP_SIZE]{};


			//SetFocus(hEditLogin);

			break;
		}
	case WM_COMMAND:		//здесь обрабатываются нажатия на кнопки, ввод текста и любые изменения состояния окна
		switch (LOWORD(wParam))
		{
		case IDC_EDIT_LOGIN:
		{
				CONST INT SIZE = 256;
				CHAR sz_buffer[SIZE]{};
				HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
				SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (HIWORD(wParam) == EN_SETFOCUS && (strcmp(sz_buffer, g_sz_INVENTATION) == 0))
				SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)"");
			if (HIWORD(wParam) == EN_KILLFOCUS && (strcmp(sz_buffer, "") == 0))
				SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_INVENTATION);
		}
			break;
		case IDC_BUTTON_COPY:
			{	
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};

			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			}
			break;
		case IDOK:
			MessageBox(hwnd, "Nothing happened", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL:
			EndDialog(hwnd, 0); 
			break;
		}
		break;
	case WM_CLOSE:			//отрабатывает при нажатии на кнопку закрыть
		EndDialog(hwnd, 0);
	}
	return false;
}
