#pragma once
#include"CollisionPrimitive.h"

///<summary>
///当たり判定ヘルパークラス
///</summary>
class Collision
{
public:
	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点（出力用）</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR &point, const Triangle &triangle, DirectX::XMVECTOR *closest);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA">球A</param>
	/// <param name="sphereB">球B</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere &sphereA, const Sphere &sphereB, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点（平面上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere &sphere, const Plane &plane, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点（三角形上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere &sphere, const Triangle &triangle, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray &lay, const Plane &plane, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray &lay, const Triangle &triangle, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="sphere">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray &lay, const Sphere &sphere, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// 球と球
	/// </summary>
	/// <param name="sphere1">球</param>
	/// <param name="sphere2">球</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere &sphere1, const Sphere &sphere2);

	/// <summary>
	/// 直方体と直方体
	/// </summary>
	/// <param name="box1"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CheckBox2Box(const Box &box1, const Box &box);

	/// <summary>
	/// 球と直方体(AABB制限付き)
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CheckSphere2Box(const Sphere &sphere, const Box &box);

	/// <summary>
	/// 球とカプセル
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="capsule">カプセル</param>
	/// <returns></returns>
	static bool CheckSphere2Capsule(const Sphere &sphere, const Capsule &capsule);

	/// <summary>
	/// カプセルとカプセル
	/// </summary>
	/// <param name="capsule1">カプセル</param>
	/// <param name="capsule2">カプセル</param>
	/// <returns></returns>
	static bool CheckCapsule2Capsule(const Capsule capsule1, const Capsule &capsule2);



	//線分と線分の距離の２乗を求める
	static float sqDistanceSegmentSegment(const DirectX::XMVECTOR &p1, const DirectX::XMVECTOR &q1, const DirectX::XMVECTOR &p2, const DirectX::XMVECTOR &q2);

	//値を範囲内に収める[low,high]
	static float clamp(float x, float low, float high)
	{
		x = (x < low) ? low : x;
		x = (x > high) ? high : x;
		return x;
	}
};