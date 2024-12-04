#include <windows.h>
#include <shlwapi.h>
#include <wchar.h>
#include <stdio.h>
#include <string.h>

#define DLL_NAME    L"VSAPI32.DLL"
#define ROOT        L"C:\\Program Files (x86)\\AsiaInfo\\OfficeScan Client\\SUSPECT\\Backup\\"
#define TMP_FILE    L"tmp.bin"


HMODULE DllHandle = NULL;

typedef int (*VSGetBackupFileInfoW_T)(const wchar_t *in, wchar_t *out);
typedef int (*VSRestoreFileW_T)(const wchar_t *in, const wchar_t *out);
VSGetBackupFileInfoW_T VSGetBackupFileInfoW = NULL;
VSRestoreFileW_T       VSRestoreFileW       = NULL;

wchar_t src_buf[0x1000];
wchar_t dst_buf[0x1000];

bool Restore(const wchar_t *name)
{
    VSGetBackupFileInfoW(name, dst_buf);
    wprintf(L"%ls\n", dst_buf);

    if (PathFileExistsW(dst_buf))
    {
        wprintf(L"  File exists. Skip.\n");
        return false;
    }
    int result = VSRestoreFileW(name, TMP_FILE);
    wprintf(L"VSRestoreFileW return: 0x%08x\n", result);
    if (result != 0)
    {
        wprintf(L"VSRestoreFileW failed\n");
        return false;
    }

    wprintf(L"copy result: %d\n", CopyFileW(TMP_FILE, dst_buf, TRUE));
    DeleteFile(TMP_FILE);
    return true;
}

void RestoreAll()
{
    WIN32_FIND_DATA ffd;
    HANDLE          hFind = FindFirstFileW(ROOT L"*.qtn", &ffd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            lstrcpyW(src_buf, ROOT);
            lstrcatW(src_buf, ffd.cFileName);
            Restore(src_buf);
        } while (FindNextFileW(hFind, &ffd) != 0);
    }
}

bool LoadFunctions()
{
    HMODULE DllHandle = LoadLibraryW(DLL_NAME);
    if (DllHandle == NULL)
    {
        return false;
    }

    VSGetBackupFileInfoW = (VSGetBackupFileInfoW_T)GetProcAddress(DllHandle, "VSGetBackupFileInfoW");
    VSRestoreFileW       = (VSRestoreFileW_T)GetProcAddress(DllHandle, "VSRestoreFileW");
    return VSGetBackupFileInfoW && VSRestoreFileW;
}

void PrintHelp()
{
    printf("A local files restore tool for stupid Asia OfficeScan.\n"
           "Usage: asia_restore [qtn file] | --all\n\n"
           "Example:\n"
           "Restore a signle file:\n"
           "    asia_restore \"C:\\Program Files (x86)\\AsiaInfo\\OfficeScan Client\\SUSPECT\\Backup\\somefile.qtn\"\n\n"
           "Restore all:\n"
           "    asia_restore --all\n"
           );
}

int wmain(int argc, wchar_t **argv)
{
    if (argc != 2 || StrCmpW(argv[1], L"--help") == 0)
    {
        PrintHelp();
        return 1;
    }

    if (!LoadFunctions())
    {
        printf("Make sure you put the VSAPI32.DLL in the same directory.\n");
        return -1;
    }

    if (StrCmpW(argv[1], L"--all") == 0)
    {
        RestoreAll();
    }
    else
    {
        Restore(argv[1]);
    }

    FreeLibrary(DllHandle);

    return 0;
}
