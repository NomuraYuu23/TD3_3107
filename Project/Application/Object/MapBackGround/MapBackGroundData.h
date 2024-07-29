#pragma once
#include "../../../Engine/Math/Vector3.h"

struct MapBackGroundData
{
	Vector3 position; // 位置
	Vector3 size; // 大きさ
};

inline void to_json(nlohmann::json& json, const MapBackGroundData& value) {
    json = nlohmann::json{
        {"position",value.position },
        {"size",value.size },
    };
}

inline void from_json(const nlohmann::json& json, MapBackGroundData& value) {

    // キーを確認
    if (json.contains("position") && json["position"].is_array() &&
        json.contains("size") && json["size"].is_array() ) {

        value.position = json["position"].get<Vector3>();
        value.size = json["size"].get<Vector3>();
    }

}