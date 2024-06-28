////////////////////////////////////////////////////////////////
//        File: themaphore.h
// Description: contents description of themaphore
//      Author: IVP
//        Date: 22.10.2006

#ifndef __THEMAPHORE_H__
#define __THEMAPHORE_H__

#include <fstream>
#include <iostream>
using namespace std;
#ifndef __EMPTY_H__
#include <empty.h>
#endif

#define UNX_SYNCHRO
//#define WIN_SYNCHRO
//#define WIN_SECTION
//#define EXPERIMENT

#ifdef WIN_SECTION
#ifndef WIN_SYNCHRO
#define WIN_SYNCHRO
#endif
#endif

#ifdef WIN_SYNCHRO
#include <windows.h>
#endif

#ifdef WIN_SECTION
#define WIN_DECLARE(x)	CRITICAL_SECTION x
#define WIN_CREATE(x)	InitializeCriticalSection(&(x))
#define WIN_ENTER(x)	EnterCriticalSection(&(x))
#define WIN_LEAVE(x)	LeaveCriticalSection(&(x))
#define WIN_DELETE(x)	DeleteCriticalSection(&(x))
#elif defined(WIN_SYNCHRO)
#define WIN_DECLARE(x)	HANDLE x
#define WIN_CREATE(x)	((x) = CreateMutex(NULL,FALSE,NULL))
#define WIN_ENTER(x)	WaitForSingleObject((x),INFINITE)
#define WIN_LEAVE(x)	ReleaseMutex(x)
#define WIN_DELETE(x)	CloseHandle(x)
#else
#define WIN_DECLARE(x)	pthread_mutex_t x
#define WIN_CREATE(x)	pthread_mutex_init(&(x),NULL)
#define WIN_ENTER(x)	pthread_mutex_lock(&(x))
#define WIN_LEAVE(x)	pthread_mutex_unlock(&(x))
#define WIN_DELETE(x)	pthread_mutex_destroy(&(x))
#endif

//#define EXPERIMENT
#ifdef WIN32
#define ASM_START	_asm {
#define ASM_LINE(x)	x
#define ASM_LINE2(x,y)	x,y
#define ASM_FINAL	}
#else
#define ASM_START	__asm__(
#define ASM_LINE(x)	"##x##"
#define ASM_LINE2(x,y)	"##x,y##"
#define ASM_FINAL	);
#endif


