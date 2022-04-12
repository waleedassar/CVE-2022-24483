#include "stdafx.h"
#include "windows.h"
#include "stdio.h"

#include "Header.h"



ulonglong EnumProcesses_And_ExtractCreateTime(ulonglong argPID)
{
	ulong Size = 0x1000;
	void* p = VirtualAlloc(0,Size,MEM_COMMIT,PAGE_READWRITE);
	ulong ReturnedLength = 0;

	if(p)
	{
		int ret = ZwQuerySystemInformation(SystemProcessInformation,p,Size,&ReturnedLength);
		//printf("ZwQuerySystemInformation, ret: %X, ReturnedLength: %X\r\n",ret,ReturnedLength);


		if(ret == STATUS_INFO_LENGTH_MISMATCH)
		{
			VirtualFree(p,0,MEM_RELEASE);
			Size  = ReturnedLength;
			p = VirtualAlloc(0,Size,MEM_COMMIT,PAGE_READWRITE);
			if(p)
			{
				ret = ZwQuerySystemInformation(SystemProcessInformation,p,Size,&ReturnedLength);
				//printf("ZwQuerySystemInformation, ret: %X, ReturnedLength: %X\r\n",ret,ReturnedLength);
			}
			else
			{
				printf("EnumProcesses: Error allocating memory\r\n");
				return -1;
			}
		}



		_SYSTEM_PROCESS_INFORMATION* pSysInfo = (_SYSTEM_PROCESS_INFORMATION*)p;
		unsigned long Offset = 0;
		while(Offset < ReturnedLength)
		{
			//printf("Offset: %X\r\n",Offset);
			//printf("ReturnedLength: %X\r\n",ReturnedLength);

			if(pSysInfo->ImageName.Buffer)
			{
				//wprintf(L"=> %s, pid: %I64X\r\n",pSysInfo->ImageName.Buffer,pSysInfo->UniqueProcessId);
			}

			if( (pSysInfo->UniqueProcessId))
			{
				if(pSysInfo->UniqueProcessId == argPID)
				{
					return pSysInfo->CreateTime;
				}
			}

			Offset = pSysInfo->NextEntryOffset;
			pSysInfo = (_SYSTEM_PROCESS_INFORMATION*) (((char*)pSysInfo) + Offset);
		
			//printf("Press any key to continue\r\n");
			//getchar();

			if(Offset == 0)
			{
				printf("Done\r\n");
				break;
			}
		}
		VirtualFree(p,0,MEM_RELEASE);
	}

	return 0;
}

ulonglong GetProcessCreateTime_method_2(ulong pid)
{
	return EnumProcesses_And_ExtractCreateTime(pid);
}
//-------------


ulonglong GetProcessCreateTime_method_1(ulong pid)
{
	_KERNEL_USER_TIMES Times = {0};
	ulong ProcessId = pid;
	if(ProcessId)
	{
		_OBJECT_ATTRIBUTES ObjAttr = {sizeof(ObjAttr)};

		_CLIENT_ID Cid = {0};
		Cid.UniqueProcess = ProcessId;

		//PROCESS_QUERY_LIMITED_INFORMATION allows to Query processes with Higher IL

		HANDLE hProcess = 0;
		int ret = ZwOpenProcess(&hProcess,PROCESS_QUERY_LIMITED_INFORMATION,&ObjAttr,&Cid);
		if(ret >= 0)
		{
			ret = ZwQueryInformationProcess(hProcess,ProcessTimes,
								&Times,sizeof(Times),0);

			if(ret >= 0)
			{
				return Times.CreateTime;
			}
		}			
	}
	return 0;
}