#include "Collision2D.h"
#include "../Math/Matrix3x3.h"
#include <numbers>

bool Collision2D::IsCollision(const Box& box1, const Box& box2)
{
		
	float thata = box1.rotation_ * static_cast<float>(std::numbers::pi) / 180.0f;
	Matrix3x3 box1RotationMatrix = Matrix3x3::MakeRotateMatrix(thata);
	thata = box2.rotation_ * static_cast<float>(std::numbers::pi) / 180.0f;
	Matrix3x3 box2RotationMatrix = Matrix3x3::MakeRotateMatrix(thata);

	std::vector<Vector2> b1Point;
	std::vector<Vector2> b2Point;

	Vector2 b1LeftTop = { 
		-box1.scale_.x / 2.0f,
		-box1.scale_.y / 2.0f };

	b1LeftTop = Matrix3x3::Transform(b1LeftTop, box1RotationMatrix);
	b1LeftTop += box1.position_;
	b1Point.push_back(b1LeftTop);

	Vector2 b1RightTop = {
		+box1.scale_.x / 2.0f,
		-box1.scale_.y / 2.0f };

	b1RightTop = Matrix3x3::Transform(b1RightTop, box1RotationMatrix);
	b1RightTop += box1.position_;
	b1Point.push_back(b1RightTop);

	Vector2 b1RightBottom = {
		+box1.scale_.x / 2.0f,
		+box1.scale_.y / 2.0f };

	b1RightBottom = Matrix3x3::Transform(b1RightBottom, box1RotationMatrix);
	b1RightBottom += box1.position_;
	b1Point.push_back(b1RightBottom);

	Vector2 b1LeftBottom = {
		-box1.scale_.x / 2.0f,
		+box1.scale_.y / 2.0f };

	b1LeftBottom = Matrix3x3::Transform(b1LeftBottom, box1RotationMatrix);
	b1LeftBottom += box1.position_;
	b1Point.push_back(b1LeftBottom);

	Vector2 b2LeftTop = {
		-box2.scale_.x / 2.0f,
		-box2.scale_.y / 2.0f };

	b2LeftTop = Matrix3x3::Transform(b2LeftTop, box2RotationMatrix);
	b2LeftTop += box2.position_;
	b2Point.push_back(b2LeftTop);

	Vector2 b2RightTop = {
		+box2.scale_.x / 2.0f,
		-box2.scale_.y / 2.0f };

	b2RightTop = Matrix3x3::Transform(b2RightTop, box2RotationMatrix);
	b2RightTop += box2.position_;
	b2Point.push_back(b2RightTop);

	Vector2 b2RightBottom = {
		+box2.scale_.x / 2.0f,
		+box2.scale_.y / 2.0f };

	b2RightBottom = Matrix3x3::Transform(b2RightBottom, box2RotationMatrix);
	b2RightBottom += box2.position_;
	b2Point.push_back(b2RightBottom);

	Vector2 b2LeftBottom = {
		-box2.scale_.x / 2.0f,
		+box2.scale_.y / 2.0f };

	b2LeftBottom = Matrix3x3::Transform(b2LeftBottom, box2RotationMatrix);
	b2LeftBottom += box2.position_;
	b2Point.push_back(b2LeftBottom);

	for (uint32_t i = 0; i < 4; ++i) {
		Vector2 a = b1Point[i];
		Vector2 b;
		if (i == 3) {
			b = b1Point[0];
		}
		else {
			b = b1Point[i + 1];
		}

		for (uint32_t j = 0; j < 4; ++j) {
			Vector2 c = b2Point[j];
			Vector2 d;
			if (j == 3) {
				d = b2Point[0];
			}
			else {
				d = b2Point[j + 1];
			}

			float s1 = Vector2::Cross((b - a), (c - a));
			float t1 = Vector2::Cross((b - a), (d - a));

			float s2 = Vector2::Cross((d - c), (a - c));
			float t2 = Vector2::Cross((d - c), (b - c));

			if ((s1 * t1 <= 0.0f) && (s2 * t2 <= 0.0f)) {
				return true;
			}

		}
	}

	return false;

}

bool Collision2D::IsCollision(const Circle& circle1, const Circle& circle2)
{

	float d = Vector2::Length(Vector2::Subtract(circle1.position_, circle2.position_));

	float r = circle1.radius_ + circle2.radius_;

	if (d <= r) {

		return true;

	}

	return false;

}

bool Collision2D::IsCollision(const Box& box, const Circle& circle)
{
	
	bool result = false;

	float thata = box.rotation_ * static_cast<float>(std::numbers::pi) / 180.0f;
	Matrix3x3 boxWorldMatrix = Matrix3x3::MakeRotateMatrix(thata);
	boxWorldMatrix = Matrix3x3::Multiply(boxWorldMatrix, Matrix3x3::MakeTranslateMatrix(box.position_));

	Matrix3x3 boxWorldMatrixInverse = Matrix3x3::Inverse(boxWorldMatrix);

	Vector2 centerInBoxLocalSpace = Matrix3x3::Transform(circle.position_, boxWorldMatrixInverse);

	// 基準の矩形
	float left = -box.scale_.x / 2.0f;
	float right = box.scale_.x / 2.0f;
	float top = -box.scale_.y / 2.0f;
	float bottom = box.scale_.y / 2.0f;

	// 矩形に対応した円
	Circle newCircre;
	newCircre.position_ = centerInBoxLocalSpace;
	newCircre.radius_ = circle.radius_;

	// 四角形の4辺に対して円の半径分だけ足したとき円が重なっていたら
	if ((newCircre.position_.x >= left - newCircre.radius_) &&
		(newCircre.position_.x <= right + newCircre.radius_) &&
		(newCircre.position_.y >= top - newCircre.radius_) &&
		(newCircre.position_.y <= bottom + newCircre.radius_)) {

		result = true;
		Vector2 boxPoint = { 0.0f, 0.0f };

		// 左
		if (newCircre.position_.x < left) {

			boxPoint.x = left;

			// 左上
			if (newCircre.position_.y < top) {

				boxPoint.y = top;

				if (Vector2::Length(boxPoint - newCircre.position_) >= newCircre.radius_) {
					result = false;
				}

			}
			// 左下
			else if(newCircre.position_.y > bottom){

				boxPoint.y = bottom;

				if (Vector2::Length(boxPoint - newCircre.position_) >= newCircre.radius_) {
					result = false;
				}

			}

		}
		else if (newCircre.position_.x > right) {

			boxPoint.x = right;

			// 右上
			if (newCircre.position_.y < top) {

				boxPoint.y = top;

				if (Vector2::Length(boxPoint - newCircre.position_) >= newCircre.radius_) {
					result = false;
				}

			}
			// 右下
			else if (newCircre.position_.y > bottom) {

				boxPoint.y = bottom;

				if (Vector2::Length(boxPoint - newCircre.position_) >= newCircre.radius_) {
					result = false;
				}

			}

		}

	}

	return result;

}

bool Collision2D::IsCollision(const Circle& circle, const Box& box)
{
	return IsCollision(box, circle);
}
