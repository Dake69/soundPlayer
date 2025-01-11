#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Callback function for libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    userData->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    // Test libcurl
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "CURL initialization failed!" << std::endl;
        return 1;
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts/1");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "Response from server: " << response << std::endl;

        // Test nlohmann/json
        auto jsonResponse = nlohmann::json::parse(response);
        std::cout << "Parsed JSON:\n" << jsonResponse.dump(4) << std::endl;
    }

    curl_easy_cleanup(curl);
    return 0;
}