/*
 * File handling declarations
 *
 * Copyright 1996 Alexandre Julliard
 */

#ifndef __WINE_FILE_H
#define __WINE_FILE_H

#include <time.h> /* time_t */
#include <sys/time.h>
#include "winbase.h"
#include "wine/windef16.h"  /* HFILE16 */

#define MAX_PATHNAME_LEN   1024

/* Definition of a full DOS file name */
typedef struct
{
    char  long_name[MAX_PATHNAME_LEN];  /* Long pathname in Unix format */
    char  short_name[MAX_PATHNAME_LEN]; /* Short pathname in DOS 8.3 format */
    int   drive;
} DOS_FULL_NAME;

#define IS_END_OF_NAME(ch)  (!(ch) || ((ch) == '/') || ((ch) == '\\'))

/* DOS device descriptor */
typedef struct
{
    char *name;
    int flags;
} DOS_DEVICE;

/* overlapped private structure */
struct async_private;
typedef void (*async_handler)(struct async_private *ovp);
typedef struct async_private
{
     LPOVERLAPPED  lpOverlapped;
     HANDLE        handle;
     int           fd;
     char         *buffer;
     async_handler func;
     int           count;
     int           type;
     LPOVERLAPPED_COMPLETION_ROUTINE completion_func;
     struct async_private *next;
     struct async_private *prev;
} async_private;

extern void WINAPI check_async_list(LPOVERLAPPED ov, DWORD status);

/* locale-independent case conversion */
inline static char FILE_tolower( char c )
{
    if (c >= 'A' && c <= 'Z') c += 32;
    return c;
}
inline static char FILE_toupper( char c )
{
    if (c >= 'a' && c <= 'z') c -= 32;
    return c;
}

inline static int FILE_contains_path (LPCSTR name)
{
    return ((*name && (name[1] == ':')) ||
            strchr (name, '/') || strchr (name, '\\'));
}

/* files/file.c */
extern int FILE_strcasecmp( const char *str1, const char *str2 );
extern int FILE_strncasecmp( const char *str1, const char *str2, int len );
extern void FILE_SetDosError(void);
extern HANDLE FILE_DupUnixHandle( int fd, DWORD access, BOOL inherit );
extern int FILE_GetUnixHandle( HANDLE handle, DWORD access );
extern BOOL FILE_Stat( LPCSTR unixName, BY_HANDLE_FILE_INFORMATION *info );
extern HFILE16 FILE_Dup2( HFILE16 hFile1, HFILE16 hFile2 );
extern HANDLE FILE_CreateFile( LPCSTR filename, DWORD access, DWORD sharing,
                               LPSECURITY_ATTRIBUTES sa, DWORD creation,
                               DWORD attributes, HANDLE template, BOOL fail_read_only,
                               UINT drive_type );
extern HANDLE FILE_CreateDevice( int client_id, DWORD access, LPSECURITY_ATTRIBUTES sa );
extern BOOL FILE_StartAsync(HANDLE handle, LPOVERLAPPED lpOverlapped, DWORD type, DWORD count, DWORD status);

extern LONG WINAPI WIN16_hread(HFILE16,SEGPTR,LONG);

/* files/directory.c */
extern int DIR_Init(void);
extern UINT DIR_GetWindowsUnixDir( LPSTR path, UINT count );
extern UINT DIR_GetSystemUnixDir( LPSTR path, UINT count );
extern DWORD DIR_SearchAlternatePath( LPCSTR dll_path, LPCSTR name, LPCSTR ext,
                                      DWORD buflen, LPSTR buffer, LPSTR *lastpart);
extern DWORD DIR_SearchPath( LPCSTR path, LPCSTR name, LPCSTR ext,
                             DOS_FULL_NAME *full_name, BOOL win32 );

/* files/dos_fs.c */
extern void DOSFS_UnixTimeToFileTime( time_t unixtime, LPFILETIME ft,
                                      DWORD remainder );
extern time_t DOSFS_FileTimeToUnixTime( const FILETIME *ft, DWORD *remainder );
extern BOOL DOSFS_ToDosFCBFormat( LPCSTR name, LPSTR buffer );
extern const DOS_DEVICE *DOSFS_GetDevice( const char *name );
extern const DOS_DEVICE *DOSFS_GetDeviceByHandle( HFILE hFile );
extern HANDLE DOSFS_OpenDevice( const char *name, DWORD access, DWORD attributes, LPSECURITY_ATTRIBUTES sa);
extern BOOL DOSFS_FindUnixName( LPCSTR path, LPCSTR name, LPSTR long_buf,
                                  INT long_len, LPSTR short_buf,
                                  BOOL ignore_case );
extern BOOL DOSFS_GetFullName( LPCSTR name, BOOL check_last,
                                 DOS_FULL_NAME *full );
extern int DOSFS_FindNext( const char *path, const char *short_mask,
                           const char *long_mask, int drive, BYTE attr,
                           int skip, WIN32_FIND_DATAA *entry );

/* win32/device.c */
extern HANDLE DEVICE_Open( LPCSTR filename, DWORD access, LPSECURITY_ATTRIBUTES sa );

#endif  /* __WINE_FILE_H */
