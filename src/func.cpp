#include <iostream>
#include <random>
#include "curl_http.h"
#include "func.hpp"
#include <algorithm>
#include <chrono>
#include <string>

std::vector<Anime> returnArrayOfAnime(const std::string& client_id, const std::string& userName)
{
    std::string resp = get_list_from_site_with_token(client_id, userName);
    if(resp == "")
    {
        std::vector<Anime> temp;
        std::string er = "No internet connection";
        temp.emplace_back(Anime(er));
        return temp;
    }
    std::string err;
    nlohmann::json j = nlohmann::json::parse(resp);
    while(j["paging"].contains("next"))
    {
    resp = get_list_from_site_with_token(client_id, userName, j["paging"]["next"]);
    nlohmann::json newJ = nlohmann::json::parse(resp);
    j["paging"] = newJ["paging"];
    for(auto test : newJ["data"])
        j["data"].push_back(test);
    }

    if(j.contains("error"))
    {
        std::vector<Anime> temp;
        err = j["error"];
        temp.emplace_back(Anime(err));
        return temp;
    }
    std::vector<Anime> animeList;
    animeList.reserve(1000);
    for(nlohmann::json data : j.at("data"))
    {
            nlohmann::json anInfo = data["node"];
            animeList.emplace_back(Anime(anInfo));
    }
    animeList.shrink_to_fit();
    return animeList;
}


void ShuffleVect(std::vector<Anime>& vect)
{
    static std::default_random_engine rEng;
    static unsigned long int time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rEng.seed(time);
    static std::mt19937 gen(rEng());
    std::shuffle(vect.begin(), vect.end(), gen);
}
std::map<uint16_t, dataStatus> allGenresInObjects(std::vector<Anime> animeData)
{
    std::map<uint16_t, dataStatus> genres;
    for(Anime an : animeData)
    {
        for(auto genre : an.genres)
        {
            dataStatus genData = {genre.second};
            genres.insert({genre.first, genData});
        }
    }
    return genres;
}

void findAndChangeCharInString(std::string& copy, char find, char assign)
{
    for(auto& c : copy)
        if(c == find)
            c = assign;

}

int returnCBoxStatus(HWND hwnd, int menu_num)
{
    return (int) SendMessage(GetDlgItem(hwnd, menu_num), BM_GETCHECK, 0, 0);
}

bool checkAnimeStatus(Anime An, std::map<std::string, int> dict)
{
    return (bool) dict.at(An.status);
}

std::wstring string_to_wstring(const std::string& text)
{
    std::wstring temp(text.begin(), text.end());
    return temp;
}