#include <iostream>     // ǥ�� �����
#include <fstream>      // ���� �����
#include <string>       // ���ڿ� ó��
#include <sstream>      // ���ڿ� ��Ʈ�� ó��
#include <iomanip>      // ��� ������
#include <thread>       // ��Ƽ������
#include <mutex>        // ��Ƽ������ ����ȭ
#include <map>          // ��(Map) ������ ����
#include <vector>       // ����(Vector) ������ ����
#include <algorithm>    // �˰��� �Լ�
#include <numeric>      // ��ġ ��� �Լ�


// Ű-�� ���� �����ϴ� Ŭ����
class KeyValuePair {
public:
    std::string key;    // Ű�� ������ ���ڿ�
    int value;          // ��

    // ������. Ű�� ���� �ʱⰪ���� �޽��ϴ�.
    KeyValuePair(std::string& key, int value) : key(key), value(value) {}
};

// ���ڿ� ó���� ���� �Լ����� �����ϴ� ���ӽ����̽�
namespace StringUtils {

    // �־��� ���ڿ��� �ҹ��ڷ� ��ȯ�ϴ� �Լ�
    std::string toLowerCase(std::string& str) {
        std::string result = str;
        // std::transform �Լ��� std::tolower �Լ� ����
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char ch) { return std::tolower(ch); });
        return result;
    }

    // �־��� ���ڿ����� Ư�� ���ڸ� �������� ��ü�ϴ� �Լ�
    std::string replaceSign(std::string& str) {
        // ���ĺ��̳� ����, ����, �׸��� ���ĺ� ���̿� ��ġ�� ��������ǥ(')�� �ƴϸ� Ư�� ���ڷ� ���
        // ���ĺ� ���̿� ��ġ�� ��������ǥ(')�� ��� ������Ʈ����(���/������ ��)�� ���Ͽ� �������� ����
        auto isSpecialCharacter = [](unsigned char ch, unsigned char prev, unsigned char next) {
            if (std::isalnum(ch) || ch == ' ') {
                return false;
            }

            if (ch == '\'' && std::isalpha(prev) && std::isalpha(next)) {
                return false;
            }

            return true;
        };

        // ���ڿ��� ��� ���ڸ� ��ȸ�ϸ鼭 Ư�� ���ڸ� ã�� �������� ����
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

    // �־��� ���ڿ��� Ư�� �����ڸ� �������� �����ϴ� �Լ�
    std::vector<std::string> split(std::string& str, char ch) {
        // �Է¹��� ���ڿ��� stringstream�� �����մϴ�.
        std::istringstream ss(str);
        // std::istream_iterator�� �̿��Ͽ� ���ڿ��� ������ �������� �����մϴ�.
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

    virtual void mapper(std::string& block, int threadId) = 0; // ���� �Լ��� ����
    virtual std::map<std::string, int> reducer() = 0; // ���� �Լ��� ����

    // �� ������ ��Ͽ� ���� mapper �Լ��� ���ķ� �����ϴ� �Լ�
    void createAndJoinThreads(std::vector<std::string>& blocks) {
        std::vector<std::thread> threads;
        threadKeyValuePairs.resize(blocks.size());

        // �� ��Ͽ� ���� ���� �Լ��� �����ϴ� �����带 ����
        for (int i = 0; i < blocks.size(); ++i) {
            threads.emplace_back([this, &blocks, i]() {
                this->mapper(blocks[i], i);
                });
        }

        // ������ ��� �����尡 �۾��� �Ϸ��� ������ ���
        for (auto& thread : threads) {
            thread.join();
        }
    }

    // ������ Ű�� ���� Ű-�� ���� �׷�ȭ�ϴ� �Լ�
    void groupByKey() {
        for (auto& pair : keyValuePairs) {
            groupedValues[pair.key].push_back(pair.value);
        }
    }



    // ����� ��� ���Ͽ� �����ϴ� �Լ�
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
        else { // ���� ���⿡ ���� ����ó��
            std::cerr << "Unable to open output file." << std::endl;
        }
    }

public:
    // ������ ����� ó���ϴ� �Լ� createAndJoinThreads, groupByKey, reducer �Լ��� ���������� ȣ��
    void processBlocks(std::vector<std::string>& blocks) {
        createAndJoinThreads(blocks);

        // �� �����尡 ������ Ű-�� �ֵ��� �ϳ��� ����Ʈ ����
        for (const auto& pairs : threadKeyValuePairs) {
            keyValuePairs.insert(keyValuePairs.end(), pairs.begin(), pairs.end());
        }

        groupByKey();
        auto sums = reducer();
        saveResult(sums);
    }

    // �Է� ������ �а�, ������ ������ ���� ������ ������� �����ϴ� �Լ�
    std::vector<std::string> fileToBlock(const std::string& filename) {
        std::ifstream fin;
        fin.open(filename);

        if (fin.fail()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return {};
        }

        int cpuCores = std::thread::hardware_concurrency(); // CPU �ھ� ����
        fin.seekg(0, std::ios::end);
        int fileSize = fin.tellg(); // ���� ũ��
        fin.seekg(0, std::ios::beg);

        int blockCount; // ��� ����, ������ ũ��� CPU �ھ��� ������ ���� ����
        if (fileSize < 1024 * 10) {
            blockCount = std::max(std::min(fileSize / 100, cpuCores), 1);
        }
        else {
            blockCount = (fileSize < 1024 * 1024) ? cpuCores
                : (fileSize < 10 * 1024 * 1024) ? cpuCores * 2
                : cpuCores * 4;
        }

        int blockSize = fileSize / blockCount; // ��� ũ��
        std::vector<std::string> blocks;

        std::string line;
        std::string blockData;
        int currentBlockSize = 0;
        while (getline(fin, line)) {
            // ���ο��� Ư�����ڸ� �����ϰ� �ҹ��ڷ� ��ȯ
            line = StringUtils::replaceSign(line);
            line = StringUtils::toLowerCase(line);
            // ���� ����� ����� ������Ʈ
            currentBlockSize += line.size();
            blockData += line + " ";

            if (currentBlockSize >= blockSize) {
                // �ܾ��� �п��� �����ϱ� ���Ͽ� ���������� �����ϴ� ������ ��ġ Ž��
                size_t lastSpace = blockData.find_last_of(' ');
                // ó������ ������ ��������� ���ڿ��� ���ο� ��� �����ͷ� ����
                std::string blockDataToProcess = blockData.substr(0, lastSpace);
                // ���� ���ڿ�(������ ���� ���� ���ڿ�)�� ���� ����� ���� �κ����� ����
                blockData = blockData.substr(lastSpace + 1);

                // ���ο� ��� �����͸� ��� ���Ϳ� �߰�
                blocks.push_back(blockDataToProcess);
                // ���� ��� ũ�⸦ ���ο� ��� �������� ũ��� ������Ʈ
                currentBlockSize = blockData.size();
            }
        }

        // ���������� ���� ����� �ִٸ� ���Ϳ� �߰�
        if (!blockData.empty()) {
            blocks.push_back(blockData);
        }

        fin.close();

        return blocks;
    }
};

// MapReduce�� ��ӹ޴� WordCount Ŭ����
class WordCount : public MapReduce {
protected:
    // ������ ��Ͽ��� �ܾ �����ϰ�, �� �ܾ ���� Ű-�� ���� �����ϴ� �Լ�
    // ��Ƽ������ ȯ�濡�� �۵��ϸ�, �� ������� ������ threadId�� ����
    void mapper(std::string& block, int threadId) override {
        std::vector<std::string> words = StringUtils::split(block, ' ');

        // �� �ܾ ���� Ű-�� ���� ���� Ű= �ܾ� �� = 1
        for (auto& word : words) {
            if (word != "") {
                // ��Ƽ������ ȯ�濡�� ���� �����Ϳ� ���� ���� ������ �����ϱ� ���� ���ؽ��� ���
                mtx.lock();
                threadKeyValuePairs[threadId].emplace_back(word, 1);
                mtx.unlock();
            }
        }
    }

    // �׷�ȭ�� Ű-�� �ֿ� ���� �� �׷��� ���� �ջ��ϴ� �Լ� 
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
            break; // ���� �б⿡ �����ϸ� while ������ �������ɴϴ�.
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