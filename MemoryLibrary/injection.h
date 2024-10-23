#pragma once
// injection.h
#ifndef INJECTION_H
#define INJECTION_H
#include "pch.h"
#include <windows.h>
#include <libloaderapi.h>
#include <iostream>

BOOL InjectDLL(DWORD processId, const char* dllPath);

#endif
