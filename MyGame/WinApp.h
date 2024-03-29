#pragma once
#include<Windows.h>

class WinApp
{
public:
private:
	WNDCLASSEX w{};
	HWND hwnd = nullptr;
public:
	//window横幅・縦幅
	static const int window_width = 1900;
	static const int window_height = 1020;
	RECT wrc;

	//ゲッター 
	//このhwndとかスワップチェーン作成とかで使うため
	//dxgifactoryは汎用機能のところ
	//instancehaは入力処理部分で使う
	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetIns() { return w.hInstance; }

	//メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void Createwindow(); //初期化
	//メッセージ処理
	bool Processmassage();
	void Deletewindow();
	void Finalize();
	static int CreateErrorWindow(bool errorjudg, LPCTSTR text);
};
