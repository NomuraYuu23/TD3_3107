#pragma once
#include "../Math/Matrix4x4.h"

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
	Matrix4x4 ScaleInverse; //アウトライン用
};
