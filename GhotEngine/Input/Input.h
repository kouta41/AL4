/// <summary>
/// 入力の設定
/// </summary>

#pragma once
#include <Window.h>
#include <cassert>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Xinput.h>
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"dinput8.lib")

class Input {
public:
	struct ButtonState {
		bool isPressed;
		bool wasPressed;

		ButtonState() : isPressed(false), wasPressed(false) {}
	};

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	static void Update();

	/// <summary>
	/// 押されてたら作動
	/// </summary>
	/// <param name="keyNum"></param>
	/// <returns></returns>
	bool PushKey(uint8_t keyNum);

	/// <summary>
	/// 押された瞬間作動
	/// </summary>
	/// <param name="keyNum"></param>
	/// <returns></returns>
	bool PressedKey(uint32_t keyNum);

	/// <summary>
	/// パッドの設定
	/// </summary>
	/// <returns></returns>
	static bool GetJoystickState();

	/// <summary>
	/// パッドの設定
	/// </summary>
	/// <param name="out"></param>
	/// <returns></returns>
	bool GetJoystickState(XINPUT_STATE& out);

	/// <summary>
	/// パットを押したとき
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool PressedButton(WORD button);

	/// <summary>
	/// パットの更新
	/// </summary>
	/// <param name="state"></param>
	/// <param name="isPressed"></param>
	void UpdateButtonState(ButtonState& state, bool isPressed);


private:

	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;

	BYTE keys[256] = {};
	BYTE preKeys[256] = {};

	bool isInitialize = false;

	XINPUT_STATE state_{};
	XINPUT_STATE preState_{};

	//0x80=押している状態
	//0x00=押してない状態
	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
};
