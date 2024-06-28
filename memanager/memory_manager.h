//  Copyright 2019 Valerian Ivashenko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.



#pragma once
//#define SLOW_EDITION
#ifdef SLOW_EDITION
#define SLOW_ALLOCATOR
#endif
//#include "themaphore.h"
//namespace mema {
#ifdef WIN_SECTION
#include <themaphore.h>
#else
#ifdef WIN32
#define WIN_SECTION
#include <themaphore.h>
#undef WIN_SECTION
#else
//#define _MT
#define UNX_SYNCHRO
#include <themaphore.h>
#undef UNX_SYNCHRO
#endif
#endif
//#undef __THEMAPHORE_H__
//}
#define RDTSC	__asm _emit 0x0f __asm _emit 0x31 

#ifndef x64
#ifdef WIN32
typedef unsigned __int64 t_int64;
#define RDTSC_INLINE	__inline
#else
typedef unsigned long long t_int64;
#define RDTSC_INLINE	inline
#endif

RDTSC_INLINE t_int64 _rdtsc() {
	t_int64 x;
ASM_START
//ASM_LINE(	push	eax)
//ASM_LINE(	push	edx)
ASM_LINE(	RDTSC)
ASM_LINE2(	mov dword ptr [x],eax)
ASM_LINE2(	mov dword ptr [x]+4,edx)
//ASM_LINE(	pop		edx)
//ASM_LINE(	pop		eax)
ASM_FINAL
//	__asm volatile (".byte 0x0f, 0x31" : "=A" (x));
	return x;
}

#endif

//SPECIFICATION
//let S - address space
//let U - used memory volume
//let C - static used memory volume
//let V - dynamic memory volume V = U-C
//let n - maximal allocated block volume
//there is statements
//safe memory volume:
//	U+min(n-1,floor(V/2)*max(V-n,floor(V/2))
//	U+(n-1)*(V-n)
//	S
//	worst case
//	U+floor(V/2)^2
//	S
//(*)
//	U+floor((n-1)/2)*((V/floor((n+1)/2))-2)<U+V
//	S
//maximal fragmentation
//	min(n-1,floor(V/2))*max(V-n,floor(V/2))/U
//	(n-1)*(V-n)/U
//	min(((n-1)-((n-1)*(n^2)/(n*(n-1)+S)))*100%,(fsqrt(S+1)-1)*50%)<=min((n-1)*100%,(fsqrt(S+1)-1)*50%)
//	worst case
//	(floor(V/2)^2)/U
//	(fsqrt(S+1)-1)*50%
//(*)
//	floor((n-1)/2)*((V/floor((n+1)/2))-2)/U
//	floor((n-1)/2)*((S/floor((n+1)/2))-2)/S<100%
//safe address space usage
//	S >= U+min(n-1,floor(V/2)*max(V-n,floor(V/2))
//		U <= min(n-1+((S-C)/n),2*fsqrt(S+1-C)-2)+C
//		U <= min(n-1+(S/n),2*fsqrt(S+1)-2)
//											(S >= n*(U+1)-n^2)
//		U <= 2*fsqrt(S+1-C)-2+C
//		worst case
//		U <= 2*fsqrt(S+1)-2
//(*)
//	S>= U+floor((n-1)/2)*((V/floor((n+1)/2))-2)<U+V
//		U <= (S+2*floor((n-1)/2))/(1+floor((n-1)/2)/floor((n+1)/2))
//		U <= ((S-C)+2*floor((n-1)/2))/(1+floor((n-1)/2)/floor((n+1)/2))+C
//		U <= 0.5*(S-C)+C=0.5*(S+C)
//		U <= 0.5*S

//C-S=O(-(U-C)*(2+ln(ln(U-C)/ln(2))/ln(2)))
//O((1+ln(ln(U-C)/ln(2))/ln(2)))
//O(((m-n)/m)*(ln(2*(ln(U-C)-ln(m))/(ln(U-C)-ln(n)))/ln(2)))
//(2^n)^(b-2)
//lnn*2^(n/2-n/b)-(lnn+ln(b-2))=lnn*(2^(n/2-n/b)-1)-ln(b-2)=lnn+lnn*(2^(n/2-n/b)-2)-ln(b-2)
//lnn>ln(b-2)/(2^(n/2-n/b)-1)
//lnn/ln(b-2)>1/(2^(n/2-n/b)-1)

#ifndef min
#define min(x,y) ((x)<(y)?(x):(y))
#endif

#ifndef max
#define max(x,y) ((x)>(y)?(x):(y))
#endif

#ifndef _MT
#define STH
#endif

#ifndef STH
#define THEMAPHORE(x) Themaphore x;
#define ENTER(x)	(x).enter();
#define LEAVE(x)	(x).leave();
#define WAIT(x)		(x).wait();
#else 
#define THEMAPHORE(x)
#define ENTER(x)
#define LEAVE(x)
#define WAIT(x)
#endif 

#ifdef WIN32
//#define INLINE	inline
#define INLINE	__forceinline
#else
#define INLINE	inline
#endif

#define FIRSTFIT
#define FIRSTFIT_WEAK
	
	// if PREFIXTREE is defined root node will be left node
