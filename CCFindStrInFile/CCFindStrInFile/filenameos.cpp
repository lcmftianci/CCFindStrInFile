#define _CRT_SECURE_NO_WARNINGS

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

	//鍚堟垚璺緞杩斿洖鍘?
	std::string strFileDir = drive;
	strFileDir += dir;
	return strFileDir;
}

//鍐欏叆鏂囦欢锛屼繚瀛樻闈㈠脊鍑?
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

//渚垮埄绌垮叆鐨勬枃浠跺す灏嗘墍鏈夋枃浠惰矾寰勮緭鍑?
void ReverseDirectory(std::string strPath, std::vector<std::string>& arrFilepath)
{
	//浼犲叆鐨勬枃浠跺す
	CFileFind finder;
	//const char*杞崲鎴怢PCTSTR鏂囦欢
	//杩欓噷鏈潵鑾峰彇鐨勬槸涓�涓枃浠惰矾寰?
	//鏍规嵁鏂囦欢璺緞鎵惧埌鎵�鍦ㄧ洰褰?
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
			//::PostMessage(/*pDlg->m_hWnd*/theApp.GetMainWnd()->m_hWnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_OPEN_VIDEO, BN_CLICKED), NULL);
			//::SendMessage(theApp.GetMainWnd()->m_hWnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_OPEN_VIDEO, BN_CLICKED), NULL);
			ReverseDirectory(strs, arrFilepath);
		}
		CString strFilePath = finder.GetFilePath();
		std::string strFilePaths = W2A(strFilePath);
		arrFilepath.push_back(strFilePaths);
	}
	return;
}


//鏌ユ壘瀛楃涓?
bool FindString(std::string strData, std::string strFilePath)
{
	//鎵撳紑鏂囦欢,鏌ユ壘瀛楃涓诧紝鍏抽棴鏂囦欢杩斿洖鍊?
	FILE* fp = fopen(strFilePath.c_str(), "rt");
	if (fp == NULL)
	{
		//fclose(fp);  鎵撳紑鏂囦欢澶辫触鏃朵笉鑳藉叧鏂囦欢锛屼笉鑳藉拰java涓�鏍?
		return false;
	}

	//鏌ユ壘瀛楃涓?
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
	//鏌ユ壘瀛楃涓插苟灏嗗瓨鍦ㄧ浉搴斿瓧绗︿覆鐨勬枃浠跺啓鍏ュ埌鍐欏叆鍒版寚瀹氬瓧绗︽暟缁?
	std::vector<std::string>::iterator iter = arrSrcPath.begin();
	USES_CONVERSION;
	char* chMsg;
	std::string strDif;
	CString strMsg;
	for (; iter != arrSrcPath.end(); iter++)
	{
		//这里发送消息有点问题，大量数据是崩溃
		/*	strDif = "正在解析-->";
			strDif += *iter;
			chMsg = const_cast<char*>(strDif.c_str());
			strMsg = A2W(chMsg);
			::SendMessage(theApp.GetMainWnd()->m_hWnd, MSG_SHOW_MSG, 0, (LPARAM)(&strMsg));*/
		if (FindString(strStrFind, iter->data()))
		{
			arrDesPath.push_back(iter->data());
		}	
	}
}
