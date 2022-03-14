#include "Camera.h"
using namespace DirectX;

Camera::Camera()
{
	matView = XMMatrixIdentity();
	//射影変換
	matProjection = XMMatrixIdentity();
}

Camera::~Camera()
{
}

Camera *Camera::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	Camera *view = new Camera();
	if (view == nullptr) {
		return nullptr;
	}

	// 初期化
	return view;
}

void Camera::Initilize(Vec3 eye, Vec3 target, Vec3 up)
{
	//射影変換用
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
	this->eye = eye, this->target = target, this->up = up;
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::SetCamera(Vec3 eye, Vec3 target, Vec3 up)
{
	this->eye = eye, this->target = target, this->up = up;
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//射影変換用
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 5100.0f
	);
}

void Camera::FollowCamera(Vec3 position, Vec3 d, float angleX, float angleY)
{
	target = position;//注視点座標
   //カメラ追従
   //1
	Vec3 V0 = d;
	//2
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(angleX));//Y軸
	rotM *= XMMatrixRotationX(XMConvertToRadians(angleY));//X軸
	//3
	XMVECTOR v3 = { V0.x,V0.y,V0.z };
	XMVECTOR v = XMVector3TransformNormal(v3, rotM);

	//4
	Vec3 f3 = { v.m128_f32[0],v.m128_f32[1],v.m128_f32[2] };
	eye.x = target.x + f3.x, eye.y = target.y + f3.y, eye.z = target.z + f3.z;

	//5
	//ビュー変換行列
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

XMMATRIX Camera::GetMatView()
{
	return matView;
}

XMMATRIX Camera::GetProjection()
{
	return matProjection;
}

Vec3 Camera::GetEye()
{
	return eye;
}

Vec3 Camera::GetTarget()
{
	return target;
}

Vec3 Camera::GetUp()
{
	return up;
}
