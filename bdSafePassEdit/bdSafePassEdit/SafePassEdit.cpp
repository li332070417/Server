#include "StdAfx.h"
#include <time.h>
#include <winternl.h>
#include "AES.h"
#include ".\safepassedit.h"

#define ISKEYDOWN(_x_) (GetKeyState(_x_) & 0x80)
#define ISKEYTOGGLED(_x_) (GetKeyState(_x_) & 0x81)
#define HF_ANSI            0x0002
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }

INT WINAPI GetRolHashEx(LPCSTR lpApiString);
FARPROC WINAPI MyGetProcAddressByHashEx(HMODULE hModule, INT hash);
HHOOK MyNtUserSetWindowsHookEx( HANDLE hmod, PUNICODE_STRING pstrLib, DWORD idThread, int nFilterType, HOOKPROC pfnFilterProc, DWORD dwFlags);

typedef VOID
(NTAPI* PRtlInitUnicodeString) (
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
	);

CSafePassEdit::CSafePassEdit(void)
: m_hCurWnd(NULL)
{
	if (m_AES == NULL) {
		m_AES = new AES("\x12\x43\xBD\xAE\xCD\x2B\xFF\xCC\xCF\x40\x52\x16\x07\x23\xFC\x22");
	}
	m_dwPassLen = 0;
	m_szPass = m_AES->Cipher(_T(""));
}

CSafePassEdit::~CSafePassEdit(void)
{
	m_lockInstances.Lock();
	m_pInstances.RemoveKey(m_hCurWnd);
	const DWORD nLength = m_pInstances.GetSize();
	m_lockInstances.Unlock();

	if (0 == nLength) {
		WaitForSingleObject(m_hHookThread, 1000*20);
		WaitForSingleObject(m_hKBDThread, 1000*20);
		UnhookWindowsHookEx(m_hDebugHook);
		UnhookWindowsHookEx(m_hLLKBDHook);
		m_hHookThread = NULL;
		m_hKBDThread = NULL;
		m_hDebugHook = NULL;
		m_hLLKBDHook = NULL;	
	}

	SAFE_DELETE(m_AES);
}

DWORD WINAPI CSafePassEdit::KBDThreadProc( LPVOID param ) {
	MSG msg;
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	srand(time(NULL));

	while (true) {

		m_lockInstances.Lock();
		const DWORD nLength = m_pInstances.GetSize();
		m_lockInstances.Unlock();
		if (nLength<=0)
			break;

		Sleep(500);
		CSafePassEdit* pInst = NULL;
		BOOL bFound = FALSE;

		//DWORD proccessID = 0;
		//const DWORD threadID = ::GetWindowThreadProcessId(::GetForegroundWindow(), &proccessID);
		//if (proccessID != ::GetCurrentProcessId())
		//{
		//	continue;
		//}

		//AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(), NULL), GetCurrentThreadId(), TRUE);
		//bFound = m_pInstances.Lookup(::GetFocus(), pInst);
		//AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(), NULL), GetCurrentThreadId(), FALSE);
		if(m_pFocusEdit) bFound = m_pInstances.Lookup(m_pFocusEdit->GetSafeHwnd(), pInst); 

		if (m_bHooking == TRUE && bFound == TRUE) {
			InputKey(rand() % 25 + 65);
		}
	}
	return 0;
}

