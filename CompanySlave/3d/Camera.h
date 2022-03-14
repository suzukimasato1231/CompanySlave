#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include"Vec.h"
extern const int window_width;
extern const int window_height;

class Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
private:
	//ビュー変換行列
	XMMATRIX matView;
	//射影変換
	XMMATRIX matProjection;

	Vec3 eye = {0.0f,0.0f,10.0f}, target = {}, up = {};
public:
	Camera();

	~Camera();

	/// <summary>
	/// カメラ生成
	/// </summary>
	/// <returns></returns>
	static Camera *Create();

	//カメラ初期化/カメラ位置eye/カメラターゲットtarget /カメラの向きup
	void Initilize(Vec3 eye, Vec3 target, Vec3 up);
	//カメラ位置セット/カメラ位置eye/カメラターゲットtarget /カメラの向きup
	void SetCamera(Vec3 eye, Vec3 target, Vec3 up);

	/// <summary>
	/// カメラが追従する
	/// </summary>
	/// <param name="position">追従するオブジェクトの座標</param>
	/// <param name="d">オブジェクトとカメラの距離</param>
	/// <param name="angleX">カメラの向きX</param>
	/// <param name="angleY">カメラの向きY</param>
	/// <returns></returns>
	void FollowCamera(Vec3 position, Vec3 d, float angleX = 0, float angleY = 0);

	//matViewを獲得
	XMMATRIX GetMatView();
	//projectionを獲得
	XMMATRIX GetProjection();

	Vec3 GetEye();

	Vec3 GetTarget();

	Vec3 GetUp();
};