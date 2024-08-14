// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <metahost.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <comdef.h>

#pragma comment(lib, "mscoree.lib")

void LogToFile(const std::wstring& message) {
    std::wofstream logFile;
    logFile.open(L"C:\\Users\\Public\\log.txt", std::ios_base::app);
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.close();
    }
}

std::wstring ToWString(DWORD value) {
    std::wstringstream wss;
    wss << value;
    return wss.str();
}

std::wstring GetErrorMessage(HRESULT hr) {
    _com_error err(hr);
    return std::wstring(err.ErrorMessage());
}

int main()
{
    ICLRMetaHost* metaHost = NULL; // Declare our CLR Meta Host value as a NULL
    ICLRRuntimeInfo* runtimeInfo = NULL; // Declare our CLR Runtime Info as a Null
    ICLRRuntimeHost* runtimeHost = NULL; // Declare our CLR HOST as a NULL

    if (CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&metaHost) == S_OK) { // If Creating CLR Instance with follow parameters is successful
        LogToFile(L"CLR instance created successfully.");

        if (metaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (LPVOID*)&runtimeInfo) == S_OK) { // If getting Runtime version is successful
            LogToFile(L"Got CLR runtime version 4.0.30319 successfully.");

            BOOL isStarted = FALSE;
            if (runtimeInfo->IsStarted(&isStarted, NULL) == S_OK && isStarted) {
                LogToFile(L"CLR runtime host is already started.");

                if (runtimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&runtimeHost) == S_OK) {
                    LogToFile(L"Got CLR runtime host interface successfully.");
                }
                else {
                    LogToFile(L"Failed to get CLR runtime host interface.");
                }

            }
            else {
                if (runtimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&runtimeHost) == S_OK) { // If getting the interface with the follow parameters is successful
                    LogToFile(L"Got CLR runtime host interface successfully.");

                    if (runtimeHost->Start() == S_OK) { // Start the CLR (If it is successful)
                        LogToFile(L"CLR runtime host started successfully.");
                    }
                    else {
                        LogToFile(L"Failed to start CLR runtime host.");
                    }
                }
                else {
                    LogToFile(L"Failed to get CLR runtime host interface.");
                }
            }

            DWORD pReturnValue; // Declare our return value as a DWORD

            // Invoke our method through CLR host using following parameters
            HRESULT hr = runtimeHost->ExecuteInDefaultAppDomain(L"C:\\Users\\Public\\captainhook.dll", L"Captain_HooK.HooK", L"InstallHook", nullptr, &pReturnValue);
            if (hr == S_OK) {
                LogToFile(L"Method executed successfully with return value: " + ToWString(pReturnValue));
            }
            else {
                LogToFile(L"Failed to execute method in default app domain. Error: " + GetErrorMessage(hr) + L" (HRESULT: " + ToWString(hr) + L")");
            }

            // OPTIONAL: You can keep the CLR Opened depending on your needs
            runtimeInfo->Release();
            metaHost->Release();
            runtimeHost->Release();
        }
        else {
            LogToFile(L"Failed to get CLR runtime version.");
        }
    }
    else {
        LogToFile(L"Failed to create CLR instance.");
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        auto Thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
        if (Thread)
            return TRUE;
        else
            return FALSE;
    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
