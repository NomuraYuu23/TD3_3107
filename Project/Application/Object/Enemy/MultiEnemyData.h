#pragma once
#include "../../../Engine/Math/Vector3.h"

/// <summary>
/// マルチエネミーデータ
/// </summary>
struct MultiEnemyData {
	Vector3 position;
	float distance;
	int32_t enemyMaxCount;
};


inline void to_json(nlohmann::json& json, const MultiEnemyData& value) {
    json = nlohmann::json{
        {"position",value.position },
        {"distance",value.distance },
        {"enemyMaxCount",value.enemyMaxCount },
    };
}

inline void from_json(const nlohmann::json& json, MultiEnemyData& value) {

    // キーを確認
    if (json.contains("position") && json["position"].is_array() &&
        json.contains("distance") && json["distance"].is_number_float() &&
        json.contains("enemyMaxCount") && json["enemyMaxCount"].is_number_integer()) {

        value.position = json["position"].get<Vector3>();
        value.distance = json["distance"].get<float>();
        value.enemyMaxCount = json["enemyMaxCount"].get<int32_t>();

    }

}

