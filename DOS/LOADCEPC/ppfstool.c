//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

Abstract:  

Functions:


Notes: 

--*/
#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <sys\utime.h>
#include <ctype.h>
#include <direct.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <time.h>
#include "ppfs.h"

#if DEBUG
VOID    DumpCounters(VOID);
#endif

DWORD   CurMSec = 0;
DWORD   *PtrCurMSec = &CurMSec;

#define COPY_FLAG_UPDATE_EXISTING   0x0001
#define COPY_FLAG_UPDATE_OR_ADD     0x0002

typedef unsigned long _fsize_t; /* Could be 64 bits for Win32 */

struct _finddata_t {
    unsigned long   attrib;
    time_t  time_create;    /* -1 for FAT file systems */
    time_t  time_access;    /* -1 for FAT file systems */
    time_t  time_write;
    _fsize_t    size;
    char    name[260];
};

void
usage2(char *pszProgramPath)
{
    char    *pszProgramName;

    if ((pszProgramName = strrchr(pszProgramPath, '\\')) == NULL)
    {
        pszProgramName = pszProgramPath;
    }
    else
    {
        pszProgramName++;
    }

    printf(
        "%s: Parallel Port FileSystem Tool\n"
        "usage: %s -d[:]<pattern> -(g|r|u)[[:]<pattern> [<destination>]\n"
        "-d    Display a directory of files matching <pattern>.\n\n"
        "-g    Get files matching <pattern> and copy them to the\n"
        "      optionally specified <destination>.  Any existing file with\n"
        "      the same name will be overwritten.\n\n"
        "-r    Refresh files matching <pattern> which already exist in <destination>\n"
        "      and have a timestamp newer than the one in <destination>.\n\n"
        "-u    Update files matching <pattern> which don't already exist in\n"
        "      <destination> or those that have a timestamp newer than those\n"
        "      in <destination>.\n\n"
        "<pattern> Windows filename path with optional wildcard characters.\n\n"
        "<destination> If not specified then the current directory is the\n"
        "      default.\n"
        "      If specified and it doesn't exist and the last character is \\,\n"
        "      then the directory is created.  Otherwise <destination> is\n"
        "      treated as the name of the file.\n"
        "      It is an error for <pattern> to match multiple files\n"
        "      when <destination> specifies a file.\n",
        pszProgramName, pszProgramName);
}

VOID
DisplayDirectory(char *pszPath)
{
    DWORD                   hFind;
    struct  _finddata_t     fd;
    struct  tm             *ptmWrite;

    hFind = rfindfirst(0, pszPath, &fd);

    if (hFind == 0)
    {
        printf("%s: No files found\n", pszPath);
    }
    else if (hFind != -1)
    {
        do
        {
            ptmWrite = gmtime(&fd.time_write);

            printf(
                "%c%c%c%c%c %2.2d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d %-8ld %s\n",
                fd.attrib & _A_RDONLY ? 'R' : ' ',
                fd.attrib & _A_HIDDEN ? 'H' : ' ',
                fd.attrib & _A_SYSTEM ? 'S' : ' ',
                fd.attrib & _A_SUBDIR ? 'D' : ' ',
                fd.attrib & _A_ARCH   ? 'A' : ' ',
                ptmWrite->tm_mon + 1, ptmWrite->tm_mday, ptmWrite->tm_year,
                ptmWrite->tm_hour, ptmWrite->tm_min, ptmWrite->tm_sec,
                fd.size, fd.name);
            
            hFind = rfindnext(hFind, &fd);
        }
        while (hFind != 0 && hFind != -1);
    }

    if (hFind == -1)
    {
        printf("%s: Error processing directory\n", pszPath);
    }
}

VOID
DrawPercent (DWORD CurValue, DWORD MaxValue)
{
    DWORD   dwPercent;
    static DWORD dwOldPercent = 10000;

    if (CurValue == (DWORD)-1) {
        dwOldPercent = 10000;
        fprintf(stderr, "%s                         ", MaxValue);
        return;
    }

    dwPercent = (CurValue*100)/MaxValue;
    if ((dwPercent / 2) != (dwOldPercent / 2))
    {
        fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");

        for (dwOldPercent=0; dwOldPercent < (dwPercent/2); dwOldPercent++)
        {
            fprintf(stderr, "%c", 219);
        }

        for (; dwOldPercent < 50; dwOldPercent++)
        {
            fprintf(stderr, "%c", 176);
        }

        dwOldPercent = dwPercent;
        fprintf(stderr, " %2d%%", dwPercent);
    }
    else
    {
        fprintf(stderr, "\b\b\b%2d%%", dwPercent);
    }
}

