#include "stdio.h"
#include "memory_manager.h"


#define PREALLOCATOR (void*)0
typedef unsigned TVOLUME;
typedef TVOLUME TCELL;
typedef TCELL TINDEX;
typedef unsigned* TMEMORY;

static const TVOLUME VOLUME = 0x07FFFFFF;
static const TVOLUME NULL_INDEX = VOLUME-VOLUME;
static const TVOLUME BVOLUME = VOLUME;
static const TVOLUME SVOLUME = VOLUME;

const unsigned maxc = 11570270;
static const unsigned dc = 8;

static volatile unsigned c;

int main (int argc, char** argv) {
    printf("Test\n");
    TCELL *m = new TCELL[VOLUME];
    memory_manager<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,void*,0,1,BVOLUME,4,2,PREALLOCATOR,SVOLUME> *mmu = new memory_manager<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,void*,0,1,BVOLUME,4,2,PREALLOCATOR,SVOLUME>((TMEMORY)m);

    TVOLUME v1, v2;
    TINDEX p1, p2;

    p2 = mmu->allocate(v2=maxc-dc);
    TVOLUME _4 = 0, _3 = 0, _c = 0, i = c;
    _4 += (p2 == NULL_INDEX)? 0: v2;
    for (i = c = dc+1; i < maxc;) {
	p1 = i;
	mmu->deallocate(v1=1,p1);
	_3 += (p1 == NULL_INDEX)? 0 : v1;
	i = c += 4;
	if ((i-1)%400==0) std::cout << i << "\t\r";
    }

    std::cout << std::endl;
    for (i = c = dc+3; i < maxc;) {
	p1 = i;
	mmu->deallocate(v1,p1);
	_3 += (p1 == NULL_INDEX)? 0 : v1;
	i = c += 4;
	if ((i-3)%400==0) std::cout << i << "\t\r";
    }

    std::cout << std::endl;
    for (i = c = dc+2; i < maxc;) {
	p1 = i;
	mmu->deallocate(v1,p1);
	_3 += (p1 == NULL_INDEX)? 0 : v1;
	i = c += 2;
	p2 = mmu->allocate(v2=2,p2);
	mmu->deallocate(v2=1,p2+1);
	if ((i-2)%400==0) std::cout << i << "\t\r";
    }
    std::cout << std::endl;

//    mmu->deallocate(v2,p2);


    delete mmu->m();
    return 0;
}