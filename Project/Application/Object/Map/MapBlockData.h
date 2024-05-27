#pragma once
#include "../../../Engine/Math/Vector2.h"

struct MapBlockData
{
	Vector2 position; // 位置
	Vector2 size; // 大きさ
};

inline void to_json(nlohmann::json& json, const MapBlockData& value) {
    json = nlohmann::json{
        {"position",value.position },
        {"size",value.size },
    };
}

inline void from_json(const nlohmann::json& json, MapBlockData& value) {

    // キーを確認
    if (json.contains("position") && json["position"].is_array() &&
        json.contains("size") && json["size"].is_array() ) {

        value.position = json["position"].get<Vector2>();
        value.size = json["size"].get<Vector2>();
    }

}