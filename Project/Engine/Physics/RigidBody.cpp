#include "RigidBody.h"

Vector3 RigidBody::TorqueCalc(
	const Vector3& centerOfGravity,
	const Vector3& pointOfAction,
	const Vector3& force)
{

	Vector3 result;
	Vector3 r = Vector3::Subtract(pointOfAction, centerOfGravity);
	result = Vector3::Cross(r, force);
	return result;

}

Matrix4x4 RigidBody::InertiaTensorCalc(
	const Matrix4x4& postureMatrix,
	const Matrix4x4& basicPostureInertiaTensor)
{

	Matrix4x4 result;

	Matrix4x4 transposePostureMatrix = Matrix4x4::Transpose(postureMatrix);

	result = Matrix4x4::Multiply(Matrix4x4::Multiply(postureMatrix, basicPostureInertiaTensor), transposePostureMatrix);

	return result;
}

Matrix4x4 RigidBody::PostureCalc(
	const Matrix4x4& postureMatrix,
	const Vector3& angularVelocity,
	float time)
{

	Vector3 postureX = { postureMatrix.m[0][0],postureMatrix.m[0][1], postureMatrix.m[0][2] };
	Vector3 postureY = { postureMatrix.m[1][0],postureMatrix.m[1][1], postureMatrix.m[1][2] };
	Vector3 postureZ = { postureMatrix.m[2][0],postureMatrix.m[2][1], postureMatrix.m[2][2] };

	postureX = Vector3::Normalize(Vector3::Cross(angularVelocity, postureX) + postureX);
	postureY = Vector3::Normalize(Vector3::Cross(angularVelocity, postureY) + postureY);
	postureZ = Vector3::Normalize(Vector3::Cross(angularVelocity, postureZ) + postureZ);

	Matrix4x4 result;
	result.m[0][0] = postureX.x;
	result.m[0][1] = postureX.y;
	result.m[0][2] = postureX.z;
	result.m[0][3] = 0.0f;

	result.m[1][0] = postureY.x;
	result.m[1][1] = postureY.y;
	result.m[1][2] = postureY.z;
	result.m[1][3] = 0.0f;

	result.m[2][0] = postureZ.x;
	result.m[2][1] = postureZ.y;
	result.m[2][2] = postureZ.z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;

}

Vector3 RigidBody::AngularMomentumCalc(
	const Vector3& angularMomentum,
	const Vector3& torque,
	float time)
{

	Vector3 result;
	result = Vector3::Add(angularMomentum, Vector3::Multiply(torque, time));
	return result;

}

Vector3 RigidBody::AngularVelocityCalc(
	const Matrix4x4& inertiaTensor,
	const Vector3& angularMomentum)
{

	Vector3 result;
	result = Matrix4x4::TransformNormal(angularMomentum, Matrix4x4::Inverse(inertiaTensor));
	return result;

}