class Themaphore {
	typedef unsigned int uint;
#ifdef WIN_SYNCHRO
	WIN_DECLARE(s);
#ifdef LOG_SECTION
	WIN_DECLARE(m);
	WIN_DECLARE(z);
#endif
#elif defined(UNX_SYNCHRO)
	WIN_DECLARE(s);
	int c;
	pthread_t t;
#else
	volatile uint _s;
#endif
public:
#ifdef WIN_SYNCHRO
	Themaphore () {WIN_CREATE(s);
#ifdef LOG_SECTION
		WIN_CREATE(m);
		WIN_CREATE(z);
#endif
	};
	void clear() {WIN_DELETE(s);
#ifdef LOG_SECTION
		WIN_DELETE(m);
		WIN_DELETE(z);
#endif
	};
	~Themaphore () {clear();};
#elif defined(UNX_SYNCHRO)
	Themaphore ():c(0),t(0) {WIN_CREATE(s);
	};
	void clear() {WIN_DELETE(s);c=0;t=0;}
	~Themaphore () {clear();};
#else
	Themaphore (uint s = 1):_s(1<<s) {};
#endif
#ifdef WIN_SYNCHRO
#elif defined(UNX_SYNCHRO)
#else
	operator uint&() {	return (uint&)_s;};
	uint operator= (uint s) {	return _s = s;};
#endif
#ifdef WIN_SYNCHRO
	inline uint operator --() {	return enter(), 0;	};
#elif defined(UNX_SYNCHRO)
	inline uint operator --() {	return enter(), 0;	};
#else
	inline uint operator --() {	return enter(), _s;	};
#endif
	inline void enter() { //std::cout << "e:" << this << std::endl;
#ifdef WIN_SYNCHRO
#ifdef LOG_SECTION
		WIN_ENTER(z);
#ifdef WIN_SECTION
		WIN_ENTER(m);
		if (s.LockCount <= -1) {
			if (this < (void*)0x01000000)
				cerr << this->s.LockCount << endl;
            DWORD id = GetCurrentThreadId();
			if (s.OwningThread == (HANDLE)id) cerr << "?" << endl;
			WIN_LEAVE(m);
		}
#endif
#endif
		WIN_ENTER(s);
#ifdef LOG_SECTION
		WIN_LEAVE(z);
#endif
#elif defined(UNX_SYNCHRO)
		if ((c > 0) && (t == pthread_self())) {c++;cout << "duplicate:" << c << endl;}
		else {WIN_ENTER(s);
		t = pthread_self();c=1;
}
#else
		volatile uint* s = &_s;
		uint v = _s;
ASM_START
ASM_LINE(	push eax)
ASM_LINE2(	mov eax,dword ptr [s])
ASM_LINE2(	mov ebx,v)
ASM_LINE(l:)
ASM_LINE(	nop)
ASM_LINE2(	shr dword ptr [eax],1)
ASM_LINE(	jz l)
ASM_LINE(	pop eax)
ASM_FINAL
//				while ((_s >>= 1) == 0);
#endif
	};
#ifdef WIN_SYNCHRO
	inline void wait() {	enter();leave();	};
#elif defined(UNX_SYNCHRO)
	inline void wait() {	enter();leave();	};
#else
	inline void wait(uint s = 0) {	while (_s != (uint)1<<s);	};
#endif
	inline uint operator ++() {	return leave();	};
	inline uint operator ++(int s) {	return leave(s);	};
	inline uint leave(uint s = 1) { //std::cout << "l:" << this << std::endl;
#ifdef WIN_SYNCHRO
#ifdef LOG_SECTION
#ifdef WIN_SECTION
		WIN_ENTER(m);
		int r = this->s.LockCount;
		if ((r < -2) || (this < (void*)0x01000000)) {
			cerr << ":" << r;
			cerr << " " << this;
			if (r < -2)
				cerr << " " << GetCurrentThreadId();
			cerr << endl;
		}
		WIN_LEAVE(m);
#endif
#endif
		return WIN_LEAVE(this->s), s;
#elif defined(EXPERIMENT)
		uint* __s = &s;
		uint* ___s = &_s;
ASM_START
ASM_LINE(	push eax)
ASM_LINE(	push ebx)
ASM_LINE2(	mov ebx,1)
ASM_LINE2(	mov eax,dword ptr [__s])
ASM_LINE2(	shl ebx,1)
ASM_LINE2(	mov eax,dword ptr [___s])
ASM_LINE(l0:)
ASM_LINE(	nop)
ASM_LiNE2(	or dword ptr [eax],ebx)
ASM_LINE(	jz l0)
ASM_LINE(	pop ebx)
ASM_LINE(	pop eax)
ASM_FINAL
		return _s;
#elif defined(UNX_SYNCHRO)
		if (t == pthread_self()) c--;
		return (c==0)?WIN_LEAVE(this->s):0;
#else
		return _s |= (1<<s); 
#endif
	};
};

////////////////////////////////////////////////////////////////
template <typename S, class B = _empty> class lockable {
#ifdef WIN_SYNCHRO
	S s;
#elif defined(UNX_SYCHRO)
	S s;
#else
	S s, m;
#endif
public:
#ifdef WIN_SYNCHRO
	lockable ():s() {};
#elif defined(UNX_SYNCHRO)
	lockable ():s() {};
#else
	lockable ():s(),m() {};
	lockable (S _s):s(_s),m() {	};
#endif
	int lock () {
#ifdef WIN_SYNCHRO
		return --s;
#elif defined(UNX_SYNCHRO)
		return --s;
#else
		int r;
		/*static S*/ m;
#ifdef EXPERIMENT
		cout << "!" << endl;
#endif
		--m;
		r = --s;
		++m;
		return r;
#endif
	};
	int unlock() {
#ifdef WIN_SYNCHRO
		return ++s;
#elif defined(EXPERIMENT)
		int r;
		cout << "~" << endl;
		if ((r = ++s) == 0) {
			cout << "#" << endl;
		}
		return r;
#else
		return ++s;
#endif
	};
	operator S&() {return s;};
	int operator !() {return lock();};
	int operator ~() {return unlock();};
//	int _lock() {cout << "e:" << this << endl; int r = lock(); if (r != 0) cout << "error::" << r << endl; return r;};
//	int _unlock() {cout << "l:" << this << endl; return unlock();};
};
#endif//__THEMAPHORE_H__
