#pragma once
#include "../../../Engine/Math/Vector3.h"

/// <summary>
/// シングルエネミーデータ
/// </summary>
struct SingleEnemyData {
	Vector3 position;
	int32_t typeNum;
};

inline void to_json(nlohmann::json& json, const SingleEnemyData& value) {
    json = nlohmann::json{
        {"position",value.position },
        {"typeNum",value.typeNum },
    };
}

inline void from_json(const nlohmann::json& json, SingleEnemyData& value) {

    // キーを確認
    if (json.contains("position") && json["position"].is_array() &&
        json.contains("typeNum") && json["typeNum"].is_array()) {

        value.position = json["position"].get<Vector3>();
        value.typeNum = json["typeNum"].get<int32_t>();
    }

}
