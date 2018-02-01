#pragma once

#include "kernel/types.h"

/*!
 * Base class of every kernel object in the system.
 */
template<typename T, XboxTypes::KOBJECTS type> 
class KernelObject {
public:
	/*!
	 * Initializes the kernel object.
	 */
	virtual void Initialize() = 0;

private:
	XboxTypes::KOBJECTS m_objectType;
};

// Skeletons for a few object types
/*
class ThreadObject : public KernelObject<XboxTypes::KTHREAD, XboxTypes::ThreadObject> {
public:
	void Initialize();
};

// One base type for a given struct type that has more than one object type
template<XboxTypes::KOBJECTS type>
class BaseEventObject : public KernelObject<XboxTypes::KEVENT, type> {
public:
	void Initialize(); // initializes common fields on the struct
protected:
	virtual void InitializeEvent(); // provides subclasses a chance to fill in specific data
};

class SyncEventObject : public BaseEventObject<XboxTypes::EventSynchronizationObject> {
protected:
	void InitializeEvent();
};

class NotifEventObject : public BaseEventObject<XboxTypes::EventNotificationObject> {
protected:
	void InitializeEvent();
};
*/

/*!
 * The object manager is a core component of the Xbox kernel that provides
 * structured access to system resources, such as files, devices, threads,
 * synchronization entities and more.
 */
class ObjectManager {
public:
	// TODO: define
	// - methods to create and destroy objects
	// - add and remove references by pointer or handle
private:
	// TODO: object namespace
	// - Initialize basic structure with default directories and objects
};
