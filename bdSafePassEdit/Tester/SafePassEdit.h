#pragma once
#include <Afxtempl.h>

class CSafePassEdit : public CEdit
{
public:
	CSafePassEdit(void);
	~CSafePassEdit(void);

	CString GetPassword();
	BOOL InputChar(CHAR chChar, UINT uVKey);

	VOID Clear();

protected:
	virtual VOID PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();

private:
	static DWORD WINAPI HookThreadProc(LPVOID param);
	static DWORD WINAPI KBDThreadProc(LPVOID param);
	static LRESULT CALLBACK DebugProc(int nCode, WPARAM wParam, LPARAM lParam); 
	static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, PKBDLLHOOKSTRUCT lParam);
	static TCHAR VKey2Char(UINT uVKey);
	static VOID InputKey(UINT uVKey);
	static BOOL InputChar(CSafePassEdit* pInst, CHAR chChar, UINT uVKey);
	static HHOOK MySetWindowsHookEx(INT idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId);

private:
	DWORD m_dwPassLen;
	CString m_szPass;

	static BOOL m_bHooking;
	static HHOOK m_hDebugHook;
	static HHOOK m_hLLKBDHook;
	static HANDLE m_hHookThread;
	static HANDLE m_hKBDThread;

	static HMODULE m_hNTDLL;
	static PVOID* m_AES;
	static CMap<HWND, HWND, CSafePassEdit*, CSafePassEdit*> m_pInstances;
};