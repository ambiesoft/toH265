// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

// TODO: reference additional headers your program requires here

enum ERROR_RETURN
{
	RETURN_NOERROR = 0,
	RETURN_NOINPUTFILE,
	RETURN_TOOMANYINPUTFILES,
	RETURN_FILENOTFOUND,
	RETURN_STREAMNOTFOUND,
	RETURN_STREAMISH265
};

#define I18N(s) s