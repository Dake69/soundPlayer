#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to search Spotify using the Web API
void searchSpotify(const std::string& accessToken, const std::string& query) {
    const std::string endpoint = "https://api.spotify.com/v1/search?q=" + query + "&type=track&limit=5";
    CURL* curlHandle = curl_easy_init();

    if (curlHandle) {
        std::string response;
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());

        curl_easy_setopt(curlHandle, CURLOPT_URL, endpoint.c_str());
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);

        CURLcode result = curl_easy_perform(curlHandle);
        if (result != CURLE_OK) {
            std::cerr << "Error: " << curl_easy_strerror(result) << std::endl;
        } else {
            // Parse JSON response
            auto jsonResponse = nlohmann::json::parse(response);
            for (const auto& track : jsonResponse["tracks"]["items"]) {
                std::cout << "Track: " << track["name"] << " by " << track["artists"][0]["name"] << "\n";
            }
        }

        curl_easy_cleanup(curlHandle);
        curl_slist_free_all(headers);
    }
}

int main() {
    std::string accessToken;
    std::cout << "Enter Spotify Access Token: ";
    std::cin >> accessToken;

    std::string query;
    std::cout << "Enter search term: ";
    std::cin >> query;

    searchSpotify(accessToken, query);
    return 0;
}