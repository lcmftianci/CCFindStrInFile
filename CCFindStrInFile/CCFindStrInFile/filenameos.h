#pragma once
#include <iostream>
#include <vector>
#include <afx.h>
#include <atlbase.h>
#include <stdlib.h>

void ReverseDirectory(std::string strPath, std::vector<std::string>& arrFilepath);

std::string GetFileDirectory(std::string strFilePath);
