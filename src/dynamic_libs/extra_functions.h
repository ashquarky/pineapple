
void (* OSInitMutex)(void* mutex);
void (* OSLockMutex)(void* mutex);
void (* OSUnlockMutex)(void* mutex);
int (* OSTryLockMutex)(void* mutex);
int (*IOS_Ioctl)(int fd, unsigned int request, void *input_buffer,unsigned int input_buffer_len, void *output_buffer, unsigned int output_buffer_len);
int (*IOS_Open)(char *path, unsigned int mode);
int (*IOS_Close)(int fd);

void InitExtraFunctionPointers() {
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSInitMutex", &OSInitMutex);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSLockMutex", &OSLockMutex);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSUnlockMutex", &OSUnlockMutex);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSTryLockMutex", &OSTryLockMutex);
	OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Ioctl", &IOS_Ioctl);
	OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Open", &IOS_Open);
	OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Close", &IOS_Close);
}
