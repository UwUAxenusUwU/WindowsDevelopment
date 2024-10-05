#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include<Richedit.h>
#include <iostream>
#include "resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "TextEditor";

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CHAR* FormatLastError();

BOOL LoadTextFileToEdit(HWND hEdit, LPSTR lpszFileName);
BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR lpszFileName);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));
	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	wClass.hIcon = (HICON)LoadImage(hInstance, "txt.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hIconSm = (HICON)LoadImage(hInstance, "txt.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	HBITMAP background = (HBITMAP)LoadImage(hInstance, "background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	wClass.hbrBackground = CreatePatternBrush(background);

	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed!", "Error", MB_OK || MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS,
		g_sz_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	std::cout << FormatLastError() << std::endl;
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed!", "Error", MB_OK || MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, hwnd, 0, NULL) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeConsole();
	return 0;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hRichEdit20 = LoadLibrary("riched20.dll");
	static CHAR szFileName[MAX_PATH] = "";
	static LPSTR lpstrFileText = NULL;
	static DWORD dwFileTextLen = 0;
	static BOOL textChanged = FALSE;
	switch (uMsg)
	{
	case WM_CREATE:
	{

		RECT clientRect;
		RECT windowRect;
		GetWindowRect(hwnd, &windowRect);
		GetClientRect(hwnd, &clientRect);
		std::cout << "Window: " << windowRect.left << "\t" << windowRect.top << "\t" << windowRect.right << "\t" << windowRect.bottom << std::endl;
		std::cout << "Client: " << clientRect.left << "\t" << clientRect.top << "\t" << clientRect.right << "\t" << clientRect.bottom << std::endl;
		HWND hEdit = CreateWindowEx
		(
			NULL, RICHEDIT_CLASS, "",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
			0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, hwnd, (HMENU)IDC_EDIT, NULL, NULL
		);
	}
	break;
	case WM_SIZE:
	{
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		MoveWindow(GetDlgItem(hwnd, IDC_EDIT), 10, 10, clientRect.right - 20, clientRect.bottom - 20, true);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
		{

			CHAR szFileName[MAX_PATH]{};

			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0C Plus Plus files: (*.cpp | *.h)\0*.cpp;*.h\0All files: (*.*)\0*.*\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = "txt";

			if (GetOpenFileName(&ofn))
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);


				DWORD dwCurrentTextLenght = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);

				LPSTR lpstrCurrentText = (LPSTR)GlobalAlloc(GPTR, dwCurrentTextLenght + 1);
				SendMessage(hEdit, WM_GETTEXT, dwCurrentTextLenght + 1, (LPARAM)lpstrCurrentText);
				BOOL fileWasChanged = TRUE;
				if(lpstrCurrentText && lpstrFileText) strcmp(lpstrCurrentText, lpstrFileText);
				GlobalFree(lpstrCurrentText);
				if (fileWasChanged)
				{
					switch (MessageBox(hwnd, "Save changes in file?", "File has been changed!", MB_YESNOCANCEL || MB_ICONQUESTION))
					{
					case IDYES: SendMessage(hwnd, WM_COMMAND, LOWORD(ID_FILE_SAVE), 0); break;
					case IDNO: 
					{
						LoadTextFileToEdit(hEdit, szFileName);
						GlobalFree(lpstrFileText);
						dwFileTextLen = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
						lpstrFileText = (LPSTR)GlobalAlloc(GPTR, dwFileTextLen + 1);
						SendMessage(hEdit, WM_GETTEXT, dwFileTextLen, (LPARAM)lpstrFileText);
					}
					case IDCANCEL: break;
					}
				}
			}
		}
		break;
		case ID_FILE_SAVE:
		{
			if (strlen(szFileName))
				SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName);
			else SendMessage(hwnd, WM_COMMAND, LOWORD(ID_FILE_SAVEAS), 0);
		}
		break;

		case ID_FILE_SAVEAS:
		{
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Text files: (*.txt)\0.txt\0C Plus Plus files (*.cpp | *.h)\0*.cpp;*.h\0All files: (*.*)\0*.*\0";
			ofn.lpstrDefExt = "txt";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&ofn))SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName);
		}
		break;
		case IDC_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				textChanged = TRUE;
			}
		}
		break;
		}
		break;
	}
	case WM_DESTROY:
		FreeLibrary(hRichEdit20);
		;;;;;;;;;		PostQuitMessage(0);
		break;
	case WM_CLOSE:

		DestroyWindow(hwnd);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	}
	return FALSE;
}

CHAR* FormatLastError()
{
	DWORD errorID = GetLastError();
	LPSTR lpszBuffer = NULL;
	DWORD dwSize = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_RUSSIAN_RUSSIA), (LPSTR)&lpszBuffer, 0, NULL);
	return lpszBuffer;
}

BOOL LoadTextFileToEdit(HWND hEdit, LPSTR lpszFileName)
{
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile
	(
		lpszFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		0
	);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != UINT_MAX)
		{
			LPSTR lpszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (lpszFileText)
			{
				DWORD dwRead;
				if (ReadFile(hFile, lpszFileText, dwFileSize, &dwRead, NULL))
				{
					if (SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)lpszFileText))
						bSuccess = TRUE;
				}
				GlobalFree(lpszFileText);
			}
			CloseHandle(hFile);
		}
	}
	return bSuccess;
}

BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR lpszFileName)
{
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
		if (dwTextLength)
		{
			LPSTR lpszText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
			if (lpszText)
			{
				if (SendMessage(hEdit, WM_GETTEXT, dwTextLength + 1, (LPARAM)lpszText))
				{
					DWORD dwWriten;
					if (WriteFile(hFile, lpszText, dwTextLength, &dwWriten, NULL)) bSuccess = TRUE;
				}
				GlobalFree(lpszText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}