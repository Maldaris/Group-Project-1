// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <queue>
#include <climits>
#include <set>
#include <string>
#define MSVC_13
#ifdef MSVC_13 
#define JSON_DLL
#include "json\json.h"
#define ARMADILLO_DLL
#include <armadillo.h>
#endif
// TODO: reference additional headers your program requires here
