#pragma once
#include "../../../../Engine/Math/Vector3.h"

struct CheckPointData
{
	Vector3 position;
	int32_t checkPointNumber;
};

inline void to_json(nlohmann::json& json, const CheckPointData& value) {
    json = nlohmann::json{
        {"position",value.position },
        {"checkPointNumber",value.checkPointNumber },
    };
}

inline void from_json(const nlohmann::json& json, CheckPointData& value) {

    // キーを確認
    if (json.contains("position") && json["position"].is_array() &&
        json.contains("checkPointNumber") && json["checkPointNumber"].is_number_integer()) {

        value.position = json["position"].get<Vector3>();
        value.checkPointNumber = json["checkPointNumber"].get<int32_t>();
    }

}