#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <cstdlib>
#include <ctime>

void makeSequences(std::vector<std::vector<int> >& sequences);

const int NUM_PAGES = 250;
const int NUM_SEQUENCES = 100;
const int SEQUENCE_LEN = 1000;
const int MAX_FRAMES_TO_TEST = 100;

int main() {
    std::vector<std::vector<int> > sequences(NUM_SEQUENCES, std::vector<int>(SEQUENCE_LEN));
    makeSequences(sequences);
    bool inMemoryTable[NUM_PAGES];
    for (int i = 0; i < NUM_PAGES; i++) {
        inMemoryTable[i] = false;
    }
    int results[NUM_SEQUENCES][MAX_FRAMES_TO_TEST];
    std::queue<int> inMemoryQueue;

    int seq_num = 1;
    for (auto&& sequence : sequences) {
        for (int max_frames = 1; max_frames <= MAX_FRAMES_TO_TEST; max_frames++) {
            for (int seq_index = 0; seq_index < SEQUENCE_LEN; seq_index++) {
                if (seq_index == 0) {
                    results[seq_num - 1][max_frames - 1] = 0;
                }
                // Check if page number is in inMemoryTable
                // -1 because the pages start at 1 but the indices of inMemoryTable start at 0
                if (!inMemoryTable[sequence[seq_index] - 1]) {
                    inMemoryQueue.push(sequence[seq_index]);
                    inMemoryTable[sequence[seq_index] - 1] = true;
                    results[seq_num - 1][max_frames - 1]++;
                    if (inMemoryQueue.size() > max_frames) {
                        inMemoryTable[inMemoryQueue.front() - 1] = false;
                        inMemoryQueue.pop();
                    }
                }
            }
            // Empty queue
            std::queue<int> empty;
            std::swap(inMemoryQueue, empty);
            // Set all values in inMemoryTable to false
            for (int i = 0; i < NUM_PAGES; i++) {
                inMemoryTable[i] = false;
            }
        }
        seq_num++;
    }
    std::cout << "Length of memory reference string: 1000" << std::endl;
    std::cout << "Maximum frames of physical memory: 100" << std::endl << std::endl;

    int anomalyCount = 0;
    // Test results and make output
    for (int i = 0; i < NUM_SEQUENCES; i++) {
        int prevFaultCount = 0;
        for (int j = (MAX_FRAMES_TO_TEST - 1); j >= 0; j--) {
            if (prevFaultCount > results[i][j]) {
                anomalyCount++;
                std::cout << "Anomaly Discovered!" << std::endl;
                std::cout << "    " << "Sequence: " << std::to_string(i + 1) << std::endl;
                std::cout << "    " << "Page Faults: " << prevFaultCount << " @ Frame Size: " << j << std::endl;
                std::cout << "    " << "Page Faults: " << results[i][j] << " @ Frame Size: ";
                std::cout << std::to_string(j + 1) << std::endl;
            }
            prevFaultCount = results[i][j];
        }
    }
    std::cout << std::endl << "Anomaly detected " << anomalyCount << " times." << std::endl;
    return 0;
}

void makeSequences(std::vector<std::vector<int> >& sequences) {
    std::srand(std::time(NULL));
    for (auto& sequence : sequences) {
        for (auto& pageNum : sequence) {
            // Insert a random page number from 1 to NUM_PAGES to sequence
            pageNum = (std::rand() % NUM_PAGES) + 1;
        }
    }
}