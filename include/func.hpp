#pragma once
    #include <vector>
    #include "anime.hpp"

    std::vector<Anime> returnTableOfAnime(const std::string& client_id, const std::string& userName);
    void ShuffleVect(std::vector<Anime>& vect);