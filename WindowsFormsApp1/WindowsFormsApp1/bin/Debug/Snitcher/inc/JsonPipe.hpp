//
// Created by Oladushek on 3/17/2021.
//
#pragma once
#include <nlohmann/json.hpp>

using Json = nlohmann::json;

template<typename T>
T ExtractValue(const Json &data, std::string key) {
    if (!data.contains(key))
        throw sv::Exception{std::string{"Key not found: "} + key, __FILE__, __LINE__};

    try {
        return data[key];
    } catch (const std::exception&)
    {
        throw sv::Exception{std::string{"Wrong expected type "} + key, __FILE__, __LINE__};
    }
    return data[key];
}
