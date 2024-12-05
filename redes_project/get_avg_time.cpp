#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iomanip>

namespace fs = std::filesystem; // Alias for convenience
using namespace std;

// Function to extract the "Time taken" value from a file
double extractTimeFromFile(const string& filepath) {
    ifstream file(filepath);
    if (!file) {
        cerr << "Error: Cannot open file " << filepath << endl;
        return -1.0;
    }

    string line;
    double timeTaken = 0.0;

    // Read through the file to find the "Time taken" line
    while (getline(file, line)) {
        if (line.rfind("Time taken:", 0) == 0) { // Line starts with "Time taken:"
            size_t pos = line.find(":");
            if (pos != string::npos) {
                timeTaken = stod(line.substr(pos + 1)); // Convert to double
            }
            break;
        }
    }

    file.close();
    return timeTaken;
}

// Function to find the common string among file names
string findCommonString(const vector<string>& fileNames) {
    if (fileNames.empty()) return "";

    string common = fileNames[0];
    for (const auto& name : fileNames) {
        size_t i = 0;
        while (i < common.size() && i < name.size() && common[i] == name[i]) {
            ++i;
        }
        common = common.substr(0, i); // Update common prefix
        if (common.empty()) break;   // No common prefix
    }

    return common;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <directory_path>" << endl;
        return 1;
    }

    string dirPath = argv[1];
    vector<string> fileNames;
    double totalTime = 0.0;
    int fileCount = 0;

    // Iterate over files in the directory
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            string filepath = entry.path().string();
            fileNames.push_back(entry.path().filename().string());

            // Extract time from the file
            double time = extractTimeFromFile(filepath);
            if (time >= 0.0) {
                totalTime += time;
                ++fileCount;
            }
        }
    }

    if (fileCount == 0) {
        cerr << "No valid files found in the directory." << endl;
        return 1;
    }

    // Calculate average time
    double averageTime = totalTime / fileCount;

    // Find common string among file names
    string commonString = findCommonString(fileNames);

    // Create the output file in the same directory
    string outputFileName = dirPath + "/average_" + commonString + ".txt";
    ofstream outputFile(outputFileName);
    if (!outputFile) {
        cerr << "Error: Cannot create file " << outputFileName << endl;
        return 1;
    }

    // Write the average time to the output file
    outputFile << "Average time taken: " << fixed << setprecision(6) << averageTime << " seconds." << endl;

    outputFile.close();

    cout << "Average time written to file: " << outputFileName << endl;

    return 0;
}
