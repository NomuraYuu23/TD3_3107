#pragma once
#include "../../../Engine/Math/Vector3.h"

/// <summary>
/// シングルエネミーデータ
/// </summary>
struct SingleEnemyData {
	Vector3 position;
	int32_t typeNum;
    Vector3 endPosition;
    int32_t pad;
};

inline void to_json(nlohmann::json& json, const SingleEnemyData& value) {
    json = nlohmann::json{
        {"position",value.position },
        {"typeNum",value.typeNum },
        {"easeEndPosition" ,value.endPosition},
        {"pad" ,value.pad}
    };
}

inline void from_json(const nlohmann::json& json, SingleEnemyData& value) {

    // キーを確認
    if (json.contains("position") && json["position"].is_array() &&
        json.contains("typeNum") && json["typeNum"].is_number_integer() &&
        json.contains("easeEndPosition") && json["easeEndPosition"].is_array() &&
        json.contains("pad") && json["pad"].is_number_integer()) {

        value.position = json["position"].get<Vector3>();
        value.typeNum = json["typeNum"].get<int32_t>();
        value.endPosition = json["easeEndPosition"].get<Vector3>();
        value.pad = json["pad"].get<int32_t>();
    }

}
