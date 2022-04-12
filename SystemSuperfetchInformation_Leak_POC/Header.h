#pragma once
#include "windows.h"


#define ulong unsigned long
#define ulonglong unsigned long long
#define ULONG unsigned long
#define ULONGLONG unsigned long long
#define ushort unsigned short
#define USHORT unsigned short
#define uchar unsigned char
#define UCHAR unsigned char

#define STATUS_INFO_LENGTH_MISMATCH       0xC0000004

#define ProcessTimes 0x4

#define SystemSuperfetchInformation 0x4F
#define SystemProcessInformation 0x5

//Flags
#define QueryProcessWorkingSetSwapPages 1
#define ProcessQueryStoreStats 2
#define BypassMinimalProcesses 4

struct _UNICODE_STRING
{
	unsigned short Length;
	unsigned short MaxLength;
	unsigned long Pad;
	wchar_t* Buffer;
};


struct _OBJECT_ATTRIBUTES {
  ULONGLONG           Length;
  HANDLE          RootDirectory;
  _UNICODE_STRING* ObjectName;
  ULONGLONG           Attributes;
  PVOID           SecurityDescriptor;
  PVOID           SecurityQualityOfService;
};


struct _CLIENT_ID
{
	ulonglong UniqueProcess;
	ulonglong UniqueThread;
};



//Size 0x50
struct _SYSTEM_THREAD_INFORMATION
{
	ulonglong ApcTime;//at 0x0
	ulonglong UserTime;//at 0x8
	ulonglong CreateTime;//at 0x10
	ulong WaitTime;//at 0x18
	ulong Pad;//at 0x1C
	ulonglong StartAddress;//at 0x20
	_CLIENT_ID CID;//at 0x28
	ulong Priority;//at 0x38
	ulong BasePriority;//at 0x3C
	ulong ContextSwitches;//at 0x40
	ulong ThreadState;//at 0x44
	ulong WaitReason;//at 0x48
	ulong Pad1;//at 0x4C
};


typedef struct _SYSTEM_PROCESS_INFORMATION {
  ULONG                   NextEntryOffset; //at 0x0
  ULONG                   NumberOfThreads; //at 0x4
  ULONGLONG					WorkingSetPrivateSize;//at 0x8
  ULONG HardFaultCount; //at 0x10
  ULONG					ActiveThreadsHighWatermark;//at 0x14
  ULONGLONG				CycleTime;//at 0x18
  ULONGLONG           CreateTime; //at 0x20
  ULONGLONG           UserTime;//at 0x28
  ULONGLONG           KernelTime;//at 0x30
  _UNICODE_STRING          ImageName;//at 0x38
  ULONG               BasePriority; //at 0x48
  ULONG PAD0;//at 0x4C
  ULONGLONG                  UniqueProcessId;//at 0x50
  ULONGLONG                  InheritedFromUniqueProcessId;//at 0x58
  ULONG HandleCount;//at 0x60
  ULONG SessionId;//at 0x64
  ULONGLONG UniqueProcessId_;//at 0x68
  ULONGLONG       PeakVirtualSize;//at 0x70
  ULONGLONG		  VirtualSize;//at 0x78
  ULONG			PageFaultCount;//at 0x80
  ULONG PAD1;//at 0x84
  ULONGLONG PeakWorkingSetSize;//at 0x88
  ULONGLONG WorkingSetSize;	   //at 0x90
  ULONGLONG ProcessQuotaPeak_1; //at 0x98
  ULONGLONG ProcessQuotaUsage_1;//at 0xA0
  ULONGLONG ProcessQuotaPeak_0; //at 0xA8
  ULONGLONG ProcessQuotaUsage_0;//at 0xB0
  ULONGLONG CommitCharge;//at 0xB8
  ULONGLONG CommitChargePeak;//at 0xC0
  ULONGLONG CommitCharge_;//at 0xC8
  ULONGLONG ReadOperationCount;//at 0xD0
  ULONGLONG WriteOperationCount;//at 0xD8
  ULONGLONG OtherOperationCount;//at 0xE0
  ULONGLONG ReadTransferCount;//at 0xE8
  ULONGLONG WriteTransferCount;//at 0xF0
  ULONGLONG OtherTransferCount;//at 0xF8
  _SYSTEM_THREAD_INFORMATION           Threads[1];//at 0x100
} SYSTEM_PROCESS_INFORMATION;







struct _SUPERFETCH_INFORMATION
{
	ulong Class;
	ulong Signature;
	ulong SubClass;
	ulong field_C;
	ulonglong pTrace;
	ulong TraceSize;
	ulong field_1C;
};




struct _BUFF_0x60
{
  ulong Type;
  ulong SessionIdOrProcessId;
  ulong ImagePathHash;
  ulong field_C;
  ulonglong CreateTimeXored;
  ulonglong SessionGlobalVA;
  ulonglong WorkingSetPrivateSize;
  ulonglong NumberOfPrivatePages;
  ulong SessionId;
  char Description[16];
  ulong field_44;
  ulonglong WorkingSetSwapPages;
  ulonglong WorkingSetSize;
  ulong DeepFreezeState;
  ulong ProcessAttributes;
};

struct _PREFETCH_PRIV_SOURCE_ENUM
{
  ulong Revision;
  ulong Flags;
  ulong Output;
  ulong Pad;
  //_BUFF_0x60 Buff60[1];
};




typedef struct _KERNEL_USER_TIMES
{
    ulonglong CreateTime;
    ulonglong ExitTime;
    ulonglong KernelTime;
    ulonglong UserTime;
} KERNEL_USER_TIMES, *PKERNEL_USER_TIMES;




extern "C"
{
		int ZwQuerySystemInformation(int InfoClass,void* OutputBuffer,ulong OutputBufferLength,ulong* pReturnedLength);
		int ZwOpenProcess(HANDLE* ProcessHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,_CLIENT_ID* ClientId );
		int ZwQueryInformationProcess(HANDLE hProcess,ulonglong InfoClass,void* pInfo,ulonglong InfoLength,ulonglong* pRequiredLength);
}




ulonglong GetProcessCreateTime_method_2(ulong pid);
ulonglong GetProcessCreateTime_method_1(ulong pid);
void PrintBuff60(_BUFF_0x60* p60);