#include <iostream>
#include <thread>
#include <chrono>
#include <curl/curl.h>
#include <mutex>
#include <nlohmann/json.hpp> 

std::mutex dataMutex;          
std::string sharedTimeData;    


size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}


std::string fetchCurrentTime() {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https:
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        
        try {
            auto jsonResponse = nlohmann::json::parse(readBuffer);
            return jsonResponse["datetime"];
        } catch(const std::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << '\n';
            return "";
        }
    }
    return "";
}

void updateTimeData() {
    while (true) {
        std::string currentTime = fetchCurrentTime();
        {
            std::lock_guard<std::mutex> guard(dataMutex);
            sharedTimeData = currentTime;
        }
        std::this_thread::sleep_for(std::chrono::seconds(6));
    }
}


void displayTimeData() {
    while (true) {
        std::string currentTime;
        {
            std::lock_guard<std::mutex> guard(dataMutex);
            currentTime = sharedTimeData;
        }
        if (!currentTime.empty()) {
            std::cout << "Current time is: " << currentTime << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main() {
    std::thread timeUpdater(updateTimeData);
    std::thread timeDisplayer(displayTimeData);

    timeUpdater.join();
    timeDisplayer.join();

    return 0;
}
