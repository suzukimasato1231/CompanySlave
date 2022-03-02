#pragma once
#include"Object.h"
/// <summary>
/// }Œ`ƒ‚ƒfƒ‹
/// </summary>
class Shape
{
public:
	Shape();
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	/// <param name="dev">ƒfƒoƒCƒX</param>
	static void Init(ID3D12Device *dev);
public://}Œ`ƒ‚ƒfƒ‹
	/// <summary>
	/// OŠp
	/// </summary>
	/// <param name="width">‰¡‚Ì’·‚³</param>
	/// <param name="height">c‚Ì’·‚³</param>
	/// <returns></returns>
	static Object::ObjectData CreateTriangularPyramid(const float width, const float height);
	/// <summary>
	/// ’ZŒ`
	/// </summary>
	/// <param name="width">‰¡‚Ì’·‚³</param>
	/// <param name="height">c‚Ì’·‚³</param>
	/// <returns></returns>
	static Object::ObjectData CreateRect(const float width, const float height);
	/// <summary>
	/// ‰~
	/// </summary>
	/// <param name="r">”¼Œa</param>
	/// <param name="num">•ªŠ„”</param>
	/// <returns></returns>
	static Object::ObjectData CreateCircle(const float r, const int num);
	/// <summary>
	/// —§•û‘Ì
	/// </summary>
	/// <param name="width">‰¡‚Ì’·‚³</param>
	/// <param name="height">c‚Ì’·‚³</param>
	/// <param name="depth">[‚³</param>
	/// <returns></returns>
	static Object::ObjectData CreateSquare(const float width, const float height, const float depth);
	/// <summary>
	/// ‰~’Œ
	/// </summary>
	/// <param name="division">•ªŠ„”</param>
	/// <param name="prizmHeight">‚‚³</param>
	/// <param name="radius">’ê–Ê‚Ì”¼Œa</param>
	/// <returns></returns>
	static Object::ObjectData CreateCylinder(int division, int prizmHeight, int radius);
public:
	static ID3D12Device *dev;					//ƒfƒoƒCƒX

	static void CreateModel(Object::ObjectData &polygon);
};