/// Program for timing execution of another program.
///
/// Usage:
/// > stopwatch <program> <args>
///
#include <stdio.h>
#include <chrono>
#include <windows.h>

bool HasSpaces(const char* str)
{
    while (*str != 0)
    {
        if (*str == ' ')
            return true;

        str++;
    }
    return false;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Invalid number of parameters\n");
        printf("Usage: stopwatch.exe <program> <args>\n");
        return 0;
    }

    // Build command line string
    char command_line[4096];
    char* p = command_line;
    for (int i = 1; i < argc; i++)
    {
        int n;
        if (HasSpaces(argv[i]))
        {
            n = sprintf(p, "\"%s\" ", argv[i]);
        }
        else
        {
            n = sprintf(p, "%s ", argv[i]);
        }
        p += n;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // TODO: Quote program as necessary
    STARTUPINFO startup = {};
    PROCESS_INFORMATION process_info = {};
    BOOL success = CreateProcessA(argv[1], command_line, NULL, NULL, FALSE, 0, NULL, NULL, &startup, &process_info);
    if (!success)
    {
        // TODO: Print more specific error
        printf("Failed to start program\n");
        return 1;
    }

    WaitForSingleObject(process_info.hProcess, INFINITE);

    auto end = std::chrono::high_resolution_clock::now();

    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    double elapsed = std::chrono::duration<double>(end - start).count();
    printf("Elapsed: %f\n", elapsed);

    return 0;
}
