#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

bool BackupFilePermissions(const wchar_t* filepath, const wchar_t* backuppath)
{
    SECURITY_INFORMATION secInfo = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;

    HANDLE hFile = CreateFileW(filepath, READ_CONTROL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        wcout << L"Failed to open file: " << filepath << L"\n";
        return false;
    }

    PSECURITY_DESCRIPTOR pSD = NULL;
    DWORD dwLength = 0;
    BOOL bResult = GetFileSecurityW(filepath, secInfo, NULL, 0, &dwLength);
    if (!bResult && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, dwLength);
        if (!pSD)
        {
            wcout << L"Failed to allocate memory for security descriptor.\n";
            CloseHandle(hFile);
            return false;
        }
        bResult = GetFileSecurityW(filepath, secInfo, pSD, dwLength, &dwLength);
        if (!bResult)
        {
            wcout << L"Failed to retrieve file security information.\n";
            LocalFree(pSD);
            CloseHandle(hFile);
            return false;
        }
    }
    else
    {
        wcout << L"Failed to retrieve required buffer size for security descriptor.\n";
        CloseHandle(hFile);
        return false;
    }

    HANDLE hBackup = CreateFileW(backuppath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hBackup == INVALID_HANDLE_VALUE)
    {
        wcout << L"Failed to create backup file: " << backuppath << L"\n";
        LocalFree(pSD);
        CloseHandle(hFile);
        return false;
    }

    DWORD bytesWritten = 0;
    bResult = WriteFile(hBackup, pSD, dwLength, &bytesWritten, NULL);
    if (!bResult || bytesWritten != dwLength)
    {
        wcout << L"Failed to write security descriptor to backup file.\n";
        LocalFree(pSD);
        CloseHandle(hBackup);
        CloseHandle(hFile);
        return false;
    }

    LocalFree(pSD);
    CloseHandle(hBackup);
    CloseHandle(hFile);

    return true;
}

bool RestoreFilePermissions(const wchar_t* backuppath, const wchar_t* filepath)
{
    HANDLE hFile = CreateFileW(filepath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        wcout << L"Failed to open file: " << filepath << L"\n";
        return false;
    }

    HANDLE hBackup = CreateFileW(backuppath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hBackup == INVALID_HANDLE_VALUE)
    {
        wcout << L"Failed to open backup file: " << backuppath << L"\n";
        CloseHandle(hFile);
        return false;
    }

    DWORD dwLength = GetFileSize(hBackup, NULL);
    if (dwLength == INVALID_FILE_SIZE)
    {
        wcout << L"Failed to retrieve backup file size.\n";
        CloseHandle(hBackup);
        CloseHandle(hFile);
        return false;
    }

    PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, dwLength);
    if (!pSD)
    {
        wcout << L"Failed to allocate memory for security descriptor.\n";
        CloseHandle(hBackup);
        CloseHandle(hFile);
        return false;
    }

    DWORD bytesRead = 0;
    BOOL bResult = ReadFile(hBackup, pSD, dwLength, &bytesRead, NULL);
    if (!bResult || bytesRead != dwLength)
    {
        wcout << L"Failed to read security descriptor from backup file.\n";
        LocalFree(pSD);
        CloseHandle(hBackup);
        CloseHandle(hFile);
        return false;
    }

    bResult = SetFileSecurityW(filepath, DACL_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION, pSD);
    if (!bResult)
    {
        wcout << L"Failed to restore file security.\n";
        LocalFree(pSD);
        CloseHandle(hBackup);
        CloseHandle(hFile);
        return false;
    }

    LocalFree(pSD);
    CloseHandle(hBackup);
    CloseHandle(hFile);
    return true;
}

int main()
{
    wstring path;
    wstring backupPath;

    // Prompt the user to enter the path and backup path
    cout << "Enter the file path: ";
    getline(wcin, path);
    cout << "Enter the backup file path: ";
    getline(wcin, backupPath);

    // Prompt the user to select an option
    cout << "Select an option:\n";
    cout << "1. Backup file permissions\n";
    cout << "2. Restore file permissions from backup\n";
    int option;
    cin >> option;

    bool success = false;

    switch (option)
    {
    case 1:
    {
        // Backup file permissions
        success = BackupFilePermissions(path.c_str(), backupPath.c_str());
        if (success) {
            wcout << L"Permissions backup created successfully.\n";
        } else {
            wcout << L"Error creating permissions backup.\n";
        }
        break;
    }
    case 2:
    {
        // Restore file permissions from backup
        success = RestoreFilePermissions(backupPath.c_str(), path.c_str());
        if (success) {
            wcout << L"File permissions restored from backup successfully.\n";
        } else {
            wcout << L"Error restoring file permissions from backup.\n";
        }
        break;
    }
    default:
        cout << "Invalid option selected.\n";
        break;
    }

    return 0;
}
