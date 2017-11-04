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