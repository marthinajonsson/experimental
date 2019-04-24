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

#include <assert.h>
template<typename T>
void pop_front(std::vector<T>& vec)
{
    assert(!vec.empty());
    vec.erase(vec.begin());
}

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

#include <algorithm>
void getUnpackData(std::string& file) {

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

#include "FileStructure.h"
std::vector<std::string> parse(std::string &&decompressedFilename, std::string &&findStr)
{
    std::fstream file;
    std::string compressedFilename = decompressedFilename + ".gz";
    file.open(decompressedFilename, std::ios::in);

    if(!file.is_open()) {
        getUnpackData(compressedFilename);
        file.open(decompressedFilename, std::ios::in);
    }
    std::vector<std::string> row;

    std::string line, word, temp;

    std::string header;
    getline(file, header);

    std::stringstream s(header);

    while (getline(s, word, '\t')) {
        row.emplace_back(word);
    }
    row.emplace_back("HEADERSTOP");
    while (!file.eof()) {

        getline(file, line);

        if( std::search(line.begin(), line.end(),
                        findStr.begin(), findStr.end()) == line.end()){
            continue;
        }

        std::stringstream s(line);

        while (getline(s, word, '\t')) {
            row.emplace_back(word);
        }
    }
    file.close();

    return row;
}


double Task ()
{
    FileStructure filestructure;
    std::cout << "Async thread running" << std::endl;
    std::vector<std::string> result = parse("title.akas.tsv", "The Proposal");

    std::string column;
    int numColumns = 0;
    while(column != "HEADERSTOP") {
        column = result.front();
        std::pair<std::string, std::string> pair = {column, ""};
        filestructure.m_colData.emplace_back(pair);
        pop_front(result);
        numColumns++;
    }

    filestructure.m_colData.erase(filestructure.m_colData.end());
    result.erase(result.end());

    std::vector<std::pair<std::string,std::string>> temp (filestructure.m_colData);
    auto it = result.begin();

    while(it != result.end())
    {
        filestructure.m_colData.resize(0);
        std::vector<std::pair<std::string, std::string>>::iterator fileIt = filestructure.m_colData.begin();

        for(auto tempIt = temp.begin(); tempIt != temp.end(); it++, tempIt++, fileIt++) {
            fileIt->first = tempIt->first;
            fileIt->second = it != result.end() ? *it : "\N";
            filestructure.m_colData.emplace_back(fileIt->first, fileIt->second);
        }

        filestructure.m_listResult.emplace_back(filestructure.m_colData);
    }

    for(auto &element : filestructure.m_listResult) {
        auto found = std::find_if(element.begin(), element.end(), [&] (const std::pair<std::string, std::string>& p) { return p.first == "titleId"; });

        std::cout << found->first << ": " << found->second << std::endl;
    }
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