#define PREFIXTREE

	// FIRST_IF_NOT_BEST for FIRSTFIT enabled
//#define FIRST_IF_NOT_BEST

	// when RECURSIVE_234 is defined recursive list tree algorithms used
//#define RECURSIVE_1

	// when RECURSIVE_234 is defined recursive balanced 2 3 4 tree algorithms used
//#define RECURSIVE_234

	// when RECURSIVE_AVL is defined recursive AVL tree algorithms used
//#define RECURSIVE_AVL

	// if LIST_TREE is NOT defined simple lists will be used for one-cell blocks
#ifndef FREE_EDITION
#define LIST_TREE
#endif

	// if FULL_QUARTER is NOT first four memory cells will be unavailable
#define FULL_QUARTER

	// if MAX_SHIFT is defined size of parts of rbri value will be maximal as it is possible
	// else it will be 2
	// in both cases the volume of list tree nodes will be computed closely to need_size/shift
	// where need_size is number of bits required to handle all indexes of quarter of memory
#define MAX_SHIFT

	// if AVL_FIND_INDIFFERENT_INDEX is defined findPlace returns any matching block not block with minimal index
#define AVL_FIND_INDIFFERENT_INDEX

	// if AVL_OPTIMIZATION is defined nodes in AVL tree will be resized when it is possible 
#define AVL_OPTIMIZATION

	// if EXTRACT_FIRST_TERMINAL is defined at first will be extracted first found terminal node from balanced 2 3 4 trees otherwise root node
//#define EXTRACT_FIRST_TERMINAL_234

	// if EXTRACT_FIRST_TERMINAL is define, at first will be extracted first found terminal node from list tree otherwise root node
//#define EXTRACT_FIRST_TERMINAL_1

#ifndef FREE_EDITION
//#define LIST_TREE_FILTER
#define LIST_TREE_DOUBLE_EXTRACT
#endif

#ifndef STATIC_ROOTS
	// defines if root indexes can be changed dynamically
#define DYNAMIC_ROOTS
#endif

	// quantity of indexes
#define ROOT_QUANTITY	8

#define VOLUME_ZERO		TVOLUME(UNIT_INDEX-UNIT_INDEX)
#define VOLUME_ONE		TVOLUME(UNIT_INDEX+UNIT_INDEX-UNIT_INDEX)
#define VOLUME_TWO		TVOLUME(UNIT_INDEX+UNIT_INDEX+UNIT_INDEX-UNIT_INDEX)
#define VOLUME_THREE	TVOLUME(UNIT_INDEX+UNIT_INDEX+UNIT_INDEX+UNIT_INDEX-UNIT_INDEX)
#define VOLUME_FOUR		TVOLUME(UNIT_INDEX+UNIT_INDEX+UNIT_INDEX+UNIT_INDEX+UNIT_INDEX-UNIT_INDEX)
#define VOLUME_FIVE		TVOLUME(UNIT_INDEX+UNIT_INDEX+UNIT_INDEX+UNIT_INDEX+UNIT_INDEX+UNIT_INDEX-UNIT_INDEX)

//#define _BUG_INFO
//#define _PROFILE
//#define _HIRESTIME
#include <time.h>
extern double volatile 	atime1,
						atime2,
						atime3,
						atime4,
						atime5,
						atime6,
						atime7,
						atime8,
						atime9,
						dtime1,
						dtime2,
						dtime3,
						dtime4,
						dtime5,
						dtime6,
						dtime7,
						dtime8,
						tb,tc;
//void __rdtsc() { __int64 x;
//				asm volatile("rdtsc" : "=A" (x));
//				x; };
#ifdef CLOCK
#undef CLOCK
#endif
#ifdef _PROFILE
#ifdef _HIRESTIME
#define CLOCK			rdtsc
#else
#define CLOCK			clock
#endif
#define SET(b,p)		((b)=(p))
#define WATCH(b)		SET((b),(double)CLOCK())
#define COUNT(b,p,c)	(((p) += (c)-(b)),SET((b),(c)))
#define MATCH(b,p)		{\
							double t;\
							WATCH(t);\
							COUNT((b),(p),t);\
						}
#else
#define CLOCK			clock
#define SET(b,p)
#define WATCH(b)
#define MATCH(b,p)
#define COUNT(b,p,c)
#endif

#include <cstdlib>//std::exit
#include <iostream>

#ifdef SLOW_EDITION
#include "nodes_memory_slow.h"
#else
#include "nodes_memory.h"
#endif

#include "balanced_tree.h"
#include "balanced_cartesian_tree.h"
#ifndef FIRSTFIT
#include "avl_tree.h"
#endif

#ifdef LIST_TREE
#include "list_tree.h"
#else
#include "list.h"
#endif

#include "unilist.h"	

//#define LOG_ALLOCATION
#ifdef LOG_ALLOCATION
static unsigned counter = 0;
#define MAX_COUNTER	1000
#define dump_stream(str, v, p, s) std::cout << (s) << ":" << (str) << ":" << (v) << " " << (p) << std::endl;
#else
#define dump_stream(str, v, p, s)
#endif


