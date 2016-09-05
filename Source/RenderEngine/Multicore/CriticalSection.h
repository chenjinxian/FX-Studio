#pragma once

class CriticalSection : public boost::noncopyable
{
public:
	CriticalSection()
	{ 
		InitializeCriticalSection( &m_cs ); 
	}
 
	~CriticalSection()
	{
		DeleteCriticalSection( &m_cs ); 
	}
 
 
	void Lock()
	{ 
		EnterCriticalSection( &m_cs ); 
	}

	void Unlock()
	{ 
		LeaveCriticalSection( &m_cs ); 
	}
 
protected:
	   // the critical section itself
	mutable CRITICAL_SECTION m_cs;
};
 
 
/*
 Description
	  
	   helper class
 
 
	   allows automatic Locking/ Unlocking of a Resource,
	   protected by a Critical Section:
	   - locking when this object gets constructed
	   - unlocking when this object is destructed
		(goes out of scope)
 
 
 Usage
 
 
	   when you need protected access to a resource, do the following
	   1. Create a Critical Section associated with the resource
	   2. Embody the code accessing the resource in braces {}
	   3. Construct an ScopedCriticalSection object
 
 
  Example:
	   // we assume that m_CriticalSection
	   // is a private variable, and we use it to protect
	   // 'this' from being accessed while we need safe access to a resource
	  
 
 
	   // code that does not need resource locking
	  
	   {
			  ScopedCriticalSection I_am_locked( m_cs);
			 
			  // code that needs thread locking
	   }
	  
	   // code that does not need resource locking
 
 
*/
class ScopedCriticalSection : public boost::noncopyable
{
public:
	ScopedCriticalSection( CriticalSection & csResource)
		: m_csResource( csResource)
	{
		m_csResource.Lock();
	}
  
	~ScopedCriticalSection()
	{
		m_csResource.Unlock();
	}
 
private:
	   // the Critical Section for this resource
	   CriticalSection & m_csResource;
};
 
template<typename Data>
class concurrent_queue
{
private:
	std::queue<Data> the_queue;
	CriticalSection m_cs;
	//std::condition_variable the_condition_variable;
	HANDLE m_dataPushed;
public:
	concurrent_queue()
	{
		m_dataPushed = CreateEvent(NULL, TRUE, false, NULL);
	}

	void push(Data const& data)
	{
		//boost::mutex::scoped_lock lock(the_mutex);
		{
			ScopedCriticalSection locker(m_cs);
			the_queue.push(data);
		}
		//lock.unlock();
		//the_condition_variable.notify_one();
		PulseEvent(m_dataPushed);
	}

	bool empty() const
	{
		//boost::mutex::scoped_lock lock(the_mutex);
		ScopedCriticalSection locker(m_cs);
		return the_queue.empty();
	}

	bool try_pop(Data& popped_value)
	{
		//boost::mutex::scoped_lock lock(the_mutex);
		ScopedCriticalSection locker(m_cs);
		if(the_queue.empty())
		{
			return false;
		}
		
		popped_value=the_queue.front();
		the_queue.pop();
		return true;
	}

	void wait_and_pop(Data& popped_value)
	{
		//boost::mutex::scoped_lock lock(the_mutex);
		ScopedCriticalSection locker(m_cs);
		while(the_queue.empty())
		{
			//the_condition_variable.wait(lock);
			WaitForSingleObject(m_dataPushed);
		}
		
		popped_value=the_queue.front();
		the_queue.pop();
	}

};



