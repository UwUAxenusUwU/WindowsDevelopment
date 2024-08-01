#include<windows.h>
#include<iostream>
#include"resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//MessageBox(NULL, "Hello, World!\nThis is message window, babe!", "Hello, WinAPI!", MB_YESNOCANCEL | MB_ICONQUESTION);
	//std::cout << typeid(char).name() << std::endl;
	//std::cout << typeid(wchar_t).name() << std::endl;

	//hIncstance - ��� *.exe ������ ����� ���������
	//hPrevInst - ���� �������� ������������� � ��������������� ���� IA-16, IA-32 (win98) �� ������� ��� �� ��������, �������� ��� �������� ��������������;
	//lpCmdLine - long pointer to command prompt - ��� ��������� ������, ��������� � ������� WinMain ��� ������� ��������� 
	//  lpCmdLine ������������ ����� ������ �����, ������� ��������� �������� ������ �������� ������ ���� � exe �����

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//hwnd - handler to window (���������� ����)
	switch (uMsg)
	{
		//WM_ - windows message
	case WM_INITDIALOG:		//������������ ������ 1 ��� ��� �������� ����
		{//H - Handler (����������, ����������)
			HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_PINKY_PIE));
			SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
			//SendMessage(����, ���������, ��������� ���������);

			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			
			//SetFocus(hEditLogin);

			break;
		}
	case WM_COMMAND:		//����� �������������� ������� �� ������, ���� ������ � ����� ��������� ��������� ����
		switch (LOWORD(wParam))
		{
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
			MessageBox(hwnd, "Was presses OK-button!", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL:
			EndDialog(hwnd, 0); 
			break;
		}
		break;
	case WM_CLOSE:			//������������ ��� ������� �� ������ �������
		EndDialog(hwnd, 0);
	}
	return false;
}