template <	typename TMEMORY,	//memory of cells
			typename TVOLUME,	//volume type
			TVOLUME VOLUME,	// memory volume
			typename TCELL = TVOLUME,	//cell type
			typename TINDEX = TVOLUME,	//index type
			typename TPOINTER = TMEMORY,	//pointer type
			TINDEX NULL_INDEX = TINDEX(VOLUME-VOLUME),	//null pointer
			TINDEX UNIT_INDEX = TINDEX(1),	//index unit
			TVOLUME BASE_VOLUME = VOLUME, //start volume
			unsigned RATIO = 4,	//ratio of used for reducing (RATIO > SCALE)
			unsigned SCALE = 2,	//scale for reallocating
			void* REALLOCATOR = (void*)NULL,
			TVOLUME START_VOLUME = BASE_VOLUME,
			TINDEX LIST_1_INDEX = TINDEX(NULL_INDEX),				//list of free cells for first quarter of memory	(perhaps it can be improved splitting into tree at special cases)
			TINDEX LIST_2_INDEX = TINDEX(LIST_1_INDEX+UNIT_INDEX),	//list of free cells for second quarter of memory (perhaps it can be improved splitting into tree at special cases)
			TINDEX LIST_3_INDEX = TINDEX(LIST_2_INDEX+UNIT_INDEX),	//list of free cells for third quarter of memory (perhaps it can be improved splitting into tree at special cases)
			TINDEX LIST_4_INDEX = TINDEX(LIST_3_INDEX+UNIT_INDEX),	//list of free cells for fourth quarter of memory (perhaps it can be improved splitting into tree at special cases)
																	//															high	low
																	//	cells:					...		4		3		2		1		0
			TINDEX TREE_2_INDEX = TINDEX(LIST_4_INDEX+UNIT_INDEX),	//index-balanced tree for doublecells 
																	//(every node stores:										[LBRI ## RBRI]) (LBRI > RBRI)||(LBRI == NULL_INDEX)||(RBRI == NULL_INDEX)
			TINDEX TREE_3_INDEX = TINDEX(TREE_2_INDEX+UNIT_INDEX),	//index-balanced tree for triplecells 
																	//(every node stores:								[FCNI ## LBRI ## RBRI]) (LBRI > RBRI)||(LBRI == NULL_INDEX)||(RBRI == NULL_INDEX)
			TINDEX TREE_4_INDEX = TINDEX(TREE_3_INDEX+UNIT_INDEX),	//index-balanced tree for quadrocells 
																	//(every node stores:						[FCNI ## LBRI ## RBRI ## 0004]) (LBRI > FCNI > RBRI)||(LBRI == NULL_INDEX)||(RBRI == NULL_INDEX)
			TINDEX TREE_5_INDEX = TINDEX(TREE_4_INDEX+UNIT_INDEX)	//(volume##index)-balanced AVL-tree for multicells greater than quadrocells 
																	//(every node stores:			[FCNI ... ## BRHD ## LBRI ## RBRI ## NVLM]) (LBRI <> RBRI)||(LBRI == NULL_INDEX)||(RBRI == NULL_INDEX)
																	//	cells:					...		4		3		2		1		0
																	//															high	low
																	//RBRI - right (lower) branch index (equals NULL_INDEX in case of absence)
																	//LBRI - left (higher) branch index (equals NULL_INDEX in case of absence)
																	//FCNI - index of first cell of node ()
																	//NVLM - AVL-tree node volume
																	//BRHD - difference of heights of branches for AVL-tree
