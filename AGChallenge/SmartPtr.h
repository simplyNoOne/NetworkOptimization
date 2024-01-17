#pragma once
#include <atomic>

class CRefCounter
{
public:
	CRefCounter() { iCount = 0; }
	int iAdd() { 
		return(++iCount); }
	int iDec() {
		return(--iCount); };
	int iGet() { return(iCount); }
private:
	std::atomic<int> iCount;
};


template <class T>
class CSmartPtr
{
public:

	CSmartPtr<T>() {
		pcPointer = nullptr;
	}
	CSmartPtr<T>(T* pcPointer)
	{
		this->pcPointer = pcPointer;
		pcCounter = new CRefCounter();
		pcCounter->iAdd();

	}

	CSmartPtr<T> operator=(const CSmartPtr<T>& cOther) {
		if (this == &cOther)
			return *this;

		if (pcPointer) {
			if (pcCounter->iDec() == 0)
			{
				delete pcPointer;
				delete pcCounter;
			}
		}
		
		pcPointer = cOther.pcPointer;
		pcCounter = cOther.pcCounter;
		pcCounter->iAdd();

		return *this;
	}

	CSmartPtr<T>(const CSmartPtr& pcOther)
	{
		pcPointer = pcOther.pcPointer;
		pcCounter = pcOther.pcCounter;
		pcCounter->iAdd();
	}


	~CSmartPtr<T>()
	{
		if (pcPointer) {
			if (pcCounter->iDec() == 0)
			{
				delete pcPointer;
				delete pcCounter;
			}
		}
	}

	T& operator*() const { return(*pcPointer); }
	T* operator->() const { return(pcPointer); }
private:
	CRefCounter* pcCounter;
	T* pcPointer;
};

