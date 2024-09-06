#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>  // For std::fixed and std::setprecision

int main() {
    // Variables to hold parsed data
    std::string line;
    std::vector<int> values;
    std::vector<double> times;
    int value;
    double time;

    // Read from standard input (using < input redirection)
    while (std::getline(std::cin, line)) {
        // Parse the "Optimized random solution value" line
        if (line.find("Optimized random solution value:") != std::string::npos) {
            std::istringstream iss(line);
            std::string label;
            iss >> label >> label >> label >> label >> value;  // Skip first parts, get the value
            values.push_back(value);
        }
        // Parse the "Elapsed time" line
        else if (line.find("Elapsed time:") != std::string::npos) {
            std::istringstream iss(line);
            std::string label;
            iss >> label >> label >> time;  // Skip first two parts, get the time
            times.push_back(time);
        }
    }

    // Calculate the averages
    double avgValue = 0.0;
    double avgTime = 0.0;

    for (int v : values) {
        avgValue += v;
    }

    for (double t : times) {
        avgTime += t;
    }

    avgValue /= values.size();
    avgTime /= times.size();

    // Print the results, using fixed notation for the average time
    std::cout << "Average value: " << avgValue << std::endl;
    std::cout << "Average time: " << std::fixed << std::setprecision(6) << avgTime << " seconds" << std::endl;

    return 0;
}