> class memory_manager {
	THEMAPHORE(t5)
	THEMAPHORE(s4)
	THEMAPHORE(d)
	TMEMORY mem;
	//TMEMORY mem; // use next:
	//	TCELL& TMEMORY::operator[] (TINDEX index)
	//	TCELL& TCELL::operator= (INDEX index)
	//	TCELL::operator TINDEX()
	BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX> memory;

#ifdef LIST_TREE
	ListTree<TMEMORY, TVOLUME, VOLUME, TINDEX, LIST_1_INDEX, 
#ifdef FULL_QUARTER
		VOLUME_ZERO,
		VOLUME/VOLUME_FOUR,
#else
		VOLUME_FOUR, 
		VOLUME/VOLUME_FOUR+VOLUME_THREE,
#endif
		TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> list1;
	ListTree<TMEMORY, TVOLUME, VOLUME, TINDEX, LIST_2_INDEX, 
#ifdef FULL_QUARTER
		VOLUME/VOLUME_FOUR,
		VOLUME/VOLUME_TWO,
#else
		VOLUME/VOLUME_FOUR+VOLUME_THREE, 
		VOLUME/VOLUME_TWO+VOLUME_TWO,
#endif
		TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> list2;
	ListTree<TMEMORY, TVOLUME, VOLUME, TINDEX, LIST_3_INDEX, 
#ifdef FULL_QUARTER
		VOLUME/VOLUME_TWO,
		VOLUME_THREE*(VOLUME/VOLUME_FOUR),
#else
		VOLUME/VOLUME_TWO+VOLUME_TWO, 
		VOLUME_THREE*(VOLUME/VOLUME_FOUR)+VOLUME_ONE, 
#endif
		TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> list3;
	ListTree<TMEMORY, TVOLUME, VOLUME, TINDEX, LIST_4_INDEX, 
		VOLUME_THREE*(VOLUME/VOLUME_FOUR)
#ifndef FULL_QUARTER
			+VOLUME_ONE
#endif
		,
		VOLUME, 
		TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> list4;
#else
	List<TMEMORY, TVOLUME, VOLUME, TINDEX, LIST_1_INDEX, TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> list1; 
	List<TMEMORY, TVOLUME, VOLUME, TINDEX, LIST_2_INDEX, TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> list2; 
	List<TMEMORY, TVOLUME, VOLUME, TINDEX, LIST_3_INDEX, TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> list3; 
	List<TMEMORY, TVOLUME, VOLUME, TINDEX, LIST_4_INDEX, TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> list4;
#endif
public:
	typedef TVOLUME(*TCALLBACK)(const TMEMORY, TINDEX, TINDEX, TVOLUME);
	typedef bool (*TREALLOCATOR)(TMEMORY*, TVOLUME, TVOLUME);
private:
	UniList<TMEMORY, TVOLUME, VOLUME, TINDEX, LIST_1_INDEX, TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX, TCALLBACK> list; 

	BalancedTree<TMEMORY, TVOLUME, VOLUME, 2, TINDEX, TREE_2_INDEX, TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> tree2; 
	BalancedTree<TMEMORY, TVOLUME, VOLUME, 3, TINDEX, TREE_3_INDEX, TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> tree3; 
	BalancedTree<TMEMORY, TVOLUME, VOLUME, 4, TINDEX, TREE_4_INDEX, TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> tree4;
#ifndef FIRSTFIT
	AVLTree<TMEMORY, TVOLUME, VOLUME, TINDEX, TREE_5_INDEX, TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> tree5;
#else
	BalancedCartesianTree<TMEMORY, TVOLUME, VOLUME, 5, TINDEX, TREE_5_INDEX, TCELL, TPOINTER, NULL_INDEX, UNIT_INDEX> tree5;
#endif
	

	TINDEX	lastIndex;
	TVOLUME	_actualVolume;

public:
#ifdef  BASE_CHECK
	TCELL state;
#endif


	memory_manager() : //mem(&m),
		mem(),_actualVolume(START_VOLUME),
		memory(&mem,&(_actualVolume = START_VOLUME)),
#ifdef BASE_CHECK
		state(OK_STATE),
#endif
		list1(&mem,&_actualVolume),list2(&mem,&_actualVolume),list3(&mem,&_actualVolume),list4(&mem,&_actualVolume),list(&mem,&_actualVolume),
		tree2(&mem,&_actualVolume),tree3(&mem,&_actualVolume),tree4(&mem,&_actualVolume),tree5(&mem,&_actualVolume)	{
//
//		std::cout <<"memory_manager:enter"<< std::endl;///!
//		std::cout <<"memory_manager:mem "<< &mem << std::endl;///!
//		std::cout <<"memory_manager:actualVolume "<< _actualVolume << std::endl;///!
		unsigned i = ROOT_QUANTITY;
		TINDEX p = NULL_INDEX, roots[] = { 
					LIST_1_INDEX,
					LIST_2_INDEX,
					LIST_3_INDEX,
					LIST_4_INDEX,
					TREE_2_INDEX,
					TREE_3_INDEX,
					TREE_4_INDEX,
					TREE_5_INDEX
		};
		ENTER(t5);
//		for (i -= i; i < ROOT_QUANTITY; i++)
//			memory.cell(roots[i]) = NULL_INDEX;
		for (i -= i; i < ROOT_QUANTITY; i++)
			for (unsigned j = i; j < ROOT_QUANTITY; j++) 
				if (roots[i] > roots[j]) {
					p = roots[i];
					roots[i] = roots[j];
					roots[j] = p;
				}
		lastIndex = roots[--i];
		p -= TVOLUME(p);

#ifdef	SLOW_ALLOCATOR
		memory.cell(list2.root()) = list2.root();
#endif

		for (i -= i; i < ROOT_QUANTITY; i++) {
			freeAdd(p,roots[i]-p);
			p = roots[i]+UNIT_INDEX;
		}
		freeAdd(p,actualVolume()-p);
//		dump(0,50);///!
//		std::cout <<"memory_manager:"<< memory.cell(8)<< std::endl;///!
//		std::cout <<"memory_manager:leave"<< std::endl;///!
//
//#ifdef DYNAMIC_ROOTS
//		tree5.add(tree5.root()+UNIT_INDEX,VOLUME-TVOLUME(tree5.root())-VOLUME_ONE);
//#else
//		tree5.add(TREE_5_INDEX+UNIT_INDEX,VOLUME-TVOLUME(TREE_5_INDEX)-VOLUME_ONE);
//#endif
		LEAVE(t5);		
	}
	memory_manager(TMEMORY m) : //mem(&m), 
		mem(m),_actualVolume(START_VOLUME),
		memory(&mem,&(_actualVolume = START_VOLUME)),
		list1(&mem,&_actualVolume),list2(&mem,&_actualVolume),list3(&mem,&_actualVolume),list4(&mem,&_actualVolume),list(&mem,&_actualVolume),
		tree2(&mem,&_actualVolume),tree3(&mem,&_actualVolume),tree4(&mem,&_actualVolume),tree5(&mem,&_actualVolume) {
		unsigned i = ROOT_QUANTITY;
		TINDEX p = NULL_INDEX, roots[] = { 
					LIST_1_INDEX,
					LIST_2_INDEX,
					LIST_3_INDEX,
					LIST_4_INDEX,
					TREE_2_INDEX,
					TREE_3_INDEX,
					TREE_4_INDEX,
					TREE_5_INDEX
		};
		ENTER(t5);
//		for (i -= i; i < ROOT_QUANTITY; i++)
//			memory.cell(roots[i]) = NULL_INDEX;
		for (i -= i; i < ROOT_QUANTITY; i++)
			for (unsigned j = i; j < ROOT_QUANTITY; j++) 
				if (roots[i] > roots[j]) {
					p = roots[i];
					roots[i] = roots[j];
					roots[j] = p;
				}
		lastIndex = roots[--i];
		p -= TVOLUME(p);

#ifdef	SLOW_ALLOCATOR
		memory.cell(list2.root()) = list2.root();
#endif

		for (i -= i; i < ROOT_QUANTITY; i++) {
			freeAdd(p,roots[i]-p);
			p = roots[i]+UNIT_INDEX;
		}
		freeAdd(p,actualVolume()-p);

//#ifdef DYNAMIC_ROOTS
//		tree5.add(tree5.root()+UNIT_INDEX,VOLUME-TVOLUME(tree5.root())-VOLUME_ONE);
//#else
//		tree5.add(TREE_5_INDEX+UNIT_INDEX,VOLUME-TVOLUME(TREE_5_INDEX)-VOLUME_ONE);
//#endif
		LEAVE(t5);
	}

	operator TMEMORY&() {
		return mem;
	}

	TMEMORY& m() {
		return *this;
	}

	bool setRoots (TINDEX list_1_index, TINDEX list_2_index, TINDEX list_3_index, TINDEX list_4_index, TINDEX tree_2_index, TINDEX tree_3_index, TINDEX tree_4_index, TINDEX tree_5_index) {
#ifdef DYNAMIC_ROOTS
//#ifdef SLOW_ALLOCATOR
		list.root(list_1_index);
//#else
		list1.root(list_1_index);
		list2.root(list_2_index);
		list3.root(list_3_index);
		list4.root(list_4_index);
		tree2.root(tree_2_index);
		tree3.root(tree_3_index);
		tree4.root(tree_4_index);
		tree5.root(tree_5_index);
//#endif
		convertAllocatorMode();
		return true;
#else
		return false;
#endif
	}

	TINDEX allocate(TVOLUME volume, TINDEX pointer = NULL_INDEX) {  //returns index of allocated multicell
#ifdef SLOW_ALLOCATOR
		ENTER(s4);
		pointer = list.allocateFirst(volume,pointer);
		LEAVE(s4);
#endif
#ifdef	LOG_ALLOCATION
		if (counter++ > MAX_COUNTER) {
			dump_stream("allocation",volume,pointer,list.state);
			counter = 0;
		}
#endif
		return
#ifdef SLOW_ALLOCATOR
				pointer;
#else
#ifdef FIRSTFIT
				allocateFirst(volume,pointer);
#else
				allocateFirstBest(volume,pointer);
#endif
#endif
	}
#ifndef SLOW_EDITION
#include "memory_manager1.inc"
#endif //SLOW_EDITION
	TVOLUME volume(TINDEX index) {                                  //returns volume of free multicell at the index ????
		return VOLUME-VOLUME;
	}; 

	TVOLUME deallocate(TVOLUME volume, TINDEX pointer) {            //returns volume of deallocated multicell
#ifdef	LOG_ALLOCATION
		if (counter++ > MAX_COUNTER) {
			dump_stream("deallocation",volume,pointer,list.state);
			counter = 0;
		}
#endif
#ifdef SLOW_ALLOCATOR
		ENTER(s4);
		volume = list.tryDeallocate(volume,pointer);
		LEAVE(s4);
		return volume;
#else//ndef SLOW_EDITION
#include "memory_manager2.inc"
#endif//SLOW_EDITION
	};
#ifndef SLOW_EDITION
#include "memory_manager3.inc"
#endif
	INLINE TINDEX coallocate(TVOLUME volume, TINDEX pointer) {
#ifdef	SLOW_ALLOCATOR
		TINDEX index = pointer;
		return index = NULL_INDEX;
#else

		TVOLUME vol = VOLUME_ZERO;
		TINDEX index = pointer;

		ENTER(t5);		
		if (index != NULL_INDEX) if ((index = tree5.filterFirst(pointer,vol)) != NULL_INDEX) {
			if (vol >= volume) {
				index = tree5.extract(pointer,vol);
				if (vol > volume) freeAddUnsafe(index+volume,vol-volume);
			}
			else index = NULL_INDEX;
		}

		LEAVE(t5);
		return index;
#endif
	};

	static TVOLUME dflt(const TMEMORY memory, TINDEX d, TINDEX s, TVOLUME c) {
       	if (s > d) while (c-- > 0) memory[d++] = memory[s++];
        else while (c-- > 0) memory[d+c] = memory[s+c];
		return c;
	};
	INLINE TINDEX reallocateFast(TVOLUME volume, TVOLUME ovolume, TINDEX index, const TCALLBACK p = dflt) {
#ifdef SLOW_ALLOCATOR
		return list.reallocateFirst(volume,index,ovolume,p);
#else
		if (volume > ovolume) {
			if (coallocate(index+ovolume,volume-ovolume) == NULL_INDEX) {
				if (p != NULL) {
					TINDEX d = allocate(volume);
					if (d != NULL_INDEX) {
						p(*(memory.memory),d,index,ovolume);
						if (deallocate(ovolume,index) != ovolume) {
#ifdef BASE_CHECK
							state |= (INCONSISTENT|OUTOFMEMORY);
#endif
							index = NULL_INDEX;
						}
						index = d;
					}
#ifdef BASE_CHECK
					else {
						state |= OUTOFMEMORY;
					}
#endif
				} else {
					if (deallocate(ovolume,index) != ovolume) {
#ifdef BASE_CHECK
						state |= (INCONSISTENT|OUTOFMEMORY);
#endif
						index = NULL_INDEX;
					}
#ifdef BASE_CHECK
					else if ((index = allocate(volume)) != NULL_INDEX) {
						state |= (INCONSISTENT|OUTOFMEMORY);
					}
#endif
				}
			}
		}
		else if (volume < ovolume)
			if (deallocate(ovolume-volume,index+(ovolume-volume)) != (ovolume-volume)) {
#ifdef BASE_CHECK
				state |= (INCONSISTENT|OUTOFMEMORY);
#endif
				index = NULL_INDEX;
			}
		return index;
#endif
	};

	INLINE TINDEX reallocate(TVOLUME width, TVOLUME volume, TINDEX index, const TCALLBACK p = dflt) {
#ifdef SLOW_ALLOCATOR
		return list.reallocateFirst(width,volume,index,p);
#else
		TINDEX past = index+volume;
		TINDEX address = allocateFirst(width);

		if (address == past) deallocateUnsafe(volume,(address = index)+width);
		else if ((address == NULL_INDEX) || (address > past)) {
			TINDEX pre = index-VOLUME_ONE;
			TINDEX checked, pointer = address;
			TVOLUME vol = VOLUME_ZERO;

			// before
			address = NULL_INDEX;
			checked = NULL_INDEX;
			if(pre!=NULL_INDEX&&memory.isIndex(pre)){
				TINDEX fcni = memory.cell(pre);

				if(index-fcni>VOLUME_THREE){
					if((checked = tree5.filterLast(pre,vol))!=NULL_INDEX){
						address = tree5.extract(checked,vol);
					}
					if(address==NULL_INDEX&&(checked = tree4.filterLast(pre,vol))!=NULL_INDEX){
						address = tree4.extract(checked);
					}
				}
				if(address==NULL_INDEX&&(index-fcni == VOLUME_THREE)&&(checked = tree3.filterLast(pre,vol))!=NULL_INDEX){
					address = tree3.extract(checked);

				}
			}
			if(address==NULL_INDEX&&(checked = tree2.filterLast(pre,vol))!=NULL_INDEX){
				address = tree2.extract(checked);
			}
			if(address==NULL_INDEX){
				address = listExtract(pre);
				vol = VOLUME_ONE;
			}
			if(address!=NULL_INDEX){
				volume = volume + vol;
			}
			pre = address;

			// past
			address = NULL_INDEX;
			checked = NULL_INDEX;
			vol = VOLUME_ZERO;
			if(past!=NULL_INDEX&&memory.isIndex(past)){
				if(address==NULL_INDEX&&(checked = tree5.filterFirst(past,vol))!=NULL_INDEX){
					address = tree5.extract(checked,vol);
				}
				if(address==NULL_INDEX&&(checked = tree4.filterFirst(past,vol))!=NULL_INDEX){
					address = tree4.extract(checked);

				}
				if(address==NULL_INDEX&&(checked = tree3.filterFirst(past,vol))!=NULL_INDEX){
					address = tree3.extract(checked);

				}
				if(address==NULL_INDEX&&(checked = tree2.filterFirst(past,vol))!=NULL_INDEX){
					address = tree2.extract(checked);

				}
				if(address==NULL_INDEX){
					address = listExtract(past);
					vol = VOLUME_ONE;
				}
				if(address!=NULL_INDEX){
					volume = volume + vol;
				}
			}
			if (volume < width) {
				if (pointer != NULL_INDEX) {
					past = past - index;
					p(*(memory.memory),address = pointer,index,(past<width)?past:width);
					deallocateUnsafe(volume,(pre == NULL_INDEX)? index: pre);
				}
				else address = NULL_INDEX;
			}
			else {
				if (pointer != NULL_INDEX) deallocateUnsafe(width,pointer);
				if (pre == NULL_INDEX) address = index;
				else {
					past = past - index;
					p(*(memory.memory),address = pre,index,(past<width)?past:width);
				}
				deallocateUnsafe(volume-width,address+width);
			}
		}
		else {
			p(*(memory.memory),address,index,(volume<width)?volume:width);
			deallocateUnsafe(volume,index);
		}
		// TINDEX address = allocateFirst(width);
		// if (address != NULL_INDEX) {
		// 	copy(address,index,(volume<width)?volume:width);
		// 	deallocateUnsafe(volume,index);
		// }
		return address;
#endif
	};

	TPOINTER indexToPointer(TINDEX index) {
		return TPOINTER(&(m()[index]));
//		return memory.indexToPointer(index);
	};

	TINDEX pointerToIndex(TPOINTER pointer) {
		return TINDEX(pointer-TPOINTER(m()));
//		return memory.pointerToIndex(pointer);
	};

	INLINE TVOLUME& actualVolume(TVOLUME& v) {
		return _actualVolume = v;
	};

	INLINE TVOLUME& actualVolume() {
		return _actualVolume;
	};

//	void clear () {t5.clear(); s4.clear(); d.clear();}
	~memory_manager() {};

	//private:									// commented for testing purposes
#ifndef SLOW_EDITION
#include "memory_manager4.inc"
#endif
	// adds free node to list or tree depending on its size
	INLINE void freeAdd(TINDEX index, TVOLUME nodeVolume){
#ifdef SLOW_ALLOCATOR
		ENTER(s4);
		list.tryDeallocate(nodeVolume,index);
		LEAVE(s4);
#else
		if(nodeVolume == VOLUME_ONE){
			ENTER(s4);
			listAdd(index);
			LEAVE(s4);
		}else if(nodeVolume == VOLUME_TWO){
			ENTER(s4);
			tree2.add(index);
			LEAVE(s4);
		}else if(nodeVolume == VOLUME_THREE){
			ENTER(s4);
			tree3.add(index);
			LEAVE(s4);
		}else if(nodeVolume == VOLUME_FOUR){
			ENTER(s4);
			tree4.add(index);
			LEAVE(s4);
		}else if(nodeVolume > VOLUME_FOUR){
			tree5.add(index,nodeVolume);
		}else{			// nodeVolume < 1
			// error
		}
#endif
	}

	INLINE void freeAddUnsafe(TINDEX index, TVOLUME nodeVolume){
//		std::cout << "freeAddUnsafe starts" << std::endl;
#ifdef SLOW_ALLOCATOR
		list.tryDeallocate(nodeVolume,index);
#else
		if(nodeVolume == VOLUME_ONE){
			listAdd(index);
		}else if(nodeVolume == VOLUME_TWO){
			tree2.add(index);
		}else if(nodeVolume == VOLUME_THREE){
			tree3.add(index);
		}else if(nodeVolume == VOLUME_FOUR){
			tree4.add(index);
		}else if(nodeVolume > VOLUME_FOUR){
			tree5.add(index,nodeVolume);
		}else{			// nodeVolume < 1
			// error
		}
#endif
	}
#ifndef SLOW_EDITION
#include "memory_manager5.inc"
#endif

	//-------------------------------------------------------------------------
	//  testing methods     !!! test methods, might be deleted !!!
	//-------------------------------------------------------------------------

	// prints memory content                                
	void print(TINDEX from = NULL_INDEX, TINDEX to = VOLUME){
		std::cout<<"---------------------\n";
		std::cout<<"memory: ["<<from<<"] - ["<<to-TINDEX(VOLUME_ONE)<<"]\n";
		std::cout<<"---------------------\n";
		for (TINDEX i=from;i<to;i++){
			std::cout<<"["<<i<<"]\t"<<memory.cell(i);
#ifdef DYNAMIC_ROOTS
			if (i == list1.root())
				std::cout<<"\t LIST_1_INDEX";
			else if (i == list2.root())
					std::cout<<"\t LIST_2_INDEX";
			else if (i == list3.root())
					std::cout<<"\t LIST_3_INDEX";
			else if (i == list4.root())
					std::cout<<"\t LIST_4_INDEX";
			else if (i == tree2.root())
					std::cout<<"\t TREE_2_INDEX";
			else if (i == tree3.root())
					std::cout<<"\t TREE_3_INDEX";
			else if (i == tree4.root())
					std::cout<<"\t TREE_4_INDEX";
			else if (i == tree5.root())
					std::cout<<"\t TREE_5_INDEX";
#else
			if (i == LIST_1_INDEX)
				std::cout<<"\t LIST_1_INDEX";
			else if (i == LIST_2_INDEX)
					std::cout<<"\t LIST_2_INDEX";
			else if (i == LIST_3_INDEX)
					std::cout<<"\t LIST_3_INDEX";
			else if (i == LIST_4_INDEX)
					std::cout<<"\t LIST_4_INDEX";
			else if (i == TREE_2_INDEX)
					std::cout<<"\t TREE_2_INDEX";
			else if (i == TREE_3_INDEX)
					std::cout<<"\t TREE_3_INDEX";
			else if (i == TREE_4_INDEX)
					std::cout<<"\t TREE_4_INDEX";
			else if (i == TREE_5_INDEX)
					std::cout<<"\t TREE_5_INDEX";
#endif
			std::cout<<"\n";
		}
		std::cout<<"---------------------\n";
	};

	// prints all free memory structures
	void printFreeMemory(){
		std::cout<<"----------------------------------\n";
		std::cout<<"----------- FREE MEMORY ----------\n";
		std::cout<<"----------------------------------\n";
#ifdef SLOW_ALLOCATOR
		TVOLUME cells = VOLUME_ZERO, nodes = list.print(cells);
#else
//		unsigned c = 0;
		TVOLUME cells = VOLUME_ZERO;
		TVOLUME nodes = VOLUME_ZERO;
		nodes+=list1.print();
		nodes+=list2.print();
		nodes+=list3.print();
		nodes+=list4.print();
		std::cout<<"----total lists: "<<nodes<<"\t----------\n";
		cells=nodes;
		nodes+=tree2.print(cells);
		nodes+=tree3.print(cells);
		nodes+=tree4.print(cells);
		nodes+=tree5.print(cells);
#endif
		std::cout<<"----total nodes: "<<nodes<<"\t----------\n";
		std::cout<<"----total cells: "<<cells<<"\t----------\n";
		std::cout<<"----------------------------------\n";
	}

	void dump(TINDEX start = NULL_INDEX, TINDEX final = NULL_INDEX+actualVolume(), TVOLUME offset = VOLUME_ZERO) {
		ENTER(d);
		std::cout << "list1:" << list1.root() << std::endl;///!
		std::cout << "tree5:" << tree5.root() << std::endl;///!
		std::cout << "memory:" << &(memory.cell(0)) << std::endl;///!
		if (offset != VOLUME_ZERO)
		for (TINDEX i = start; i < final; i++) {
//			std::cout << mem[i] << std::endl;
			std::cout << *(((TVOLUME*)&(memory.cell(i)))-offset) << ((i == offset+list1.root())?"list1":"") << ((i == offset+tree5.root())?"tree5":"") << std::endl;
		}
		else for (TINDEX i = start; i < final; i++) {
//			std::cout << mem[i] << std::endl;
			std::cout << memory.cell(i) << ((i == list1.root())?"list1":"") << ((i == tree5.root())?"tree5":"") << std::endl;
		}
		std::cout << "----------------------------------------------" << std::endl;
		LEAVE(d);
	}
	TINDEX inconsistent() {
#ifdef SLOW_ALLOCATOR
		return NULL_INDEX;
#else
		return tree5.inconsistent();
#endif
	}
	TVOLUME maxAvailable(const TVOLUME v = VOLUME_ZERO) {
#ifdef SLOW_ALLOCATOR
		ENTER(s4);
		TVOLUME volume = list.maxAvailable(v);
		LEAVE(s4);
		return volume;
#else
		if (memory.cell(tree5.root()) != NULL_INDEX) {
			return (v > memory.cell(memory.cell(tree5.root()),TINDEX(VOLUME_THREE)))? VOLUME_ZERO: memory.cell(memory.cell(tree5.root()),TINDEX(VOLUME_THREE));
		}
		else {
			if (v > VOLUME_FOUR) return VOLUME_ZERO;
			if (memory.cell(tree4.root()) != NULL_INDEX) return VOLUME_FOUR;
			if (v > VOLUME_THREE) return VOLUME_ZERO;
			if (memory.cell(tree3.root()) != NULL_INDEX) return VOLUME_THREE;
			if (v > VOLUME_TWO) return VOLUME_ZERO;
			if (memory.cell(tree2.root()) != NULL_INDEX) return VOLUME_TWO;
			if (v > VOLUME_ONE) return VOLUME_ZERO;
			if (memory.cell(list1.root()) != NULL_INDEX) return VOLUME_ONE;
			if (memory.cell(list2.root()) != NULL_INDEX) return VOLUME_ONE;
			if (memory.cell(list3.root()) != NULL_INDEX) return VOLUME_ONE;
			if (memory.cell(list4.root()) != NULL_INDEX) return VOLUME_ONE;
		}
		return VOLUME_ZERO;
#endif
	}
	bool slowAllocatorMode() {
		return (memory.cell(list2.root())==list2.root());
	}
	bool slowAllocator() {
		return
#ifdef SLOW_ALLOCATOR
			true;
#else
			false;
#endif
	}
	bool convertAllocatorMode() {
//		std::cout << "try converting" << std::endl;
		ENTER(t5);
		ENTER(s4);
		unsigned c = 0;
		if (slowAllocator()) {
			if (!slowAllocatorMode()) {
#ifdef SLOW_EDITION
				std::cout << "can't deconvert" << std::endl;
				std::exit(0);
//				halt(0);
#else
#include "memory_manager6.inc"
#endif
			}
		}
#ifndef SLOW_EDITION
#include "memory_manager7.inc"
#endif
		LEAVE(s4);
		LEAVE(t5);
//		std::cout << "end converting" << std::endl;
		return true;
	}
};