DWORD WINAPI CSafePassEdit::HookThreadProc( LPVOID param ) {
	MSG msg = {0};
	DWORD i = 0;

	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	m_hLLKBDHook = MySetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, NULL, NULL);
	m_hKBDThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)KBDThreadProc, NULL, 0, NULL);
	while (true) {
		m_lockInstances.Lock();
		const DWORD nLength = m_pInstances.GetSize();
		m_lockInstances.Unlock();
		if (nLength<=0)
			break;

		if (::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (i >= 50) {
			CSafePassEdit* pInst = NULL;
			BOOL bFound = FALSE;
			
			//DWORD proccessID = 0;
			//const DWORD threadID = ::GetWindowThreadProcessId(::GetForegroundWindow(), &proccessID);
			//if (proccessID != ::GetCurrentProcessId())
			//{
			//	i++;
			//	::Sleep(1);
			//	continue;
			//}

			//::AttachThreadInput(threadID, ::GetCurrentThreadId(), TRUE);
			//bFound = m_pInstances.Lookup(::GetFocus(), pInst);
			//::AttachThreadInput(threadID, ::GetCurrentThreadId(), FALSE);
			if(m_pFocusEdit) bFound = m_pInstances.Lookup(m_pFocusEdit->GetSafeHwnd(), pInst); 
			
			if (TRUE == bFound)
			{
				m_bHooking = FALSE;
				::UnhookWindowsHookEx(m_hDebugHook);
				::UnhookWindowsHookEx(m_hLLKBDHook);
				m_hDebugHook = MySetWindowsHookEx(WH_DEBUG, (HOOKPROC)DebugProc, NULL, ::GetCurrentThreadId());
				m_hLLKBDHook = MySetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, ::GetModuleHandle(NULL), NULL);
				m_bHooking = TRUE;
 			}
			i = 0;
		}
		i++;
		Sleep(1);
	}
	return 0;
}

LRESULT CALLBACK CSafePassEdit::DebugProc( int nCode, WPARAM wParam, LPARAM lParam ) {
	if (nCode >= 0) {
		return 1;
	}
	return 0;
}

LRESULT CALLBACK CSafePassEdit::LowLevelKeyboardProc( int nCode, WPARAM wParam, PKBDLLHOOKSTRUCT lParam ) {
	CSafePassEdit* pInst;
	BOOL bFound = FALSE;

	//AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(), NULL), GetCurrentThreadId(), TRUE);
	//bFound = m_pInstances.Lookup(::GetFocus(), pInst);
	//AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(), NULL), GetCurrentThreadId(), FALSE);
	if(m_pFocusEdit) bFound = m_pInstances.Lookup(m_pFocusEdit->GetSafeHwnd(), pInst); 

	if (bFound == TRUE) {
		if (wParam == WM_KEYDOWN) {
			//AfxTrace("%x  %x  %x\r", lParam->scanCode, lParam->flags, lParam->dwExtraInfo);
			if (lParam->vkCode == VK_BACK || lParam->vkCode == VK_DELETE) {
				return InputChar(pInst, 0, lParam->vkCode);
			} else if (lParam->flags == 1 || lParam->flags == 0) {
				return InputChar(pInst, 0, lParam->vkCode);
			} else {
				return 0;
			}
		}
	}
	return 0;
}

LRESULT CSafePassEdit::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if (nMsg == EM_SETSEL && wParam != lParam) {
		return 0;
	} else if(nMsg == WM_MOUSEMOVE || nMsg == WM_RBUTTONDOWN || nMsg == WM_RBUTTONUP || nMsg == WM_LBUTTONDBLCLK) {
		return 0;
	} else if(nMsg == WM_CHAR) {
		if (lParam != 0) {
			return 0;
			//CEdit::OnChar(_T('?'), nRepCnt, nFlags);
		}
	} else if(WM_SETFOCUS == nMsg) {
		lParam = (LPARAM)::GetCurrentProcessId();
	}
	return CWnd::DefWindowProc(nMsg, wParam, lParam);
}

BOOL CSafePassEdit::InputChar(CHAR chChar, UINT uVKey) {
	return InputChar(this, chChar, uVKey);
}

