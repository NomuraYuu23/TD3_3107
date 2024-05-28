#pragma once
#include "../../../Engine/Math/Vector3.h"

struct ChaseEnemyData {
	// 座標
	Vector3 position;
	// 索敵範囲
	float searchLength;
	//int32_t typeNum;
};

inline void to_json(nlohmann::json& json, const ChaseEnemyData& value) {
    json = nlohmann::json{
        {"position",value.position },
        {"searchLength",value.searchLength },
    };
}

inline void from_json(const nlohmann::json& json, ChaseEnemyData& value) {

    // キーを確認
    if (json.contains("position") && json["position"].is_array() &&
        json.contains("searchLength") && json["searchLength"].is_number_float()) {

        value.position = json["position"].get<Vector3>();
        value.searchLength = json["searchLength"].get<float>();
    }

}