BOOL
GetOneFile(
    char *pszSourceFile, char *pszDestinationFile,
    DWORD dwFileSize, time_t tmModify)
{
    int                 hDestinationFile;
    DWORD               hSourceFile;

    int                 nReadCount;
    static char         cReadBuffer[16384];

    DWORD               dwBytesWritten;

    printf("%s --> %s\n", pszSourceFile, pszDestinationFile);

    hDestinationFile = _open(
        pszDestinationFile, _O_WRONLY | _O_CREAT | _O_BINARY | _O_TRUNC,
        _S_IREAD | _S_IWRITE);

    if (hDestinationFile == -1)
    {
        printf(
            "%s: Error opening file for write access, error = %d\n",
            pszDestinationFile, errno);
        return FALSE;
    }

    hSourceFile = ropen(pszSourceFile, _O_RDONLY);

    if (hSourceFile == -1)
    {
        _close(hDestinationFile);
        printf("%s: Error opening file for read access\n", pszSourceFile);
        return FALSE;
    }

    // Initialize the percent thingie.
    DrawPercent((DWORD)-1, (DWORD)"");

    dwBytesWritten = 0;

    for ( ; ; )
    {
        nReadCount = (int)rread(hSourceFile, &cReadBuffer[0], sizeof(cReadBuffer));

        if (nReadCount <= 0)
        {
            break;
        }

        _write(hDestinationFile, cReadBuffer, nReadCount);

        dwBytesWritten += nReadCount;

        DrawPercent(dwBytesWritten, dwFileSize);
    }

    fprintf(stderr, "\r                                                                         \r");

    rclose(hSourceFile);

    _close(hDestinationFile);

    if (nReadCount == 0)
    {
        struct  _utimbuf    utDestination;
        time_t              tmNow;

        time(&tmNow);

        utDestination.actime = tmNow;
        utDestination.modtime = tmModify != 0 ? tmModify : tmNow;
        _utime(pszDestinationFile, &utDestination);
    }
    else
    {
        _unlink(pszDestinationFile);
    }

    return nReadCount == 0;
}

BOOL
CheckDestination(char *pszDestination, char **ppszDirEnd)
{
    struct _stat    stDestination;
    BOOL            bIsDir;

    *ppszDirEnd = strrchr(pszDestination, '\\');

    if (*ppszDirEnd == NULL && pszDestination[1] == ':')
    {
        *ppszDirEnd = &pszDestination[1];
    }

    if (*ppszDirEnd == NULL)
    {
        *ppszDirEnd = pszDestination;
    }
    else
    {
        (*ppszDirEnd)++;
    }

    bIsDir = FALSE;

    if (pszDestination[strlen(pszDestination)-1] == '\\')
    {
        pszDestination[strlen(pszDestination)-1] = '\0';
        bIsDir = TRUE;
    }

    if (_stat(pszDestination, &stDestination) == 0)
    {
        if (stDestination.st_mode & _S_IFDIR)
        {
            bIsDir = TRUE;
        }
    }
    else if (bIsDir)
    {
        if (_mkdir(pszDestination) == -1)
        {
            printf("%s: Error creating directory, error = %d\n", pszDestination, errno);
        }
    }

    if (bIsDir)
    {
        *ppszDirEnd = &pszDestination[strlen(pszDestination)];

        if (*ppszDirEnd[-1] != '\\')
        {
            *(*ppszDirEnd)++ = '\\';
        }
    }

    return bIsDir;
}

