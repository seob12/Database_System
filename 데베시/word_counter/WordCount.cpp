#include <iostream>     // 표준 입출력
#include <fstream>      // 파일 입출력
#include <string>       // 문자열 처리
#include <sstream>      // 문자열 스트림 처리
#include <iomanip>      // 출력 포맷팅
#include <thread>       // 멀티스레딩
#include <mutex>        // 멀티스레딩 동기화
#include <map>          // 맵(Map) 데이터 구조
#include <vector>       // 벡터(Vector) 데이터 구조
#include <algorithm>    // 알고리즘 함수
#include <numeric>      // 수치 계산 함수


// 키-값 쌍을 저장하는 클래스
class KeyValuePair {
public:
    std::string key;    // 키를 저장할 문자열
    int value;          // 값

    // 생성자. 키와 값을 초기값으로 받습니다.
    KeyValuePair(std::string& key, int value) : key(key), value(value) {}
};

// 문자열 처리를 위한 함수들을 포함하는 네임스페이스
namespace StringUtils {

    // 주어진 문자열을 소문자로 변환하는 함수
    std::string toLowerCase(std::string& str) {
        std::string result = str;
        // std::transform 함수와 std::tolower 함수 적용
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char ch) { return std::tolower(ch); });
        return result;
    }

    // 주어진 문자열에서 특수 문자를 공백으로 대체하는 함수
    std::string replaceSign(std::string& str) {
        // 알파벳이나 숫자, 공백, 그리고 알파벳 사이에 위치한 작은따옴표(')가 아니면 특수 문자로 취급
        // 알파벳 사이에 위치한 작은따옴표(')의 경우 아포스트로피(축약/소유격 등)을 위하여 제거하지 않음
        auto isSpecialCharacter = [](unsigned char ch, unsigned char prev, unsigned char next) {
            if (std::isalnum(ch) || ch == ' ') {
                return false;
            }

            if (ch == '\'' && std::isalpha(prev) && std::isalpha(next)) {
                return false;
            }

            return true;
        };

        // 문자열의 모든 문자를 순회하면서 특수 문자를 찾아 공백으로 변경
        for (std::string::size_type i = 0; i < str.size(); ) {
            unsigned char prev = i > 0 ? str[i - 1] : ' ';
            unsigned char next = i < str.size() - 1 ? str[i + 1] : ' ';

            if (isSpecialCharacter(str[i], prev, next)) {
                str[i] = ' ';
            }
            ++i;
        }

        return str;
    }

    // 주어진 문자열을 특정 구분자를 기준으로 분할하는 함수
    std::vector<std::string> split(std::string& str, char ch) {
        // 입력받은 문자열을 stringstream에 저장합니다.
        std::istringstream ss(str);
        // std::istream_iterator를 이용하여 문자열을 공백을 기준으로 분할합니다.
        std::vector<std::string> result{
            std::istream_iterator<std::string>{ss},
            std::istream_iterator<std::string>{}
        };

        return result;
    }
}

class MapReduce {
protected:
    std::vector<std::vector<KeyValuePair>> threadKeyValuePairs;
    std::vector<KeyValuePair> keyValuePairs;
    std::map<std::string, std::vector<int>> groupedValues;
    std::mutex mtx;

    virtual void mapper(std::string& block, int threadId) = 0; // 가상 함수로 정의
    virtual std::map<std::string, int> reducer() = 0; // 가상 함수로 정의

    // 각 데이터 블록에 대해 mapper 함수를 병렬로 실행하는 함수
    void createAndJoinThreads(std::vector<std::string>& blocks) {
        std::vector<std::thread> threads;
        threadKeyValuePairs.resize(blocks.size());

        // 각 블록에 대해 매퍼 함수를 실행하는 스레드를 생성
        for (int i = 0; i < blocks.size(); ++i) {
            threads.emplace_back([this, &blocks, i]() {
                this->mapper(blocks[i], i);
                });
        }

        // 생성한 모든 스레드가 작업을 완료할 때까지 대기
        for (auto& thread : threads) {
            thread.join();
        }
    }

    // 동일한 키를 가진 키-값 쌍을 그룹화하는 함수
    void groupByKey() {
        for (auto& pair : keyValuePairs) {
            groupedValues[pair.key].push_back(pair.value);
        }
    }



