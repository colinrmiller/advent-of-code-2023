// #include "pch.h"

#include "helpers.h"
#include <filesystem>

namespace Helpers
{
    std::string test() {
        return "test";
    }

    std::vector<std::string> ReadAllLinesInFile(const std::filesystem::path& path)
	{
		auto lines = std::vector<std::string>{};
		if (!std::filesystem::exists(path))
		{
			return lines;
		}

		auto fileStream = std::ifstream{ path };
		auto currentLine = std::string{};
		while (std::getline(fileStream, currentLine))
		{
			lines.emplace_back(currentLine);
		}

		return lines;
	}

	std::vector<std::string> splitString(const std::string& input, char delimiter) {
		std::vector<std::string> result;
		std::string temp;

		for (char c : input) {
			if (c != delimiter) {
				temp += c;
			} else {
				result.push_back(temp);
				temp = "";
			}
		}

		if (!temp.empty()) {
			result.push_back(temp);
		}

		return result;
	}


	std::vector<int> ParseStringToIntVector(const std::string& input, char separator) 
	{
		std::vector<int> result;
		std::stringstream ss(input);
		std::string item;

		while (std::getline(ss, item, separator)) {
			try {
				result.push_back(std::stoi(item));
			} catch (...) {
			}
		}

		return result;
	}

	std::vector<long long> ParseStringToLongVector(const std::string& input, char separator) 
	{
		std::vector<long long> result;
		std::stringstream ss(input);
		std::string item;
		while (std::getline(ss, item, separator)) {
			try {
				result.push_back(std::stol(item));
			} catch (...) {
			}
		}

		return result;
	}


	std::vector<int> ParseSeparatedInts(const std::string& line, const char separator = ' ') 
	{
		auto ints = std::vector<int>{};
		auto currentInt = std::string{};
		for (auto i = 0; i < line.size(); ++i)
		{
			if (line[i] == separator)
			{
				ints.emplace_back(std::stoi(currentInt));
				currentInt = std::string{};
			}
			else
			{
				currentInt += line[i];
			}
		}

		if (!currentInt.empty())
		{
			ints.emplace_back(std::stoi(currentInt));
		}

		return ints;
	}
} 