#include <cstdio>
#include <iostream>
#include <string>
#include <stdexcept>
#include "C:\Users\aradh\OneDrive\Desktop\os\backupandrestore.cpp"



using namespace std;

// Abstract class for ACL operations
class AclOperations
{
public:
    // Data members
    string filename;   // File or folder path
    string username;   // Username for permission
    string permission; // Permission type
    char buffer[128];  // Buffer for command output

    // Virtual functions for ACL operations
    virtual string ReadACL() = 0;
    virtual void Grant_P() = 0;
    virtual void Remove_P() = 0;
    virtual void Deny_P() = 0;
    virtual void Reset_P() = 0;
    virtual void DisableInheritance() = 0;
};

// Implementation class for ACL operations
class AclOperationsImpl : public AclOperations
{
public:
    string ReadACL() override;
    void Grant_P() override;
    void Remove_P() override;
    void Deny_P() override;
    void Reset_P() override;
    void DisableInheritance() override;
};

// Function to read ACL
string AclOperationsImpl::ReadACL()
{
    // Get file path from user
    cout << "Enter the file path: ";
    cin.ignore();
    getline(cin, filename);

    // Construct command for icacls and execute it using popen
    string command = "icacls \"" + filename + "\"";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        // Throw an exception if command execution fails
        throw runtime_error("Error executing icacls command");
    }

    // Read command output into result string
    string result = "";
    while (fgets(buffer, 128, pipe))
    {
        result += buffer;
    }

    // Close the pipe and check the status
    int status = pclose(pipe);
    if (status != 0)
    {
        // Throw an exception if command execution fails
        throw runtime_error("icacls command failed with error code " + to_string(status));
    }

    // Print result and return command
    cout << result << endl;
    return command;
}

// Function to grant permission
void AclOperationsImpl::Grant_P()
{
    // Get file/folder path, username, and permission type from user
    cout << "Enter the file or folder path: ";
    cin.ignore();
    getline(cin, filename);

    cout << "Enter the username to grant permission: ";
    getline(cin, username);

    cout << "Enter the permission to grant (F, M, RX): ";
    getline(cin, permission);

    // Construct command for icacls and execute it using popen
    string command = "icacls \"" + filename + "\" /grant \"" + username + "\":" + permission;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        // Throw an exception if command execution fails
        throw runtime_error("Error executing icacls command");
    }

    // Read command output and print it to console
    while (fgets(buffer, 128, pipe))
    {
        cout << buffer;
    }

    // Close the pipe and check the status
    int result = pclose(pipe);
    if (result != 0)
    {
        // Throw an exception if command execution fails
        throw runtime_error("icacls command failed with error code " + to_string(result));
    }

    // Print success message
    cout << "Permission granted successfully!" << endl;
}

// Function to remove permission
void AclOperationsImpl::Remove_P()
{
// Get file/folder path, username, and permission type from user
cout << "Enter the file or folder path: ";
cin.ignore();
getline(cin, filename);
cout << "Enter the username to remove permission: ";
getline(cin, username);

cout << "Enter the permission to remove (F, M, RX): ";
getline(cin, permission);

// Construct command for icacls and execute it using popen
string command = "icacls \"" + filename + "\" /remove \"" + username + "\":" + permission;
FILE* pipe = popen(command.c_str(), "r");
if (!pipe)
{
    // Throw an exception if command execution fails
    throw runtime_error("Error executing icacls command");
}

// Read command output and print it to console
while (fgets(buffer, 128, pipe))
{
    cout << buffer;
}

// Close the pipe and check the status
int result = pclose(pipe);
if (result != 0)
{
    // Throw an exception if command execution fails
    throw runtime_error("icacls command failed with error code " + to_string(result));
}

// Print success message
cout << "Permission removed successfully!" << endl;
}

// Function to deny permission
void AclOperationsImpl::Deny_P()
{
// Get file/folder path, username, and permission type from user
cout << "Enter the file or folder path: ";
cin.ignore();
getline(cin, filename);
cout << "Enter the username to deny permission: ";
getline(cin, username);

cout << "Enter the permission to deny (F, M, RX): ";
getline(cin, permission);

// Construct command for icacls and execute it using popen
string command = "icacls \"" + filename + "\" /deny \"" + username + "\":" + permission;
FILE* pipe = popen(command.c_str(), "r");
if (!pipe)
{
    // Throw an exception if command execution fails
    throw runtime_error("Error executing icacls command");
}

// Read command output and print it to console
while (fgets(buffer, 128, pipe))
{
    cout << buffer;
}

// Close the pipe and check the status
int result = pclose(pipe);
if (result != 0)
{
    // Throw an exception if command execution fails
    throw runtime_error("icacls command failed with error code " + to_string(result));
}

// Print success message
cout << "Permission denied successfully!" << endl;
}

