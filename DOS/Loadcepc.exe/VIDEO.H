#pragma pack(1)
typedef struct _VESA_GENERAL_INFO
{
    UCHAR       szSignature[4];
    WORD        wVersion;
    UCHAR far * pszVendor;
    DWORD       dwCapabilities;
    WORD far *  pModeList;
    WORD        wTotalMemory;
    UCHAR       ucReserved[236];
}   VESA_GENERAL_INFO;

typedef struct _VESA_MODE_INFO
{
    WORD        wModeAttributes;
    UCHAR       ucWindowAAttributes;
    UCHAR       ucWindowBAttributes;
    WORD        wWindowGranularity;
    WORD        wWindowSize;
    WORD        wWindowASegment;
    WORD        wWindowBSegment;
    DWORD       pWindowSchemeFunction;
    WORD        wBytesPerScanLine;
    UCHAR       ucReserved[238];
}   VESA_MODE_INFO;

#pragma pack()
