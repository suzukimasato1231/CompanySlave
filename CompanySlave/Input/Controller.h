#pragma once
#include <dinput.h>
#include <DirectXMath.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
enum ButtonKind
{
	LButtonUp,
	LButtonDown,
	LButtonLeft,
	LButtonRight,
	RButtonUp,
	RButtonDown,
	RButtonLeft,
	RButtonRight,
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	ButtonKindMax,
};

enum ButtonState
{
	ButtonStateNone,
	ButtonStateDown,
	ButtonStatePush,
	ButtonStateUp,
	ButtonStateMax,
};

class Controller
{
private:
	// ゲームパッドの入力情報取得
	DIJOYSTATE pad_data;
	DIJOYSTATE oldpad_data;
	LPDIRECTINPUTDEVICE8 g_GamePadDevice = nullptr;
	LPDIRECTINPUT8 g_InputInterface = nullptr;
private:
	
public://ゲームパッド
	/// <summary>
	/// デバイス入力の初期化
	/// </summary>
	/// <returns></returns>
	bool InitController();

	/// <summary>
	/// Input終了関数
	/// </summary>
	void ReleaseInput();

	/// <summary>
	/// デバイスの更新
	/// </summary>
	void UpdateInput();

	/// <summary>
	/// ボタンが押されているかどうか
	/// </summary>
	/// <param name="button">ボタンの種類</param>
	/// <returns></returns>
	bool IsButtonPush(ButtonKind button);

	/// <summary>
	/// ボタンが押された瞬間かどうか
	/// </summary>
	/// <param name="button">ボタンの種類</param>
	/// <returns></returns>
	bool IsButtonUp(ButtonKind button);

	/// <summary>
	/// ボタンが離れた瞬間
	/// </summary>
	/// <param name="button">ボタンの種類</param>
	/// <returns></returns>
	bool IsButtonDown(ButtonKind button);
	/// <summary>
	/// 右パッド角度取得
	/// </summary>
	/// <returns></returns>
	float GetLightAngle();

	//角度を取得
	void SetAngle(DIJOYSTATE pad_data);
};