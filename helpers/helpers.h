#pragma once

#include "pch.h"

namespace Helpers
{
    std::string test();

    std::vector<std::string> ReadAllLinesInFile(const std::filesystem::path& path);
    
    std::vector<std::string> splitString(const std::string& input, char delimiter = ' ');

    std::vector<int> ParseStringToIntVector(const std::string& input, char separator = ' ');
	
    std::vector<long long> ParseStringToLongVector(const std::string& input, char separator = ' ');
}