#include <fstream>
#include "curl_http.h"
std::string get_token_from_file(const std::string& filepath)
{
    std::fstream input;
    std::string token, _;
    input.open(filepath, std::ios::in);
    if(!input.good())
    {
        std::cout<<"Open File Error"<<std::endl;
        return "";
    }
    if(input)
        input>>_>>token;
    input.close();
    return token;
}

static size_t curlstr_callback(void *ptr, size_t size, size_t nmemb, std::string* data) 
{
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}

size_t imgWriteData(void* ptr, size_t size, size_t nmemb, void* userData) {
    if (userData) {
        FileData* data = static_cast<FileData*>(userData);
        std::ofstream& fileStream = *(data->file);
        // Zapisywanie danych do strumienia pliku
        fileStream.write(static_cast<const char*>(ptr), size * nmemb);
        // Zwracanie liczby zapisanych bajtów (rozmiar * ilość)
        return size * nmemb;
    }
    return 0;
}

std::string get_list_from_site_with_token(const std::string& client_id, const std::string& userName)
{
    std::string mal_url = "https://api.myanimelist.net/v2/users/" + userName +"/animelist?limit=1000&status=plan_to_watch&nsfw=true&fields=status,media_type,num_episodes,start_season,mean,num_list_users,nsfw,genres,status,rating";
    struct curl_slist* headers = NULL;

    CURL* request = curl_easy_init();
    CURLcode response;
    std::string response_data;
    if(request)
    {
        // std::cout<<client_id<<std::endl;
        std::string auth = "X-MAL-CLIENT-ID: " + client_id;
        headers = curl_slist_append(headers, auth.c_str());
        // std::cout<<headers->data<<std::endl;
        curl_easy_setopt(request, CURLOPT_URL, mal_url.c_str());
        curl_easy_setopt(request, CURLOPT_HTTPHEADER, headers);
        // curl_easy_setopt(request, CURLOPT_SSL_VERIFYPEER, FALSE);
        curl_easy_setopt(request, CURLOPT_CAINFO, "cert/cacert-2023-01-10.pem");
        curl_easy_setopt(request, CURLOPT_CAPATH, "cert/cacert-2023-01-10.pem");
        curl_easy_setopt(request, CURLOPT_WRITEFUNCTION, &curlstr_callback);
        curl_easy_setopt(request, CURLOPT_WRITEDATA, &response_data);
        // curl_easy_setopt(request, CURLOPT_KEYPASSWD, "loooo");

        response = curl_easy_perform(request);
        if (response != CURLE_OK)
        {
            curl_slist_free_all(headers);
            curl_easy_cleanup(request);
            return "";
        }
    }
    curl_slist_free_all(headers);
    curl_easy_cleanup(request);
    return response_data;
}

std::string get_list_from_site_with_token(const std::string& client_id, const std::string& userName, const std::string& url)
{
    struct curl_slist* headers = NULL;

    CURL* request = curl_easy_init();
    CURLcode response;
    std::string response_data;
    if(request)
    {
        // std::cout<<client_id<<std::endl;
        std::string auth = "X-MAL-CLIENT-ID: " + client_id;
        headers = curl_slist_append(headers, auth.c_str());
        // std::cout<<headers->data<<std::endl;
        curl_easy_setopt(request, CURLOPT_URL, url.c_str());
        curl_easy_setopt(request, CURLOPT_HTTPHEADER, headers);
        // curl_easy_setopt(request, CURLOPT_SSL_VERIFYPEER, FALSE);
        curl_easy_setopt(request, CURLOPT_CAINFO, "cert/cacert-2023-01-10.pem");
        curl_easy_setopt(request, CURLOPT_CAPATH, "cert/cacert-2023-01-10.pem");
        curl_easy_setopt(request, CURLOPT_WRITEFUNCTION, &curlstr_callback);
        curl_easy_setopt(request, CURLOPT_WRITEDATA, &response_data);
        // curl_easy_setopt(request, CURLOPT_KEYPASSWD, "loooo");

        response = curl_easy_perform(request);
        if (response != CURLE_OK)
        {
            curl_slist_free_all(headers);
            curl_easy_cleanup(request);
            return "";
        }
    }
    curl_slist_free_all(headers);
    curl_easy_cleanup(request);
    return response_data;
}


static size_t curlimg_callback(void* buf, size_t size, size_t nmemb, void* userp)
{
	if (userp)
	{
		std::ostream& os = *static_cast<std::ostream*>(userp);
		std::streamsize len = size * nmemb;
		if (os.write(static_cast<char*>(buf), len))
			return len;
	}

	return 0;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
   size_t written {fwrite(ptr, size, nmemb, stream)};
   return written;
}

bool curl_save_img(const std::string& url, std::string filePath)
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl = curl_easy_init();
    std::ofstream ofile;
    ofile.open(filePath, std::ios::binary);
    FileData data;
    data.file = &ofile;
	if (curl)
	{
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		// curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cert/cacert-2023-01-10.pem");
        curl_easy_setopt(curl, CURLOPT_CAPATH, "cert/cacert-2023-01-10.pem");
		// curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, imgWriteData);
        // std::cout<<url.c_str()<<std::endl;
		code = curl_easy_perform(curl);
	}
    curl_easy_cleanup(curl);
	if(code != CURLE_OK)
    {
        std::cout<<curl_easy_strerror(code)<<std::endl;
        return false;
    }
    return true;
}

