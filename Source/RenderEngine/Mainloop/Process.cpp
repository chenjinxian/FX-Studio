#include "stdafx.h"
#include "Process.h"


//---------------------------------------------------------------------------------------------------------------------
// Constructor
//---------------------------------------------------------------------------------------------------------------------
Process::Process(void)
{
	m_state = UNINITIALIZED;
	//m_pParent = NULL;
	//m_pChild = NULL;
}


//---------------------------------------------------------------------------------------------------------------------
// Destructor
//---------------------------------------------------------------------------------------------------------------------
Process::~Process(void)
{
	if (m_pChild)
	{
		m_pChild->VOnAbort();
	}
}


/*
//---------------------------------------------------------------------------------------------------------------------
// Attaches this process to the passed in parent.  It's perfectly valid for this process to have been run and you can
// safely call it from within this process' OnUpdate() function.
//---------------------------------------------------------------------------------------------------------------------
bool Process::AttachToParent(Process* pParent)
{
	// If the process is alive, we need to mark it to be removed from the process list without being destroyed.  If 
	// the process is dead, kick and error and don't attach it.  If the process has already been marked for removal,
	// it means we're trying to double-attach this process, which is an error.
	if (IsAlive())
	{
		m_state = REMOVED;
	}
	else if (IsDead())
	{
		GCC_ERROR("Attempting to attach a dead process to a parent");
		return false;
	}
	else if (IsRemoved())
	{
		GCC_ERROR("Attempting to attach a process that is already being removed to another parent");
		return false;
	}

	// Attach this process to the new parent.  It will be attached at the very end of the child tree.
	pParent->AttachChild(this);

	return true;
}
*/


//---------------------------------------------------------------------------------------------------------------------
// Removes the child from this process.  This releases ownership of the child to the caller and completely removes it
// from the process chain.
//---------------------------------------------------------------------------------------------------------------------
StrongProcessPtr Process::RemoveChild(void)
{
	if (m_pChild)
	{
		StrongProcessPtr pChild = m_pChild;  // this keeps the child from getting destroyed when we clear it
		m_pChild.reset();
		//pChild->SetParent(NULL);
		return pChild;
	}

	return StrongProcessPtr();
}

/*
//---------------------------------------------------------------------------------------------------------------------
// Cancels the top level process in this chain by calling its Fail() function.
//---------------------------------------------------------------------------------------------------------------------
void Process::CancelTopLevelProcess(void)
{
	StrongProcessPtr pTop = GetTopLevelProcess();
	pTop->Fail();
}
*/