    // 결과를 출력 파일에 저장하는 함수
    void saveResult(std::map<std::string, int>& sums) {
        std::ofstream outputFile("result.txt");
        if (outputFile.is_open()) {

            system("cls");
            std::string separator(16, '-');
            outputFile << separator << std::endl;
            std::cout << separator << std::endl;

            for (auto& pair : sums) {
                outputFile << "Key:  " << pair.first << std::endl;
                outputFile << "Total count: " << pair.second << std::endl;
                outputFile << separator << std::endl;

                std::cout << "Key:  " << pair.first << std::endl;
                std::cout << "Total count: " << pair.second << std::endl;
                std::cout << separator << std::endl;
            }
            outputFile.close();
        }
        else { // 파일 열기에 실패 예외처리
            std::cerr << "Unable to open output file." << std::endl;
        }
    }

public:
    // 데이터 블록을 처리하는 함수 createAndJoinThreads, groupByKey, reducer 함수를 순차적으로 호출
    void processBlocks(std::vector<std::string>& blocks) {
        createAndJoinThreads(blocks);

        // 각 스레드가 생성한 키-값 쌍들을 하나의 리스트 생성
        for (const auto& pairs : threadKeyValuePairs) {
            keyValuePairs.insert(keyValuePairs.end(), pairs.begin(), pairs.end());
        }

        groupByKey();
        auto sums = reducer();
        saveResult(sums);
    }

    // 입력 파일을 읽고, 파일의 내용을 여러 데이터 블록으로 분할하는 함수
    std::vector<std::string> fileToBlock(const std::string& filename) {
        std::ifstream fin;
        fin.open(filename);

        if (fin.fail()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return {};
        }

        int cpuCores = std::thread::hardware_concurrency(); // CPU 코어 개수
        fin.seekg(0, std::ios::end);
        int fileSize = fin.tellg(); // 파일 크기
        fin.seekg(0, std::ios::beg);

        int blockCount; // 블록 개수, 파일의 크기와 CPU 코어의 개수에 따라 변경
        if (fileSize < 1024 * 10) {
            blockCount = std::max(std::min(fileSize / 100, cpuCores), 1);
        }
        else {
            blockCount = (fileSize < 1024 * 1024) ? cpuCores
                : (fileSize < 10 * 1024 * 1024) ? cpuCores * 2
                : cpuCores * 4;
        }

        int blockSize = fileSize / blockCount; // 블록 크기
        std::vector<std::string> blocks;

        std::string line;
        std::string blockData;
        int currentBlockSize = 0;
        while (getline(fin, line)) {
            // 라인에서 특수문자를 제거하고 소문자로 변환
            line = StringUtils::replaceSign(line);
            line = StringUtils::toLowerCase(line);
            // 현재 블록의 사이즈를 업데이트
            currentBlockSize += line.size();
            blockData += line + " ";

            if (currentBlockSize >= blockSize) {
                // 단어의 분열을 방지하기 위하여 마지막으로 등장하는 공백의 위치 탐색
                size_t lastSpace = blockData.find_last_of(' ');
                // 처음부터 마지막 공백까지의 문자열을 새로운 블록 데이터로 저장
                std::string blockDataToProcess = blockData.substr(0, lastSpace);
                // 남은 문자열(마지막 공백 다음 문자열)은 다음 블록의 시작 부분으로 설정
                blockData = blockData.substr(lastSpace + 1);

                // 새로운 블록 데이터를 블록 벡터에 추가
                blocks.push_back(blockDataToProcess);
                // 현재 블록 크기를 새로운 블록 데이터의 크기로 업데이트
                currentBlockSize = blockData.size();
            }
        }

        // 마지막으로 남은 블록이 있다면 벡터에 추가
        if (!blockData.empty()) {
            blocks.push_back(blockData);
        }

        fin.close();

        return blocks;
    }
};

// MapReduce를 상속받는 WordCount 클래스
class WordCount : public MapReduce {
protected:
    // 데이터 블록에서 단어를 추출하고, 각 단어에 대한 키-값 쌍을 생성하는 함수
    // 멀티스레드 환경에서 작동하며, 각 스레드는 고유한 threadId를 가짐
    void mapper(std::string& block, int threadId) override {
        std::vector<std::string> words = StringUtils::split(block, ' ');

        // 각 단어에 대해 키-값 쌍을 생성 키= 단어 값 = 1
        for (auto& word : words) {
            if (word != "") {
                // 멀티스레드 환경에서 공유 데이터에 대한 동시 접근을 방지하기 위해 뮤텍스를 사용
                mtx.lock();
                threadKeyValuePairs[threadId].emplace_back(word, 1);
                mtx.unlock();
            }
        }
    }

    // 그룹화된 키-값 쌍에 대해 각 그룹의 값을 합산하는 함수 
    std::map<std::string, int> reducer() override {
        std::map<std::string, int> sums;
        for (auto& pair : groupedValues) {
            int sumVal = std::accumulate(pair.second.begin(), pair.second.end(), 0);
            sums[pair.first] = sumVal;
        }
        return sums;
    }
};


int main() {
    MapReduce* mr = new WordCount();
    //WordCount mr;

    std::string filename;
    std::vector<std::string> blocks;

    while (true) {
        try {
            std::cout << "Enter the filename (without extension): ";
            std::cin >> filename;
            filename += ".txt";

            blocks = mr->fileToBlock(filename);
            break; // 파일 읽기에 성공하면 while 루프를 빠져나옵니다.
        }
        catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            std::cerr << "Please try again." << std::endl;
        }
    }

    mr->processBlocks(blocks);
    std::cout << "Processing completed.";

    return 0;
}