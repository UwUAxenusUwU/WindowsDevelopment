#include<windows.h>
#include<cstdio>
#include"resource.h"

CONST CHAR* values[] = { "This", "is", "my","first","Combo","Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{ 
		HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
		for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)values[i]);
		}
		SendMessage(hCombo, CB_SETCURSEL, 0, 0);
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
			INT i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);	//taking index of choosed element
			if (i == CB_ERR) MessageBox(hwnd, "At first made your choice!", "Attention!", MB_OK | MB_ICONWARNING);
			else
			{
				CONST INT SIZE = 256;
				CHAR sz_BUFFER[SIZE]{};			//contents string readed from Combo box
				CHAR sz_MESSAGE[SIZE]{};		//contents final message to output message box
				SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)sz_BUFFER);
				sprintf(sz_MESSAGE, "You choosed case #%i, that countains '%s'", i, sz_BUFFER);
				MessageBox(hwnd, sz_MESSAGE, "Your choice", MB_OK | MB_ICONINFORMATION);
			}
		}
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}