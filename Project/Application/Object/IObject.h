#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/Collider2D/ColliderShape2D.h"
#include "GameUtility/CommonConfig.h"

class IObject
{
public:
	IObject() {};
	~IObject() = default;

	static float sPlaySpeed;
public:
	
	enum CollisionType {
		// 2点
		kLeftSide,
		kRightSide,
		kTopSide,
		kBottomSide,
		// 1点
		kLTPoint,
		kLBPoint,
		kRTPoint,
		kRBPoint,
		// 3点
		kMultiPoints,
		kNone,
	};

	struct FourTop {
		Vector2 leftTop, rightTop, leftBottom, rightBottom;
	};

	inline static CollisionType GetCollisionType(FourTop m1, const Vector2& targetMin,const Vector2& targetMax) {
		// 四隅のチェック
		// 左上
		bool isInLT = ((m1.leftTop.x > targetMin.x) && (m1.leftTop.x < targetMax.x) &&
			(m1.leftTop.y > targetMin.y) && (m1.leftTop.y < targetMax.y));
		// 左下
		bool isInLB = ((m1.leftBottom.x > targetMin.x) && (m1.leftBottom.x < targetMax.x) &&
			(m1.leftBottom.y > targetMin.y) && (m1.leftBottom.y < targetMax.y));
		// 右上
		bool isInRT = ((m1.rightTop.x > targetMin.x) && (m1.rightTop.x < targetMax.x) &&
			(m1.rightTop.y > targetMin.y) && (m1.rightTop.y < targetMax.y));
		// 右下
		bool isInRB = ((m1.rightBottom.x > targetMin.x) && (m1.rightBottom.x < targetMax.x) &&
			(m1.rightBottom.y > targetMin.y) && (m1.rightBottom.y < targetMax.y));

		//--- パターン決め ---//
		// 数
		int inSideCount = isInLT + isInLB + isInRT + isInRB;
		// 一か所だけ
		if (inSideCount == 1) {
			if (isInLT) {
				return CollisionType::kLTPoint;
			}
			else if (isInLB) {
				return CollisionType::kLBPoint;
			}
			else if (isInRT) {
				return CollisionType::kRTPoint;
			}
			else if (isInRB) {
				return CollisionType::kRBPoint;
			}
		}
		// 複数
		else if (inSideCount >= 3) {
			return CollisionType::kMultiPoints;
		}

		else {
			// 左
			if (isInLT && isInLB) {
				return CollisionType::kLeftSide;
			}
			// 右
			else if (isInRT && isInRB) {
				return CollisionType::kRightSide;
			}
			// 上
			else if (isInLT && isInRT) {
				return CollisionType::kTopSide;
			}
			// 下
			else if (isInLB && isInRB) {
				return CollisionType::kBottomSide;
			}
		}

		return CollisionType::kNone;
	}

	/// <summary>
	/// 四点の生成関数
	/// </summary>
	/// <param name="minV2"></param>
	/// <param name="maxV2"></param>
	/// <returns></returns>
	inline static FourTop GenerateFourTop(Vector2 minV2, Vector2 maxV2) {
		FourTop result;
		// 上
		result.leftTop = { minV2.x,maxV2.y };
		result.rightTop = { maxV2.x,maxV2.y };

		// 下
		result.leftBottom = { minV2.x,minV2.y };
		result.rightBottom = { maxV2.x,minV2.y };

		return result;
	}

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	virtual void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	virtual void Draw(const BaseCamera& camera) = 0;
	/// <summary>
	/// ImGui
	/// </summary>
	virtual void ImGuiDraw() = 0;

	/// <summary>
	/// 衝突のコールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	virtual void OnCollision(ColliderParentObject2D target) = 0;

public: // アクセッサ
	/// <summary>
	/// コライダーの取得
	/// </summary>
	/// <returns></returns>
	virtual Box GetBoxCollider() = 0;

	virtual Vector2 GetColliderPosition() = 0;
	virtual Vector2 GetColliderSize() = 0;

	// 死亡フラグ取得
	bool IsDead() { return isDead_; }

protected:
	// ボックスコライダー更新
	void BoxColliderUpdate() {
		boxCollider_.Update(position2D_, scale2D_.x, scale2D_.y, 0.0f);
	}
	// サークルコライダー更新
	void CircleColliderUpdate() {
		circleCollider_.Update(position2D_, circleCollider_.radius_);
	}

	// 死亡フラグ
	bool isDead_ = false;

public:
	// モデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldtransform_;

	// コライダー用の座標・スケール
	Vector2 position2D_ = {};
	Vector2 scale2D_ = {};

	// コライダー
	Box boxCollider_;
	Circle circleCollider_;

	// マテリアル関係
	std::unique_ptr<Material> material_ = nullptr;
	int32_t enableLighting_;
	float shininess_;
	// 速さベクトル
	Vector3 velocity_ = {};

	// ローカル行列
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;

};

