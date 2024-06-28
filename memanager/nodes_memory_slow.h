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

#ifndef RECURSIVE_AVL
#define BRH_LOW		VOLUME_ZERO
#define BRH_LESS	VOLUME_ONE
#define BRH_EQUAL	VOLUME_TWO
#define BRH_GREATER	VOLUME_THREE
#define BRH_HIGH	VOLUME_FOUR
#endif

#define NODE_FILTER
//#define OUT_OF_MEMORY_CHECK
#ifndef FREE_EDITION
#define BASE_CHECK
#define LOOP_CHECK
#define SIDE_CHECK
#endif

#define OK_STATE        0
#define OUTOFMEMORY     1
#define LOOP            2
#define ADDRESS         4
#define INCONSISTENT    8

template <typename TMEMORY, 
          typename TVOLUME, 
          TVOLUME VOLUME, 
          typename TCELL, 
          typename TINDEX, 
		  typename TPOINTER,
		  TINDEX NULL_INDEX,
		  TINDEX UNIT_INDEX> 
class BaseMemory{
public:
	TVOLUME* pActualVolume;
	TMEMORY* memory; 

#ifdef BASE_CHECK
	TCELL state;
#endif
	BaseMemory(TMEMORY* memory, TVOLUME* volume) : memory(memory), pActualVolume(volume) 
#ifdef BASE_CHECK
	,state(OK_STATE)
#endif
	{	}

	// checks if specified index hits in memory array
	INLINE bool isIndex(const TINDEX index){
#ifdef OUT_OF_MEMORY_CHECK
		return index >= TINDEX(VOLUME_ZERO) && ((pActualVolume == NULL) || index < TINDEX(*pActualVolume));
#else
		return index >= TINDEX(VOLUME_ZERO) && index < TINDEX(*pActualVolume);
//		return index >= TINDEX(VOLUME_ZERO) && index < TINDEX(VOLUME);
#endif
	}

	// returns memory cell on specified index+offset
	// before return result, checks index
	INLINE TCELL& cell(const TINDEX index, const TINDEX offset = TINDEX(VOLUME_ZERO)){
#ifdef OUT_OF_MEMORY_CHECK
		if(!isIndex(index+offset)){
			std::cout<<"[ERROR!5] Out of memory error: index="<<index
				<<" offset="<<offset<<"\n";
//			abort();
			// also might be in this case we need 
			// to return something other like NULL
#ifdef BASE_CHECK
			state |= (INCONSISTENT|ADDRESS);
#endif
			return VOLUME_ZERO;
		}
#endif
		/*		if ((*memory)[1] != NULL_INDEX && ((*memory)[1] < 33554444)) {
			printf("!!!\n");
			getch();
		}*/

		return (*memory)[index+offset];
	}

	INLINE TPOINTER indexToPointer(TINDEX index) {
		return TPOINTER(&((*memory)[index]));
	};

	INLINE TINDEX pointerToIndex(TPOINTER pointer) {
		return TINDEX(pointer-TPOINTER(*memory));
	};

};

template <typename TMEMORY, 
          typename TVOLUME, 
		  TVOLUME VOLUME, 
		  int NODE_VOLUME,
          typename TCELL, 
          typename TINDEX, 
          typename TPOINTER,
		  TINDEX NULL_INDEX,
		  TINDEX UNIT_INDEX> 
class NodesMemory;
//#define SLOW_EDITION
template <typename TMEMORY, 
          typename TVOLUME, 
          TVOLUME VOLUME, 
		  typename TCELL, 
          typename TINDEX, 
          typename TPOINTER,
          TINDEX NULL_INDEX,
		  TINDEX UNIT_INDEX> 
class NodesMemory<TMEMORY,TVOLUME,VOLUME,2,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX> : public BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>{
	using BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>::pActualVolume;
public:
	using BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>::cell;
	NodesMemory(TMEMORY* m, TVOLUME* volume): BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>(m,volume) {}
};

template <typename TMEMORY, 
          typename TVOLUME, 
          TVOLUME VOLUME, 
          typename TCELL, 
          typename TINDEX, 
		  typename TPOINTER,
		  TINDEX NULL_INDEX,
		  TINDEX UNIT_INDEX> 
class NodesMemory<TMEMORY,TVOLUME,VOLUME,3,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX> : public BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>{
	using BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>::pActualVolume;
public:
	using BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>::cell;
	NodesMemory(TMEMORY* m, TVOLUME* volume): BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>(m,volume) {}
};

template <typename TMEMORY, 
          typename TVOLUME, 
          TVOLUME VOLUME, 
          typename TCELL, 
          typename TINDEX, 
		  typename TPOINTER,
		  TINDEX NULL_INDEX,
		  TINDEX UNIT_INDEX> 
class NodesMemory<TMEMORY,TVOLUME,VOLUME,4,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX> : public BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>{
	using BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>::pActualVolume;
public:
	using BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>::cell;
	NodesMemory(TMEMORY* m, TVOLUME* volume): BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>(m,volume) {}
};

#ifndef FIRSTFIT
template <typename TMEMORY, 
          typename TVOLUME, 
          TVOLUME VOLUME, 
          typename TCELL, 
          typename TINDEX, 
		  typename TPOINTER,
		  TINDEX NULL_INDEX,
		  TINDEX UNIT_INDEX> 
class NodesMemory<TMEMORY,TVOLUME,VOLUME,5,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX> : public BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>{
	using BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>::pActualVolume;
public:
	using BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>::cell;
	NodesMemory(TMEMORY* m, TVOLUME* volume): BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>(m,volume) {}
};
#else
template <typename TMEMORY, 
          typename TVOLUME, 
          TVOLUME VOLUME, 
          typename TCELL, 
          typename TINDEX, 
		  typename TPOINTER,
		  TINDEX NULL_INDEX,
		  TINDEX UNIT_INDEX> 
class NodesMemory<TMEMORY,TVOLUME,VOLUME,5,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX> : public BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>{
	using BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>::pActualVolume;
public:
	using BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>::cell;
	NodesMemory(TMEMORY* m, TVOLUME* volume): BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX>(m,volume) {}
};
#endif
//#endif