BOOL CSafePassEdit::InputChar(CSafePassEdit* pInst, CHAR chChar, UINT uVKey) {
	if (uVKey == VK_BACK) {
		DWORD dwSel;
		dwSel = LOWORD(pInst->GetSel());
		if (pInst->m_dwPassLen > 0 && dwSel > 0) {
			CString strTmp;
			strTmp = m_AES->InvCipher(pInst->m_szPass);
			strTmp.Delete(dwSel - 1, 1);
			pInst->m_szPass = m_AES->Cipher(strTmp);
			strTmp.Empty();
			pInst->m_dwPassLen--;
			pInst->SendMessage(WM_CHAR, VK_BACK, 0);
		}
		return TRUE;
	} else if (uVKey == VK_DELETE) {
		DWORD dwSel;
		dwSel = LOWORD(pInst->GetSel());
		if (dwSel != pInst->GetWindowTextLength()) {
			if (pInst->m_dwPassLen > 0) {
				pInst->SendMessage(WM_KEYDOWN, VK_DELETE, 0);
				pInst->SendMessage(WM_KEYUP, VK_DELETE, 0);
				CString strTmp;
				strTmp = m_AES->InvCipher(pInst->m_szPass);
				strTmp.Delete(dwSel, 1);
				pInst->m_szPass = m_AES->Cipher(strTmp);
				strTmp.Empty();
				pInst->m_dwPassLen--;
			}
		}
		return TRUE;
	} else if (chChar == 0) {
		if (pInst->GetLimitText() > pInst->m_dwPassLen) {
			TCHAR cChar;
			cChar = VKey2Char(uVKey);
			if (cChar != 0) {
				pInst->SendMessage(WM_CHAR, _T('?'), 0);
				CString strTmp;
				strTmp = m_AES->InvCipher(pInst->m_szPass);
				strTmp.Insert(LOWORD(pInst->GetSel() - 1), cChar);
				pInst->m_szPass = m_AES->Cipher(strTmp);
				strTmp.Empty();
				pInst->m_dwPassLen++;
				return TRUE;
			}
		}
	} else {
		if (pInst->GetLimitText() > pInst->m_dwPassLen) {
			pInst->SendMessage(WM_CHAR, _T('?'), 0);
			CString strTmp;
			strTmp = m_AES->InvCipher(pInst->m_szPass);
			strTmp.Insert(LOWORD(pInst->GetSel() - 1), chChar);
			pInst->m_szPass = m_AES->Cipher(strTmp);
			strTmp.Empty();
			pInst->m_dwPassLen++;
			return TRUE;
		}
	}
	return FALSE;
}

VOID CSafePassEdit::Clear() {
	m_szPass = m_AES->Cipher(_T(""));
	m_dwPassLen = 0;
	SendMessage(WM_SETTEXT, NULL, NULL);
	CEdit::Clear();
}

CString CSafePassEdit::GetPassword() {
	return m_AES->InvCipher(m_szPass);
}

BOOL CSafePassEdit::DestroyWindow()
{
	m_lockInstances.Lock();
	m_pInstances.RemoveKey(this->m_hWnd);
	m_lockInstances.Unlock();
	return CEdit::DestroyWindow();
}

VOID CSafePassEdit::PreSubclassWindow()
{
	m_hCurWnd = this->m_hWnd;
	m_lockInstances.Lock();
	m_pInstances.SetAt(this->m_hWnd, this);
	m_lockInstances.Unlock();
	if (m_hHookThread == NULL) {
		m_hHookThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HookThreadProc, NULL, 0, NULL);
	}
	CEdit::PreSubclassWindow();
}

