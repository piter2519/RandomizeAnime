#pragma once
    #include <nlohmann/json.hpp>
    #include <iostream>
    #include <unordered_map>
class Anime
{
    private:
    int id;
    public:
    std::string title = "";
    std::string picture_url;
    std::string status = "";
    std::string start_date = "";
    std::string end_date = "";
    float score = 0.0f;
    uint32_t popularity = 0;
    std::string nsfw = "";
    std::string rating = "";
    std::unordered_map<uint32_t, std::string> genres;
    private:
    public:
    Anime();
    Anime(std::string err);
    Anime(nlohmann::json data);
    void printAllData();
};