VOID
GetFile(char *pszSource, char *pszDestination, USHORT usCopyFlags)
{
    DWORD               hFind;

    struct _stat        stDestination;
    char                szDestinationFile[_MAX_PATH];
    char               *pDestinationDirEnd;

    struct _finddata_t  fdSource;
    char                szSourceFile[_MAX_PATH];
    char               *pSourceDirEnd;

    BOOL                bDestinationIsDir;

    int                 nFilesCopied = 0;

    BOOL                bDestinationExists;
    BOOL                bDoCopy;

    pSourceDirEnd = strrchr(pszSource, '\\');

    if (pSourceDirEnd == NULL && pszSource[1] == ':')
    {
        pSourceDirEnd = &pszSource[1];
    }

    if (pSourceDirEnd != NULL)
    {
        int     nDirLength = pSourceDirEnd - pszSource + 1;

        memcpy(szSourceFile, pszSource, nDirLength);
        pSourceDirEnd = &szSourceFile[nDirLength];
    }
    else
    {
        pSourceDirEnd = szSourceFile;
    }

    hFind = rfindfirst(0, pszSource, &fdSource);

    if (hFind == 0 || hFind == -1)
    {
        printf("%s: File(s) not found\n", pszSource);
        return;
    }

    if (pszDestination != NULL)
    {
        strcpy(szDestinationFile, pszDestination);

        bDestinationIsDir = CheckDestination(szDestinationFile, &pDestinationDirEnd);
    }
    else
    {
        pDestinationDirEnd = szDestinationFile;
        bDestinationIsDir = TRUE;
    }

    do
    {
        strcpy(pSourceDirEnd, fdSource.name);

        if (bDestinationIsDir)
        {
            strcpy(pDestinationDirEnd, fdSource.name);
        }

        bDoCopy = TRUE;

        if (usCopyFlags & (COPY_FLAG_UPDATE_EXISTING | COPY_FLAG_UPDATE_OR_ADD))
        {
            bDestinationExists = _stat(szDestinationFile, &stDestination) == 0;

            if (bDestinationExists)
            {
                //
                // Clear lsb of both times so inaccurate FATFS doesn't cause
                // problems
                //
                stDestination.st_mtime &= ~1;
                fdSource.time_write &= ~1;

                if (stDestination.st_mtime >= fdSource.time_write)
                {
                    printf("%s: Up to date\n", szDestinationFile);
                    bDoCopy = FALSE;
                }
            }
            else if (!(usCopyFlags & COPY_FLAG_UPDATE_OR_ADD))
            {
                printf("%s: Skipping\n", szSourceFile);
                bDoCopy = FALSE;
            }
        }

        if (bDoCopy)        
        {
            if (GetOneFile(szSourceFile, szDestinationFile, fdSource.size, fdSource.time_write))
            {
                nFilesCopied++;
            }
            else
            {
                break;
            }
        }

        hFind = rfindnext(hFind, &fdSource);

        if (!bDestinationIsDir && hFind != 0 && hFind != -1)
        {
            printf(
                "%s: Destination isn't a directory and multiple files selected\n",
                szDestinationFile);

            break;
        }
    }
    while (hFind != 0 && hFind != -1);

    if (hFind != 0 && hFind != -1)
    {
        rfindfirst(hFind, NULL, NULL);
    }

    printf("%d files copied\n", nFilesCopied);
}

int
main2(int argc, char **argv)
{
    char    cOption;
    int     i;
    int     OEMParallelPortInit(void);

    //
    // Keep timezone conversions from getting in our way.  All of the responses
    // from the host are in local time anyways.
    //
    _daylight = 0;
    _timezone = 0;

    OEMParallelPortInit();

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' || argv[i][0] == '/')
        {
            cOption = tolower(argv[i][1]);

            switch (cOption)
            {
            case '?':
            case 'h':
                usage2(argv[0]);
                break;
            case 'd':
                {
                    char   *pszPath;

                    if (argv[i][2] != '\0')
                    {
                        pszPath = &argv[i][2];
                        if (*pszPath == ':')
                        {
                            pszPath++;
                        }
                    }
                    else if (argc > (i+1) && argv[i+1][0] != '-' && argv[i+1][0] != '/')
                    {
                        pszPath = argv[i+1];
                        i++;
                    }
                    else
                    {
                        pszPath = "*.*"; 
                    }
                    DisplayDirectory(pszPath);
                }
                break;

            case 'g':
            case 'r':
            case 'u':
                {
                    char    *pszSource;
                    char    *pszDestination;

                    if (argv[i][2] != '\0')
                    {
                        pszSource = &argv[i][2];
                        if (*pszSource == ':')
                        {
                            pszSource++;
                        }
                    }
                    else if (argc > (i+1) && argv[i+1][0] != '-' && argv[i+1][0] != '/')
                    {
                        pszSource = argv[i+1];
                        i++;
                    }
                    else
                    {
                        printf("Error source path missing\n");
                        break;
                    }

                    if (argc > (i+1) && argv[i+1][0] != '-' && argv[i+1][0] != '/')
                    {
                        pszDestination = argv[i+1];
                        i++;
                    }
                    else
                    {
                        pszDestination = NULL;
                    }

                    GetFile(
                        pszSource, pszDestination,
                        cOption == 'r' ? COPY_FLAG_UPDATE_EXISTING :
                            cOption == 'u' ? COPY_FLAG_UPDATE_OR_ADD :
                                0);
                }
                break;

            default:
                printf("%s: Unsupported option\n\n", argv[i]);
                usage2(argv[0]);
            }
        }
    }

#if DEBUG
    DumpCounters();
#endif

    return 0;
}
