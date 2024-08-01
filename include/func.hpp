#pragma once
    #include <vector>
    #include "anime.hpp"
    #include <map>

    struct pos_size
    {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
    };
    struct dataStatus
    {
        std::string data = "";
        int status = 0;
    };

std::vector<Anime> returnArrayOfAnime(const std::string& client_id, const std::string& userName);
void ShuffleVect(std::vector<Anime>& vect);
std::map<uint16_t, dataStatus> allGenresInObjects(std::vector<Anime> animeData);


template <typename Key, typename Value>
std::vector<Value> returnArrayOfElements(std::map<Key, Value> dict)
{
    std::vector<Value> data;
    data.reserve(dict.size());
    for(auto pair : dict)
    {
        data.emplace_back(pair.second);
    }
    return data;
}

template <typename Key, typename Value>
std::vector<Key> returnArrayOfKeys(std::map<Key, Value> dict)
{
    std::vector<Key> data;
    data.reserve(dict.size());
    for(auto pair : dict)
    {
        data.emplace_back(pair.first);
    }
    return data;
}
void findAndChangeCharInString(std::string& copy, char find, char assign);

int returnCBoxStatus(HWND hwnd, int menu_num);

bool checkAnimeStatus(Anime An, std::map<std::string, int> dict);

std::wstring string_to_wstring(const std::string& text);