BOOL CSafePassEdit::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP) && 
		pMsg->wParam != VK_TAB && pMsg->wParam != VK_RETURN) {
		if (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT) {
			if (ISKEYDOWN(VK_SHIFT)) {
				return TRUE;
			}
		} else {
			return TRUE;
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}


TCHAR CSafePassEdit::VKey2Char( UINT uVKey ) {
	if (uVKey >= _T('A') && uVKey <= _T('Z')) {
		if (ISKEYTOGGLED(VK_CAPITAL)) {
			if (ISKEYDOWN(VK_SHIFT)) {
				return uVKey + _T('a') - _T('A');
			} else {
				return uVKey;
			}
		} else {
			if (ISKEYDOWN(VK_SHIFT)) {
				return uVKey;
			} else {
				return uVKey + _T('a') - _T('A');
			}
		}
	} else if (uVKey >= _T('0') && uVKey <= _T('9')) {
		if (ISKEYDOWN(VK_SHIFT)) {
			static const CHAR szNumSymbols[] = ")!@#$%^&*(";
			return szNumSymbols[uVKey - _T('0')];
		} else {
			return uVKey;
		}
	} else if (uVKey >= VK_NUMPAD0 && uVKey <= VK_NUMPAD9) {
		if (!ISKEYDOWN(VK_SHIFT)) {
			return uVKey - VK_NUMPAD0 + _T('0');
		}
	} else if (ISKEYDOWN(VK_SHIFT)) {
		switch (uVKey) {
			case VK_SPACE:
				return _T(' ');
			case VK_OEM_1:
				return _T(':');
			case VK_OEM_2:
				return _T('?');
			case VK_OEM_3:
				return _T('~');
			case VK_OEM_4:
				return _T('{');
			case VK_OEM_5:
				return _T('|');
			case VK_OEM_6:
				return _T('}');
			case VK_OEM_7:
				return _T('"');
			case VK_OEM_PLUS:
				return _T('+');
			case VK_OEM_COMMA:
				return _T('<');
			case VK_OEM_MINUS:
				return _T('_');
			case VK_OEM_PERIOD:
				return _T('>');
		}
	} else {
		switch (uVKey) {
			case VK_SPACE:
				return _T(' ');
			case VK_MULTIPLY:
				return _T('*');
			case VK_ADD:
				return _T('+');
			case VK_SUBTRACT:
				return _T('-');
			case VK_DECIMAL:
				return _T('.');
			case VK_DIVIDE:
				return _T('/');
			case VK_OEM_1:
				return _T(';');
			case VK_OEM_2:
				return _T('/');
			case VK_OEM_3:
				return _T('`');
			case VK_OEM_4:
				return _T('[');
			case VK_OEM_5:
				return _T('\\');
			case VK_OEM_6:
				return _T(']');
			case VK_OEM_7:
				return _T('\'');
			case VK_OEM_PLUS:
				return _T('=');
			case VK_OEM_COMMA:
				return _T(',');
			case VK_OEM_MINUS:
				return _T('-');
			case VK_OEM_PERIOD:
				return _T('.');
		}
	}
	return 0;
}

VOID CSafePassEdit::InputKey( UINT uVKey ) {
    keybd_event(uVKey, 0, 0, 0);
	Sleep(10);
    keybd_event(uVKey, 0, KEYEVENTF_KEYUP, 0);
}

HHOOK CSafePassEdit::MySetWindowsHookEx( INT idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId ) {
    WCHAR pwszLibFileName[MAX_PATH];
    UNICODE_STRING strLib;
	static PRtlInitUnicodeString RtlInitUnicodeString;

	if (RtlInitUnicodeString == NULL) {
		RtlInitUnicodeString = (PRtlInitUnicodeString) MyGetProcAddressByHashEx(m_hNTDLL, 0x0801F66C2);
	}
	
    if (hmod != NULL) {
        if (GetModuleFileNameW(hmod, pwszLibFileName,
                sizeof(pwszLibFileName) / sizeof(TCHAR)) == 0) {
            return NULL;
        }
		RtlInitUnicodeString(&strLib, pwszLibFileName);
	} else {
		RtlInitUnicodeString(&strLib, NULL);
	}

	return MyNtUserSetWindowsHookEx(
                hmod,
                &strLib,
                dwThreadId,
                idHook,
                lpfn,
                HF_ANSI);
}

HHOOK MyNtUserSetWindowsHookEx(
    HANDLE hmod,
    PUNICODE_STRING pstrLib,
    DWORD idThread,
    int nFilterType,
    HOOKPROC pfnFilterProc,
	DWORD dwFlags) {
	static LPBYTE lpFunc = NULL;
	static BYTE lpCode[50] = {0x90};

	if (lpCode[0] == 0x90) {
		DWORD i;
		PBYTE lpfnSetWindowsHookA = (PBYTE)MyGetProcAddressByHashEx(GetModuleHandle(_T("user32.dll")), 0x5800BFAE);
		PBYTE lpfnSetWindowsHookExW = (PBYTE)MyGetProcAddressByHashEx(GetModuleHandle(_T("user32.dll")), 0x002FE901);

		if (*(LPDWORD)(lpfnSetWindowsHookExW - 4) == 0x90909090) {
			lpFunc = lpfnSetWindowsHookExW;
			for (i = 0; i < 50; i++, lpFunc--) {
				if (lpFunc[0] == 0xb8 && 
					lpFunc[3] == 0 &&
					lpFunc[4] == 0) {
					lpFunc = lpFunc;
					break;
				}
			}
			memcpy(lpCode, lpFunc, lpfnSetWindowsHookExW - lpFunc);
		} else {
			lpFunc = lpfnSetWindowsHookA;
			for (i = 0; i < 30; i++) {
				if (lpFunc[i] == 0xe8) {
					lpFunc += i;
					lpFunc = *(PINT)(lpFunc + 1) + lpFunc + 5;
					break;
				}
			}
			memcpy(lpCode, lpFunc, 15);
			lpCode[1]++;
			for (i = 0; i < 15; i++) {
				if ((*(LPDWORD)(lpCode + i) & 0xFFFFFF) == 0xcc2) {
					lpCode[i + 1] = 0x18;
					break;
				}
			}
		}
	}

	HHOOK hRet;
	__asm {
		lea eax, lpCode;
		push dwFlags;
		push pfnFilterProc;
		push nFilterType;
		push idThread;
		push pstrLib;
		push hmod;
		call eax;
		mov [hRet], eax;
	}
	return hRet;
}

FARPROC WINAPI MyGetProcAddressByHashEx(HMODULE hModule, INT hash) {
	if (hModule == NULL || hash == 0) {
		return 0;
	}

	// 验证hModule是否有效
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;

	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		return 0;
	}

	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((PBYTE)hModule + pDosHeader->e_lfanew);

	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) {
		return 0;
	}

	// 判断导出表是否存在
	IMAGE_DATA_DIRECTORY DataDirectory = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

	if (DataDirectory.VirtualAddress == 0 || DataDirectory.Size == 0) {
		return 0;
	}

	IMAGE_EXPORT_DIRECTORY* pExportDirectory = (IMAGE_EXPORT_DIRECTORY*)((PBYTE)hModule + DataDirectory.VirtualAddress);
	PDWORD pAddressOfNames = (PDWORD)((PBYTE)hModule + pExportDirectory->AddressOfNames);
	PWORD pAddressOfNameOrdinals = (PWORD)((PBYTE)hModule + pExportDirectory->AddressOfNameOrdinals);
	PDWORD pAddressOfFunctions = (PDWORD)((PBYTE)hModule + pExportDirectory->AddressOfFunctions);

	// 按函数名查找函数地址
	for (unsigned i = 0; i < pExportDirectory->NumberOfNames; i++) {
		LPCSTR pszName = (LPCSTR)((PBYTE)hModule + pAddressOfNames[i]);
		WORD nIndex = pAddressOfNameOrdinals[i];

		if (GetRolHashEx(pszName) == hash) {
			return (FARPROC)(pAddressOfFunctions[nIndex] + (DWORD)hModule);
		}
	}

	return 0;
}

