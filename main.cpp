#include <any>
#include <future>
#include <variant>
#include <iostream>
#include <curl/curl.h>
#include <optional>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <sstream>

std::optional<bool> checkTrue(bool returnVal) {

    std::variant<int, float, std::string> var;
    var = 10;
    var = "hej";
    std::string a = std::get<std::string>(var);
    if(returnVal) {
        return true;
    }
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

void getUnpackData(std::string&& file) {

    CURL *curl = NULL;
    FILE *fp;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    CURLcode res;
    int retCode;

    if (curl) {
        std::string url = "https://datasets.imdbws.com/" + file;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        fp = fopen(file.c_str(), "wb");

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS , 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "oops";
        }

        int i = fclose(fp);
        if (i == 0) {
            std::string chmodCmd = "chmod +rx " + file;
            system(chmodCmd.c_str());
            std::string unpackCmd = "gzip -d " + file;
            system(unpackCmd.c_str());
        }
        else {
            std::cerr << "unable to unpack" << std::endl;
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        retCode = static_cast<int>(http_code);
        if(retCode != 200) {
            std::cerr << "HTTP code " << retCode << std::endl;
        }
        curl_easy_cleanup(curl);

    }
    curl_global_cleanup();
}

void parse() {
    std::fstream file;
    file.open("title.akas.tsv", std::ios::in);

    if(!file.is_open()) {
        getUnpackData("title.akas.tsv.gz");
        file.open("title.akas.tsv", std::ios::in);
    }
    std::vector<std::string> row;
    std::string line, word, temp;
    while (file >> temp) {

        row.clear();

        // read an entire row and
        // store it in a string variable 'line'
        getline(file, line);
        if(line.find("The Proposal") == std::string::npos) {
            continue;
        }
        // used for breaking words
        std::stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while (getline(s, word, '\t')) {

            // add all the column data
            // of a row to a vector
            row.push_back(word);
        }
        for(int i = 0; i < row.size() ; i++) {
            std::cout << row[i] << std::endl;
        }
    }
    file.close();
    
}


double Task ()
{
    std::cout << "Async thread running" << std::endl;
    parse();
//    std::any value = "wow";
//    value = 10.0;
//    return std::any_cast<double>(value);
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::string asyncWork = "Testing async";

    std::future<double> fut = std::async(std::launch::async, Task);
    
//    std::optional<bool> opt = checkTrue(false);
//    if (opt.has_value())
//    {
//        std::cout << "opt delivered: " << opt.value() << std::endl;
//    }

    double result = fut.get();
//    std::cout << "Thread result " << result << std::endl;

    return 0;
}


