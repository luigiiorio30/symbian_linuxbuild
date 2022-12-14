/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


#ifndef __PKGFILEPARSER_H__
#define __PKGFILEPARSER_H__

#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>

#include <sstream>
#include <string>
#include <list>
#include <map>
#undef _L

#include "pkglanguage.h"

int CompareTwoString(wchar_t const * string ,wchar_t const * option);
int CompareNString(wchar_t const * string ,wchar_t const * option, int len);


typedef class PkgParser PKGPARSER, *PPKGPARSER;

//Data structures for pkg file parsing
#define EOF_TOKEN			0
#define NUMERIC_TOKEN		1
#define ALPHA_TOKEN			2
#define QUOTED_STRING_TOKEN	3
#define AND_TOKEN			4
#define OR_TOKEN			5
#define NOT_TOKEN			6
#define EXISTS_TOKEN		7
#define DEVCAP_TOKEN		8
#define APPCAP_TOKEN		9
#define GE_TOKEN			10
#define LE_TOKEN			11
#define NE_TOKEN			12
#define IF_TOKEN			13
#define ELSEIF_TOKEN		14
#define ELSE_TOKEN			15
#define ENDIF_TOKEN			16
#define TYPE_TOKEN			17
#define KEY_TOKEN			18
#define LAST_TOKEN			18

#define MAX_STRING 255

typedef union _tag_VARIANTVAL
{
	long dwNumber;					// numeric value, e.g. 100
	wchar_t pszString[MAX_STRING];	// string value, e.g. "crystal"
}VARIANTVAL;

//Data structures to store the pkg file contents
/**
Supported package body statements
*/
typedef enum cmd_type {IF, ELSEIF, ELSE, ENDIF, INSTALLFILE, PACKAGE} CMD_TYPE;

/**
Structure to store the language details
*/
typedef struct _tag_LangList
{
	String langName;  // Language Name
	unsigned long langCode; // Language code
	unsigned long dialectCode; // Dialect code
}LANG_LIST, *PLANG_LIST;

/**
Structure to store the package file header details
*/
typedef struct _tag_Pkgheader
{
	std::list<String> pkgNameList;
	unsigned long pkgUid;
	int vMajor;
	int vMinor;
	int vBuild;
	String pkgType;
}PKG_HEADER, *PPKG_HEADER;

/**
Structure to store the installable file list
*/
typedef struct _tag_InstallFileList
{
	int langDepFlg;
	int pkgFlg;
	std::list<String> srcFiles;
	String destFile;
}INSTALLFILE_LIST, *PINSTALLFILE_LIST;

/**
Structure to store the package body details
*/
typedef struct _tag_CmdBlock
{
	CMD_TYPE cmdType; // Command type
	String cmdExpression; // Expression
	PINSTALLFILE_LIST iInstallFileList; // Installable file details
}CMD_BLOCK, *PCMD_BLOCK;

typedef std::list<PLANG_LIST> LANGUAGE_LIST;
typedef std::list<String> SISFILE_LIST, FILE_LIST;
typedef std::list<PCMD_BLOCK> CMDBLOCK_LIST;

/** 
class PkgParser
	Parses the package file generated by DUMPSIS tool

@internalComponent
@released
*/
class PkgParser
{
public:
	PkgParser(String aFile);
	~PkgParser();

	void ParsePkgFile();
	void GetEmbeddedSisList(SISFILE_LIST& embedSisList);
	void GetInstallOptions(FILE_LIST& aOptions);
	void GetLanguageList(LANGUAGE_LIST& langList);
	void GetHeader(PKG_HEADER& pkgHeader);
	void GetCommandList(CMDBLOCK_LIST& cmdList);
	String GetPkgFileName()
	{
		return iPkgFile;
	}

private:
	int OpenFile();
	void DeleteAll();

	FILE * iPkgHandle;

	LANGUAGE_LIST iLangList;
	PKG_HEADER iPkgHeader;
	SISFILE_LIST iEmbedSisFiles;
	FILE_LIST iInstallOptions;
	CMDBLOCK_LIST iPkgBlock;

	String iPkgFile;

	//Parser Methods
	void AddLanguage(String aLang, unsigned long aCode, unsigned long aDialect);
	void GetNextChar();
	void GetNextToken();
	bool GetStringToken();
	unsigned short ParseEscapeChars();
	void GetAlphaNumericToken();
	bool IsNumericToken();
	void GetNumericToken();
	void ParseEmbeddedBlockL ();
	void ParseCommentL();
	void ExpectToken(int aToken);
	void ParseHeaderL();
	void ParseLanguagesL();
	void ParseFileL();
	void ParsePackageL();
	void ParseIfBlockL();
	void ParseLogicalOp(String& aExpression);
	void ParseRelation(String& aExpression);
	void ParseUnary(String& aExpression);
	void ParseFactor(String& aExpression);
	void ParseOptionsBlockL();
	void ParsePropertyL();
	void ParseVendorNameL();
	void ParseLogoL();
	void ParseDependencyL();
	void ParseVersion();
	void ParseVendorUniqueNameL();
	void ParseTargetDeviceL();

	//Parser Attributes
	wchar_t m_pkgChar;
	int m_token;
	VARIANTVAL m_tokenValue;
	int m_nLineNo;

	void ParserError(char const* msg);

	friend String wstring2string (const std::wstring& aWide);
	friend std::wstring string2wstring (const String& aNarrow);
    friend int CompareTwoString(wchar_t const * string ,wchar_t const * option);
    friend int CompareNString(wchar_t const * string ,wchar_t const * option, int len);
};

#endif //__PKGFILEPARSER_H__
