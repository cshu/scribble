

#include <stdio.h>
#include <windows.h>

int main(int argc, char *argv[])
{
    HANDLE hFile;
    if( argc != 2 )
    {
        printf("0");
        return 1;
    }
    hFile = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        printf("0");
        return 1;
    }
    FILETIME ftCreate, ftAccess, ftWrite;
    if (GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)){
        printf("%lld\n",*(long long *)(&ftWrite));
        CloseHandle(hFile);
        return 0;
    } else {
        printf("0");
        CloseHandle(hFile);
        return 1;
    }
}