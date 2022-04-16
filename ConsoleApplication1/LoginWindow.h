#pragma once
#include <Windows.h>
#include <string>

class LoginWindow
{

public:
	LoginWindow(void (*callback)(void*), void* obj, HINSTANCE hInst);

	void setup(HWND hwnd);

	void show();
	void hide();

	HWND windowHandler();

	const std::string& getUsername();
	const std::string& getPassword();

private:

	void create(HINSTANCE hInst, LPVOID data, WNDPROC proc);

	HWND handler() const;
	HWND userHandler() const;
	HWND passHandler() const;
	HWND okHandler() const;

	void Invoke();

	static LRESULT CALLBACK GUIWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	HWND window_;
	HWND user_;
	HWND pass_;
	HWND ok_;
	HBITMAP logo_;
	HDC hdcSource_;

	HBRUSH hbrBkgnd_;

	void (*callback_)(void*);
	void* callbackObj_;

	std::string userbox_;
	std::string passwordbox_;

	HINSTANCE hInst_;
};
