#include "stdafx.h"
#include "filenameos.h"
#include "CCFindStrInFile.h"


std::string GetFileDirectory(std::string strFilePath)
{
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath(strFilePath.c_str(), drive, dir, fname, ext);

	//合成路径返回去
	std::string strFileDir = drive;
	strFileDir += dir;
	return strFileDir;
}

//写入文件，保存桌面弹出
int DisInFile(std::vector<std::string> arrSrcPath)
{
	TCHAR MyDir[_MAX_PATH];
	SHGetSpecialFolderPath(theApp.GetMainWnd()->m_hWnd, MyDir, CSIDL_DESKTOP, 0);
	std::string strFile = "";
	sprintf(const_cast<char*>(strFile.c_str()), "%s", MyDir);
	strFile += "\\result.txt";
	//FILE* fp = fopen(strFile.c_str(), "wt");
	//if (fp == NULL)
	//	AfxMessageBox(_T("Open File Error in read Result"));
	
	if (arrSrcPath.size() <= 0)
	{
		AfxMessageBox(_T("No File"));
		return 0;
	}


	//write file
	AllocConsole();
	freopen(strFile.c_str(), "wt", stdout);
	std::vector<std::string>::iterator iter = arrSrcPath.begin();
	for (; iter != arrSrcPath.end(); iter++)
	{
		printf("%s\n", iter->data());
	}
	fclose(stdout);
	FreeConsole();
	std::string strCmd = "notepad.exe ";
	strCmd += strFile;
	WinExec(strCmd.c_str(), SW_SHOW);
	return (int)arrSrcPath.size();
}

//便利穿入的文件夹将所有文件路径输出
void ReverseDirectory(std::string strPath, std::vector<std::string>& arrFilepath)
{
	//传入的文件夹
	CFileFind finder;
	//const char*转换成LPCTSTR文件
	//这里本来获取的是一个文件路径
	//根据文件路径找到所在目录
	std::string strFilePathG = GetFileDirectory(strPath);
	strFilePathG += "*.*";

	// = (LPCTSTR)strFilePathG.c_str();
	CString strFilePathW;
	char* p = const_cast<char*>(strFilePathG.c_str());
	//strFilePathW.Format("%s", p);//error C2664: 'void ATL::CStringT<wchar_t,StrTraitMFC_DLL<wchar_t,ATL::ChTraitsCRT<wchar_t>>>::Format(UINT,...)': cannot convert argument 1 from 'const char [3]' to 'const wchar_t *'
	strFilePathW = p;
	BOOL bResult = finder.FindFile(strFilePathW);
	while (bResult)
	{
		bResult = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		USES_CONVERSION;
		if (finder.IsDirectory())
		{
			CString str = finder.GetFilePath();
			std::string strs = W2A(str);
			strs += "\\";
			::SendMessage(theApp.GetMainWnd()->m_hWnd, MSG_SHOW_MSG, 0, (LPARAM)&str);
			ReverseDirectory(strs, arrFilepath);
		}
		CString strFilePath = finder.GetFilePath();
		std::string strFilePaths = W2A(strFilePath);
		arrFilepath.push_back(strFilePaths);
	}
	return;
}


//查找字符串
bool FindString(std::string strData, std::string strFilePath)
{
	//打开文件,查找字符串，关闭文件返回值
	FILE* fp = fopen(strFilePath.c_str(), "rt");
	if (fp == NULL)
{
fclose(fp);
		return false;
}

	//查找字符串
	char chValue[2048] = "";
	while (!feof(fp))
	{
		fgets(chValue, 2047, fp);
		if (strstr(chValue, strData.c_str()) != NULL)
{
fclose(fp);
			return true;
}
	}

fclose(fp);
	return false;
}

void FindString(std::vector<std::string>& arrDesPath, std::vector<std::string> arrSrcPath, std::string strStrFind)
{
	//查找字符串并将存在相应字符串的文件写入到写入到指定字符数组
	std::vector<std::string>::iterator iter = arrSrcPath.begin();
	for (; iter != arrSrcPath.end(); iter++)
	{
		if (FindString(strStrFind, iter->data()))
		{
			arrDesPath.push_back(iter->data());
			::SendMessage(theApp.GetMainWnd()->m_hWnd, MSG_SHOW_MSG, 0, (LPARAM)((*iter).c_str()));
		}	
	}
}
