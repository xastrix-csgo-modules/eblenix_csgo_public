#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <time.h>
#include <psapi.h>
#include <shlwapi.h>

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS (NTSTATUS)0L
#endif

using _NtOpenProcess = NTSTATUS(NTAPI*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, CLIENT_ID*);
using _NtAllocateVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID*, ULONG, PSIZE_T, ULONG, ULONG);
using _NtWriteVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
using _NtCreateThreadEx = NTSTATUS(NTAPI*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, HANDLE,
	PVOID, PVOID, ULONG, SIZE_T, SIZE_T, SIZE_T, PVOID);

using _LoadLibraryA = FARPROC(WINAPI*)(LPCSTR);

namespace util
{
	inline auto get_proc_id(const std::string& process_name) -> DWORD
	{
		DWORD ret{};

		{
			PROCESSENTRY32 proc_info{};
			proc_info.dwSize = sizeof(proc_info);

			const auto h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
			if (h == INVALID_HANDLE_VALUE)
				return ret;

			Process32First(h, &proc_info);

			while (Process32Next(h, &proc_info))
			{
				if (proc_info.szExeFile == process_name) {
					ret = proc_info.th32ProcessID;
					break;
				}
			}

			CloseHandle(h);
		}

		return ret;
	}

	inline auto file_exists(const std::string& path) -> bool
	{
		struct stat s{};
		stat(path.c_str(), &s);

		return (s.st_mode & S_IFREG);
	}

	inline auto get_current_path() -> std::string
	{
		char path[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, path);

		return std::string{ path } + "\\";
	}

	template <typename T>
	inline auto get_export(const char* mod_name, const char* function_name) -> T
	{
		return reinterpret_cast<T>(GetProcAddress(GetModuleHandleA(mod_name), function_name));
	}

	inline auto inject(const DWORD pid, const std::string& dll_name) -> int
	{
		CLIENT_ID         cid{};
		NTSTATUS          status{};
		HANDLE            proc{},
			              thread{};
		OBJECT_ATTRIBUTES oa{};
		PVOID             base_addr{};
		SIZE_T            bytes_written{};

		std::string dll_path = get_current_path() + dll_name;
		SIZE_T dll_size      = dll_path.size() + 1;

		if (!file_exists(dll_path)) {
			printf("[-] dll not found: %s\n", dll_path.c_str());
			return 1;
		}

		InitializeObjectAttributes(&oa, NULL, 0, NULL, NULL);

		cid.UniqueProcess              = (HANDLE)pid;
		cid.UniqueThread               = NULL;
		status                         = get_export<_NtOpenProcess>("ntdll.dll", "NtOpenProcess")(
			&proc,
			PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
			PROCESS_VM_OPERATION  | PROCESS_VM_WRITE | PROCESS_VM_READ,
			&oa, &cid);

		if (status != STATUS_SUCCESS) {
			printf("[-] NtOpenProcess failed with status: 0x%X\n", status);
			return 1;
		}

		status                         = get_export<_NtAllocateVirtualMemory>("ntdll.dll", "NtAllocateVirtualMemory")(
			proc, &base_addr, 0,
			&dll_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE
		);

		if (status != STATUS_SUCCESS) {
			printf("[-] NtAllocateVirtualMemory failed with status: 0x%X\n", status);
			return 1;
		}

		status                         = get_export<_NtWriteVirtualMemory>("ntdll.dll", "NtWriteVirtualMemory")(
			proc, base_addr,
			(PVOID)dll_path.c_str(),
			(ULONG)dll_size, &bytes_written
		);

		if (status != STATUS_SUCCESS) {
			printf("[-] NtWriteVirtualMemory failed with status: 0x%X\n", status);
			return 1;
		}

		const auto ll                  = get_export<_LoadLibraryA>("kernel32.dll", "LoadLibraryA");

		if (!ll) {
			printf("[-] failed to get LoadLibraryA export from kernel32.dll\n");
			return 1;
		}

		status                         = get_export<_NtCreateThreadEx>("ntdll.dll", "NtCreateThreadEx")(
			&thread, 0x1fffff, 0, proc,
			ll, base_addr, FALSE,
			0, 0, 0, 0
		);

		if (status != STATUS_SUCCESS) {
			printf("[-] NtCreateThreadEx failed with status: 0x%X\n", status);
			return 1;
		}

		CloseHandle(thread);
		CloseHandle(proc);

		return 0;
	}
}