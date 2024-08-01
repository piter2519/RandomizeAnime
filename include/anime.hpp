#pragma once
    #include <nlohmann/json.hpp>
    #include <iostream>
    #include <map>
class Anime
{
    private:
    int id;
    public:
    std::string title = "";
    std::string picture_url;
    std::string img_path = "";
    std::string status = "";
    float score = 0.0f;
    uint32_t popularity = 0;
    uint32_t numOfEp = 0;
    uint16_t year = 0;
    std::string season = "unknown";
    std::string mediaType = "unknown";
    std::string nsfw = "";
    std::string rating = "unknown";
    std::map<uint16_t, std::string> genres;
    private:
    public:
    Anime();
    Anime(std::string err);
    Anime(nlohmann::json data);
    void printAllData();
    bool genreExist(uint16_t genreId);
    int returnId();
    std::string stringOfGenres();
    std::string stringOfSeasonData();
};

