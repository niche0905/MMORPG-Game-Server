#include <iostream>
#include <fstream>
#include <json/json.h>


int main()
{
    std::ifstream file("my_tile.json", std::ifstream::binary);
    Json::Value root;
    file >> root;

    uint32_t width = root["width"].asUInt();
    uint32_t height = root["height"].asUInt();
    const Json::Value& data = root["layers"][0]["data"];

    std::cout << "Width: " << width << ", Height: " << height << std::endl;
    if (data.size() != (width * height)) {
        std::cout << "Error" << std::endl;
    }

    std::ofstream output("map.bin", std::ios::binary);
    if (not output.is_open()) {
        std::cout << "Error can't open" << std::endl;
        exit(-1);
    }

    output.write(reinterpret_cast<const char*>(&width), sizeof(uint32_t));
    output.write(reinterpret_cast<const char*>(&height), sizeof(uint32_t));

    for (Json::ArrayIndex i = 0; i < data.size(); ++i) {
        uint8_t tile = static_cast<uint8_t>(data[i].asUInt());
        output.write(reinterpret_cast<const char*>(&tile), sizeof(uint8_t));
    }

    output.close();
    std::cout << "Export Success!" << std::endl;

    return 0;
}
