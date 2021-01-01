// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#include <Windows.h>

#include "DebugMacro.h"

#include "../../lsMisc/IsFileOpen.h"
#include "../../lsMisc/getStdString.net.h"
#include "../../lsMisc/OpenCommon.h"

enum ERROR_RETURN
{
	RETURN_NOERROR = 0,
	RETURN_NOINPUTFILE,
	RETURN_TOOMANYINPUTFILES,
	RETURN_FILENOTFOUND,
	RETURN_STREAMNOTFOUND,
	RETURN_DURATIONNOTFOUND,
	RETURN_STREAMISH265,
	RETURN_STREAMISVP9,
	RETURN_SHUTDOWN_MUST_NOT_WITH_ARGS,
};



#define I18N(s) Ambiesoft::toH265::Program::getString(s)

