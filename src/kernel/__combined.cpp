#include "AvGetSavedDataAddress.cpp"
#include "AvSendTVEncoderOption.cpp"
#include "AvSetDisplayMode.cpp"
#include "AvSetSavedDataAddress.cpp"
#include "DbgBreakPoint.cpp"
#include "DbgBreakPointWithStatus.cpp"
#include "DbgLoadImageSymbols.cpp"
#include "DbgPrint.cpp"
#include "DbgPrompt.cpp"
#include "DbgUnLoadImageSymbols.cpp"
#include "ExAcquireReadWriteLockExclusive.cpp"
#include "ExAcquireReadWriteLockShared.cpp"
#include "ExAllocatePool.cpp"
#include "ExAllocatePoolWithTag.cpp"
#include "ExfInterlockedInsertHeadList.cpp"
#include "ExfInterlockedInsertTailList.cpp"
#include "ExfInterlockedRemoveHeadList.cpp"
#include "ExFreePool.cpp"
#include "ExInitializeReadWriteLock.cpp"
#include "ExInterlockedAddLargeInteger.cpp"
#include "ExInterlockedAddLargeStatistic.cpp"
#include "ExInterlockedCompareExchange64.cpp"
#include "ExQueryNonVolatileSetting.cpp"
#include "ExQueryPoolBlockSize.cpp"
#include "ExRaiseException.cpp"
#include "ExRaiseStatus.cpp"
#include "ExReadWriteRefurbInfo.cpp"
#include "ExReleaseReadWriteLock.cpp"
#include "ExSaveNonVolatileSetting.cpp"
#include "FscGetCacheSize.cpp"
#include "FscInvalidateIdleBlocks.cpp"
#include "FscSetCacheSize.cpp"
#include "HalClearSoftwareInterrupt.cpp"
#include "HalDisableSystemInterrupt.cpp"
#include "HalEnableSecureTrayEject.cpp"
#include "HalEnableSystemInterrupt.cpp"
#include "HalGetInterruptVector.cpp"
#include "HalInitiateShutdown.cpp"
#include "HalIsResetOrShutdownPending.cpp"
#include "HalReadSMBusValue.cpp"
#include "HalReadSMCTrayState.cpp"
#include "HalReadWritePCISpace.cpp"
#include "HalRegisterShutdownNotification.cpp"
#include "HalRequestSoftwareInterrupt.cpp"
#include "HalReturnToFirmware.cpp"
#include "HalWriteSMBusValue.cpp"
#include "HalWriteSMCScratchRegister.cpp"
#include "InterlockedCompareExchange.cpp"
#include "InterlockedDecrement.cpp"
#include "InterlockedExchange.cpp"
#include "InterlockedExchangeAdd.cpp"
#include "InterlockedFlushSList.cpp"
#include "InterlockedIncrement.cpp"
#include "InterlockedPopEntrySList.cpp"
#include "InterlockedPushEntrySList.cpp"
#include "IoAllocateIrp.cpp"
#include "IoBuildAsynchronousFsdRequest.cpp"
#include "IoBuildDeviceIoControlRequest.cpp"
#include "IoBuildSynchronousFsdRequest.cpp"
#include "IoCheckShareAccess.cpp"
#include "IoCreateDevice.cpp"
#include "IoCreateFile.cpp"
#include "IoCreateSymbolicLink.cpp"
#include "IoDeleteDevice.cpp"
#include "IoDeleteSymbolicLink.cpp"
#include "IoDismountVolume.cpp"
#include "IoDismountVolumeByName.cpp"
#include "IofCallDriver.cpp"
#include "IofCompleteRequest.cpp"
#include "IoFreeIrp.cpp"
#include "IoInitializeIrp.cpp"
#include "IoInvalidDeviceRequest.cpp"
#include "IoMarkIrpMustComplete.cpp"
#include "IoQueryFileInformation.cpp"
#include "IoQueryVolumeInformation.cpp"
#include "IoQueueThreadIrp.cpp"
#include "IoRemoveShareAccess.cpp"
#include "IoSetIoCompletion.cpp"
#include "IoSetShareAccess.cpp"
#include "IoStartNextPacket.cpp"
#include "IoStartNextPacketByKey.cpp"
#include "IoStartPacket.cpp"
#include "IoSynchronousDeviceIoControlRequest.cpp"
#include "IoSynchronousFsdRequest.cpp"
#include "KeAlertResumeThread.cpp"
#include "KeAlertThread.cpp"
#include "KeBoostPriorityThread.cpp"
#include "KeBugCheck.cpp"
#include "KeBugCheckEx.cpp"
#include "KeCancelTimer.cpp"
#include "KeConnectInterrupt.cpp"
#include "KeDelayExecutionThread.cpp"
#include "KeDisconnectInterrupt.cpp"
#include "KeEnterCriticalRegion.cpp"
#include "KeGetCurrentIrql.cpp"
#include "KeGetCurrentThread.cpp"
#include "KeInitializeApc.cpp"
#include "KeInitializeDeviceQueue.cpp"
#include "KeInitializeDpc.cpp"
#include "KeInitializeEvent.cpp"
#include "KeInitializeInterrupt.cpp"
#include "KeInitializeMutant.cpp"
#include "KeInitializeQueue.cpp"
#include "KeInitializeSemaphore.cpp"
#include "KeInitializeTimerEx.cpp"
#include "KeInsertByKeyDeviceQueue.cpp"
#include "KeInsertDeviceQueue.cpp"
#include "KeInsertHeadQueue.cpp"
#include "KeInsertQueue.cpp"
#include "KeInsertQueueApc.cpp"
#include "KeInsertQueueDpc.cpp"
#include "KeIsExecutingDpc.cpp"
#include "KeLeaveCriticalRegion.cpp"
#include "KePulseEvent.cpp"
#include "KeQueryBasePriorityThread.cpp"
#include "KeQueryInterruptTime.cpp"
#include "KeQueryPerformanceCounter.cpp"
#include "KeQueryPerformanceFrequency.cpp"
#include "KeQuerySystemTime.cpp"
#include "KeRaiseIrqlToDpcLevel.cpp"
#include "KeRaiseIrqlToSynchLevel.cpp"
#include "KeReleaseMutant.cpp"
#include "KeReleaseSemaphore.cpp"
#include "KeRemoveByKeyDeviceQueue.cpp"
#include "KeRemoveDeviceQueue.cpp"
#include "KeRemoveEntryDeviceQueue.cpp"
#include "KeRemoveQueue.cpp"
#include "KeRemoveQueueDpc.cpp"
#include "KeResetEvent.cpp"
#include "KeRestoreFloatingPointState.cpp"
#include "KeResumeThread.cpp"
#include "KeRundownQueue.cpp"
#include "KeSaveFloatingPointState.cpp"
#include "KeSetBasePriorityThread.cpp"
#include "KeSetDisableBoostThread.cpp"
#include "KeSetEvent.cpp"
#include "KeSetEventBoostPriority.cpp"
#include "KeSetPriorityProcess.cpp"
#include "KeSetPriorityThread.cpp"
#include "KeSetTimer.cpp"
#include "KeSetTimerEx.cpp"
#include "KeStallExecutionProcessor.cpp"
#include "KeSuspendThread.cpp"
#include "KeSynchronizeExecution.cpp"
#include "KeTestAlertThread.cpp"
#include "KeWaitForMultipleObjects.cpp"
#include "KeWaitForSingleObject.cpp"
#include "KfLowerIrql.cpp"
#include "KfRaiseIrql.cpp"
#include "KiUnlockDispatcherDatabase.cpp"
#include "MmAllocateContiguousMemory.cpp"
#include "MmAllocateContiguousMemoryEx.cpp"
#include "MmAllocateSystemMemory.cpp"
#include "MmClaimGpuInstanceMemory.cpp"
#include "MmCreateKernelStack.cpp"
#include "MmDbgAllocateMemory.cpp"
#include "MmDbgFreeMemory.cpp"
#include "MmDbgQueryAvailablePages.cpp"
#include "MmDbgReleaseAddress.cpp"
#include "MmDbgWriteCheck.cpp"
#include "MmDeleteKernelStack.cpp"
#include "MmFreeContiguousMemory.cpp"
#include "MmFreeSystemMemory.cpp"
#include "MmGetPhysicalAddress.cpp"
#include "MmIsAddressValid.cpp"
#include "MmLockUnlockBufferPages.cpp"
#include "MmLockUnlockPhysicalPage.cpp"
#include "MmMapIoSpace.cpp"
#include "MmPersistContiguousMemory.cpp"
#include "MmQueryAddressProtect.cpp"
#include "MmQueryAllocationSize.cpp"
#include "MmQueryStatistics.cpp"
#include "MmSetAddressProtect.cpp"
#include "MmUnmapIoSpace.cpp"
#include "NtAllocateVirtualMemory.cpp"
#include "NtCancelTimer.cpp"
#include "NtClearEvent.cpp"
#include "NtClose.cpp"
#include "NtCreateDirectoryObject.cpp"
#include "NtCreateEvent.cpp"
#include "NtCreateFile.cpp"
#include "NtCreateIoCompletion.cpp"
#include "NtCreateMutant.cpp"
#include "NtCreateSemaphore.cpp"
#include "NtCreateTimer.cpp"
#include "NtDeleteFile.cpp"
#include "NtDeviceIoControlFile.cpp"
#include "NtDuplicateObject.cpp"
#include "NtFlushBuffersFile.cpp"
#include "NtFreeVirtualMemory.cpp"
#include "NtFsControlFile.cpp"
#include "NtOpenDirectoryObject.cpp"
#include "NtOpenFile.cpp"
#include "NtOpenSymbolicLinkObject.cpp"
#include "NtProtectVirtualMemory.cpp"
#include "NtPulseEvent.cpp"
#include "NtQueryDirectoryFile.cpp"
#include "NtQueryDirectoryObject.cpp"
#include "NtQueryEvent.cpp"
#include "NtQueryFullAttributesFile.cpp"
#include "NtQueryInformationFile.cpp"
#include "NtQueryIoCompletion.cpp"
#include "NtQueryMutant.cpp"
#include "NtQuerySemaphore.cpp"
#include "NtQuerySymbolicLinkObject.cpp"
#include "NtQueryTimer.cpp"
#include "NtQueryVirtualMemory.cpp"
#include "NtQueryVolumeInformationFile.cpp"
#include "NtQueueApcThread.cpp"
#include "NtReadFile.cpp"
#include "NtReadFileScatter.cpp"
#include "NtReleaseMutant.cpp"
#include "NtReleaseSemaphore.cpp"
#include "NtRemoveIoCompletion.cpp"
#include "NtResumeThread.cpp"
#include "NtSetEvent.cpp"
#include "NtSetInformationFile.cpp"
#include "NtSetIoCompletion.cpp"
#include "NtSetSystemTime.cpp"
#include "NtSetTimerEx.cpp"
#include "NtSignalAndWaitForSingleObjectEx.cpp"
#include "NtSuspendThread.cpp"
#include "NtUserIoApcDispatcher.cpp"
#include "NtWaitForMultipleObjectsEx.cpp"
#include "NtWaitForSingleObject.cpp"
#include "NtWaitForSingleObjectEx.cpp"
#include "NtWriteFile.cpp"
#include "NtWriteFileGather.cpp"
#include "NtYieldExecution.cpp"
#include "ObCreateObject.cpp"
#include "ObfDereferenceObject.cpp"
#include "ObfReferenceObject.cpp"
#include "ObInsertObject.cpp"
#include "ObMakeTemporaryObject.cpp"
#include "ObOpenObjectByName.cpp"
#include "ObOpenObjectByPointer.cpp"
#include "ObReferenceObjectByHandle.cpp"
#include "ObReferenceObjectByName.cpp"
#include "ObReferenceObjectByPointer.cpp"
#include "PhyGetLinkState.cpp"
#include "PhyInitialize.cpp"
#include "PsCreateSystemThread.cpp"
#include "PsCreateSystemThreadEx.cpp"
#include "PsQueryStatistics.cpp"
#include "PsSetCreateThreadNotifyRoutine.cpp"
#include "PsTerminateSystemThread.cpp"
#include "READ_PORT_BUFFER_UCHAR.cpp"
#include "READ_PORT_BUFFER_ULONG.cpp"
#include "READ_PORT_BUFFER_USHORT.cpp"
#include "RtlAnsiStringToUnicodeString.cpp"
#include "RtlAppendStringToString.cpp"
#include "RtlAppendUnicodeStringToString.cpp"
#include "RtlAppendUnicodeToString.cpp"
#include "RtlAssert.cpp"
#include "RtlCaptureContext.cpp"
#include "RtlCaptureStackBackTrace.cpp"
#include "RtlCharToInteger.cpp"
#include "RtlCompareMemory.cpp"
#include "RtlCompareMemoryUlong.cpp"
#include "RtlCompareString.cpp"
#include "RtlCompareUnicodeString.cpp"
#include "RtlCopyString.cpp"
#include "RtlCopyUnicodeString.cpp"
#include "RtlCreateUnicodeString.cpp"
#include "RtlDowncaseUnicodeChar.cpp"
#include "RtlDowncaseUnicodeString.cpp"
#include "RtlEnterCriticalSection.cpp"
#include "RtlEnterCriticalSectionAndRegion.cpp"
#include "RtlEqualString.cpp"
#include "RtlEqualUnicodeString.cpp"
#include "RtlExtendedIntegerMultiply.cpp"
#include "RtlExtendedLargeIntegerDivide.cpp"
#include "RtlExtendedMagicDivide.cpp"
#include "RtlFillMemory.cpp"
#include "RtlFillMemoryUlong.cpp"
#include "RtlFreeAnsiString.cpp"
#include "RtlFreeUnicodeString.cpp"
#include "RtlGetCallersAddress.cpp"
#include "RtlInitAnsiString.cpp"
#include "RtlInitializeCriticalSection.cpp"
#include "RtlInitUnicodeString.cpp"
#include "RtlIntegerToChar.cpp"
#include "RtlIntegerToUnicodeString.cpp"
#include "RtlLeaveCriticalSection.cpp"
#include "RtlLeaveCriticalSectionAndRegion.cpp"
#include "RtlLowerChar.cpp"
#include "RtlMapGenericMask.cpp"
#include "RtlMoveMemory.cpp"
#include "RtlMultiByteToUnicodeN.cpp"
#include "RtlMultiByteToUnicodeSize.cpp"
#include "RtlNtStatusToDosError.cpp"
#include "RtlRaiseException.cpp"
#include "RtlRaiseStatus.cpp"
#include "RtlRip.cpp"
#include "RtlSnprintf.cpp"
#include "RtlSprintf.cpp"
#include "RtlTimeFieldsToTime.cpp"
#include "RtlTimeToTimeFields.cpp"
#include "RtlTryEnterCriticalSection.cpp"
#include "RtlUlongByteSwap.cpp"
#include "RtlUnicodeStringToAnsiString.cpp"
#include "RtlUnicodeStringToInteger.cpp"
#include "RtlUnicodeToMultiByteN.cpp"
#include "RtlUnicodeToMultiByteSize.cpp"
#include "RtlUnwind.cpp"
#include "RtlUpcaseUnicodeChar.cpp"
#include "RtlUpcaseUnicodeString.cpp"
#include "RtlUpcaseUnicodeToMultiByteN.cpp"
#include "RtlUpperChar.cpp"
#include "RtlUpperString.cpp"
#include "RtlUshortByteSwap.cpp"
#include "RtlVsnprintf.cpp"
#include "RtlVsprintf.cpp"
#include "RtlWalkFrameChain.cpp"
#include "RtlZeroMemory.cpp"
#include "WRITE_PORT_BUFFER_UCHAR.cpp"
#include "WRITE_PORT_BUFFER_ULONG.cpp"
#include "WRITE_PORT_BUFFER_USHORT.cpp"
#include "XcBlockCrypt.cpp"
#include "XcBlockCryptCBC.cpp"
#include "XcCryptService.cpp"
#include "XcDESKeyParity.cpp"
#include "XcHMAC.cpp"
#include "XcKeyTable.cpp"
#include "XcModExp.cpp"
#include "XcPKDecPrivate.cpp"
#include "XcPKEncPublic.cpp"
#include "XcPKGetKeyLen.cpp"
#include "XcRC4Crypt.cpp"
#include "XcRC4Key.cpp"
#include "XcSHAFinal.cpp"
#include "XcSHAInit.cpp"
#include "XcSHAUpdate.cpp"
#include "XcUpdateCrypto.cpp"
#include "XcVerifyPKCS1Signature.cpp"
#include "XeLoadSection.cpp"
#include "XeUnloadSection.cpp"