INT __declspec(naked) WINAPI GetRolHashEx(LPCSTR lpApiString) {
	__asm {
		mov eax, dword ptr[esp + 4];
		push esi;
		push edx;
		xor  edx, edx;
		xchg  eax, esi;
		cld;
	lNext:
		lodsb;
		test al, al;
		jz lRet;
		rol edx, 3;
		xor dl, al;
		jmp lNext;
	lRet:
		xchg eax, edx
		pop edx;
		pop esi;
		ret 4;
	}
}

HMODULE __declspec(naked) WINAPI GetNTDLLHandleEx() {
	__asm {
		push 0x30;
		pop eax;
		mov eax, dword ptr fs:[eax];
		mov eax, dword ptr [eax + 0xc];
		mov eax, dword ptr [eax + 0x1c];
		mov eax, dword ptr [eax + 0x8];
		ret;
	}
}

AES* CSafePassEdit::m_AES = NULL;

CMap<HWND, HWND, CSafePassEdit*, CSafePassEdit*> CSafePassEdit::m_pInstances;

HMODULE CSafePassEdit::m_hNTDLL = GetNTDLLHandleEx();

HANDLE CSafePassEdit::m_hKBDThread = NULL;

HANDLE CSafePassEdit::m_hHookThread = NULL;

HHOOK CSafePassEdit::m_hLLKBDHook = NULL;

HHOOK CSafePassEdit::m_hDebugHook = NULL;

BOOL CSafePassEdit::m_bHooking = FALSE;

CSafePassEdit* CSafePassEdit::m_pFocusEdit = NULL;

ATL::CComAutoCriticalSection CSafePassEdit::m_lockInstances;

BEGIN_MESSAGE_MAP(CSafePassEdit, CEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CSafePassEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	m_pFocusEdit = this;
}

void CSafePassEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	if( m_pFocusEdit == this )
		m_pFocusEdit = NULL;
}