// Function to reset permissions to default
void AclOperationsImpl::Reset_P()
{
// Get file/folder path from user
cout << "Enter the file or folder path: ";
cin.ignore();
getline(cin, filename);
// Construct command for icacls and execute it using popen
string command = "icacls \"" + filename + "\" /reset";
FILE* pipe = popen(command.c_str(), "r");
if (!pipe)
{
    // Throw an exception if command execution fails
    throw runtime_error("Error executing icacls command");
}

// Read command output and print it to console
while (fgets(buffer, 128, pipe))
{
    cout << buffer;
}

// Close the pipe and check the status
int result = pclose(pipe);
if (result != 0)
{
    // Throw an exception if command execution fails
    throw runtime_error("icacls command failed with error code " + to_string(result));
}

// Print success message
cout << "Permissions reset successfully!" << endl;
}

void AclOperationsImpl::DisableInheritance()
{
    // Get file/folder path from user
    std::cout << "Enter the file or folder path: ";
    std::string path;
    std::getline(std::cin >> std::ws, path);

    // Construct command to check inheritance status and execute it using popen
    std::string checkCommand = "icacls \"" + path + "\"";
    FILE* checkPipe = popen(checkCommand.c_str(), "r");
    if (!checkPipe)
    {
        throw std::runtime_error("Error executing icacls command");
    }

    // Read command output and check if inheritance is enabled or disabled
    std::string checkOutput;
    char checkBuffer[128];
    while (fgets(checkBuffer, 128, checkPipe))
    {
        checkOutput += checkBuffer;
    }
    bool inheritanceEnabled = checkOutput.find("(OI)(CI)(IO)") != std::string::npos;

    // Close the check pipe and check the status
    int checkResult = pclose(checkPipe);
    if (checkResult != 0)
    {
        throw std::runtime_error("icacls command failed with error code " + std::to_string(checkResult));
    }

    // Display current inheritance status and prompt user to enable or disable it
    std::cout << "Current inheritance status: " << (inheritanceEnabled ? "Enabled" : "Disabled") << std::endl;
    std::cout << "Do you want to enable or disable inheritance? (e/d): ";
    char choice;
    std::cin >> choice;

    // Construct command to enable/disable inheritance and execute it using popen
    std::string command;
    switch (choice)
    {
        case 'e':
        case 'E':
            command = "icacls \"" + path + "\" /inheritance:e";
            break;
        case 'd':
        case 'D':
            command = "icacls \"" + path + "\" /inheritance:d";
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            return;
    }
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        throw std::runtime_error("Error executing icacls command");
    }

    // Read command output and print it to console
    char buffer[128];
    while (fgets(buffer, 128, pipe))
    {
        std::cout << buffer;
    }

    // Close the pipe and check the status
    int result = pclose(pipe);
    if (result != 0)
    {
        throw std::runtime_error("icacls command failed with error code " + std::to_string(result));
    }

    // Print success message
    std::cout << "Permissions inheritance " << (choice == 'd' || choice == 'D' ? "disabled" : "enabled") << " successfully!" << std::endl;
}

// Main function
int main()
{
AclOperationsImpl acl;
wstring path;
wstring backupPath;
// Run ACL operations until the user chooses to exit
while (true)
{
    try
    {
        // Display menu to user
        cout << "Enter your choice:\n";
        cout << "1. Read ACL\n";
        cout << "2. Grant Permission\n";
        cout << "3. Remove Permission\n";
        cout << "4. Deny Permission\n";
        cout << "5. Reset Permissions\n";
        cout << "6. DisableInheritance\n";
        cout << "7. Create File Permission Backup \n";
        cout << "8. Exit\n";

        // Get user input
        int choice;
        cin >> choice;

        switch (choice)
        {
            case 1:
                acl.ReadACL();
                break;
            case 2:
                acl.Grant_P();
                break;
            case 3:
                acl.Remove_P();
                break;
            case 4:
                acl.Deny_P();
                break;
            case 5:
                acl.Reset_P();
                break;
            case 6:
                acl.DisableInheritance();
                break;
            case 7:
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
                            } else
                            {
                                wcout << L"Error creating permissions backup.\n";
                            }break;
                            }
                            case 2:
                            {
                                // Restore file permissions from backup
                                success = RestoreFilePermissions(backupPath.c_str(), path.c_str());
                                if (success) {
                                    wcout << L"File permissions restored from backup successfully.\n";
                                } else
                                {
                                    wcout << L"Error restoring file permissions from backup.\n";
                                }
                                break;
                                }
                                default:
                                    cout << "Invalid option selected.\n";
                                    break;
                    }
                }
            case 8:
                // Exit the program
                return 0;
                break;
            default:
                // Throw an exception if the user enters an invalid choice
                throw invalid_argument("Invalid choice entered");
                }
                }
                catch (const exception& e)
                {
                    // Print error message to console
                    cerr << e.what() << endl;
                }
                }
                return 0;
}

