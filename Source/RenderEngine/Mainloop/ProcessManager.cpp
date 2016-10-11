#include "ProcessManager.h"

ProcessManager::~ProcessManager(void)
{
	ClearAllProcesses();
}


//---------------------------------------------------------------------------------------------------------------------
// The process update tick.  Called every logic tick.  This function returns the number of process chains that 
// succeeded in the upper 32 bits and the number of process chains that failed or were aborted in the lower 32 bits.
//---------------------------------------------------------------------------------------------------------------------
unsigned int ProcessManager::UpdateProcesses(unsigned long deltaMs)
{
	unsigned short int successCount = 0;
	unsigned short int failCount = 0;

	ProcessList::iterator it = m_processList.begin();
	while (it != m_processList.end())
	{
		// grab the next process
		StrongProcessPtr pCurrProcess = (*it);

		// save the iterator and increment the old one in case we need to remove this process from the list
		ProcessList::iterator thisIt = it;
		++it;

		// process is uninitialized, so initialize it
		if (pCurrProcess->GetState() == Process::UNINITIALIZED)
			pCurrProcess->VOnInit();

		// give the process an update tick if it's running
		if (pCurrProcess->GetState() == Process::RUNNING)
			pCurrProcess->VOnUpdate(deltaMs);

		// check to see if the process is dead
		if (pCurrProcess->IsDead())
		{
			// run the appropriate exit function
			switch (pCurrProcess->GetState())
			{
				case Process::SUCCEEDED :
				{
					pCurrProcess->VOnSuccess();
					StrongProcessPtr pChild = pCurrProcess->RemoveChild();
					if (pChild)
						AttachProcess(pChild);
					else
						++successCount;  // only counts if the whole chain completed
					break;
				}

				case Process::FAILED :
				{
					pCurrProcess->VOnFail();
					++failCount;
					break;
				}

				case Process::ABORTED :
				{
					pCurrProcess->VOnAbort();
					++failCount;
					break;
				}
			}

			// remove the process and destroy it
			m_processList.erase(thisIt);
		}
	}

	return ((successCount << 16) | failCount);
}


//---------------------------------------------------------------------------------------------------------------------
// Attaches the process to the process list so it can be run on the next update.
//---------------------------------------------------------------------------------------------------------------------
WeakProcessPtr ProcessManager::AttachProcess(StrongProcessPtr pProcess)
{
	m_processList.push_front(pProcess);
	return WeakProcessPtr(pProcess);
}

//---------------------------------------------------------------------------------------------------------------------
// Clears all processes (and DOESN'T run any exit code)
//---------------------------------------------------------------------------------------------------------------------
void ProcessManager::ClearAllProcesses(void)
{
	m_processList.clear();
}

//---------------------------------------------------------------------------------------------------------------------
// Aborts all processes.  If immediate == true, it immediately calls each ones OnAbort() function and destroys all 
// the processes.
//---------------------------------------------------------------------------------------------------------------------
void ProcessManager::AbortAllProcesses(bool immediate)
{
	ProcessList::iterator it = m_processList.begin();
	while (it != m_processList.end())
	{
		ProcessList::iterator tempIt = it;
		++it;

		StrongProcessPtr pProcess = *tempIt;
		if (pProcess->IsAlive())
		{
			pProcess->SetState(Process::ABORTED);
			if (immediate)
			{
				pProcess->VOnAbort();
				m_processList.erase(tempIt);
			}
		}
	}
}


