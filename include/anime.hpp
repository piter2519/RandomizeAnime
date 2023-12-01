#pragma once
    #include <nlohmann/json.hpp>
    #include <iostream>
class Anime
{
    private:
    int id;
    public:
    std::string title;
    std::string picture_url;
    private:
    public:
    Anime();
    Anime(std::string err);
    Anime(nlohmann::json data);
    void printAllData();
};