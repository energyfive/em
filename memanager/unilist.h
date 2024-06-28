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



template <typename TMEMORY, 
          typename TVOLUME, 
          TVOLUME VOLUME,
          typename TINDEX, 
          TINDEX LIST_INDEX,
          typename TCELL, 
          typename TPOINTER,
          TINDEX NULL_INDEX,
          TINDEX UNIT_INDEX,
	      typename TCALLBACK> 
class UniList{
protected:
#ifdef DYNAMIC_ROOTS
	TINDEX ROOT_INDEX;
#endif
#ifdef OUT_OF_MEMORY_CHECK
public:
#endif
        BaseMemory<TMEMORY,TVOLUME,VOLUME,TCELL,TINDEX,TPOINTER,NULL_INDEX,UNIT_INDEX> memory;
public:
#ifdef  BASE_CHECK
	TCELL state;
#endif
        UniList(TMEMORY* m, TVOLUME* volume): memory(m,volume){
#ifdef  BASE_CHECK
		state = OK_STATE;
#endif
#ifdef DYNAMIC_ROOTS
		ROOT_INDEX = LIST_INDEX;
#endif
		memory.cell(LIST_INDEX)=LIST_INDEX;
        }
#ifdef DYNAMIC_ROOTS
	INLINE TINDEX root(TINDEX i) {
		return ROOT_INDEX = i;
	}
#endif

	INLINE TINDEX root() {
#ifdef DYNAMIC_ROOTS
		return ROOT_INDEX;
#else
		return LIST_INDEX;
#endif
	}
	TVOLUME maxAvailable(const TVOLUME v = VOLUME_ZERO) {
		TVOLUME volume = VOLUME_ZERO;
		TINDEX start = root(), finish = memory.cell(start)
#ifdef LOOP_CHECK
, passed = start, repeated = passed
#endif
;
		for (;start != finish; finish = memory.cell(start)) {
			if (memory.cell(start = finish) < finish) start = memory.cell(finish);
			if (start >= *(memory.pActualVolume)) {
#ifdef BASE_CHECK
				state |= (INCONSISTENT|ADDRESS);
#endif
				break;
			}
#ifdef BASE_CHECK
			if (passed == start) {
				state |= (INCONSISTENT|LOOP);
				break;
			}
			if (passed == repeated) {
				passed = start;
				repeated = root();
			}
			else if (memory.cell(repeated=memory.cell(repeated)) < repeated) repeated = memory.cell(repeated);
#endif
			if (finish-start+VOLUME_ONE>v) if (finish-start+VOLUME_ONE>volume) volume = finish-start+VOLUME_ONE;
		}
		return volume;
	}
/*
	// adds new element in list
	void add(TINDEX index){
#ifdef DYNAMIC_ROOTS
		TINDEX list = root();
#else
		TINDEX list = LIST_INDEX;
#endif
		while((memory.cell(list)!=NULL_INDEX)&&(memory.cell(list)<=index)){
			list=memory.cell(list);
		}
		memory.cell(index)=memory.cell(list);
		memory.cell(list)=index;
	};

	// returns first node from list
	INLINE TINDEX first(){
#ifdef DYNAMIC_ROOTS
		return memory.cell(root());
#else
		return memory.cell(LIST_INDEX);
#endif
	}

	// find and extract element from list
	TINDEX extract (TINDEX index){
#ifdef DYNAMIC_ROOTS
		TINDEX list = root();
#else
		TINDEX list = LIST_INDEX;
#endif
		while(memory.cell(list)!=NULL_INDEX){
			if(memory.cell(list)==index){
				memory.cell(list)=memory.cell(index);
				return index;
			}
			list=memory.cell(list);
		}
		return NULL_INDEX;
	};

	// extract first element from list
	TINDEX extractFirst (){
#ifdef DYNAMIC_ROOTS
		TINDEX first = memory.cell(root());
#else
		TINDEX first = memory.cell(LIST_INDEX);
#endif
		if(first != NULL_INDEX){
#ifdef DYNAMIC_ROOTS
			memory.cell(root())=memory.cell(first);
#else
			memory.cell(LIST_INDEX)=memory.cell(first);
#endif
		}
		return first;
	};
*/
	TINDEX allocateFirst (TVOLUME volume, TINDEX p = NULL_INDEX){
		TINDEX start = root(), finish = memory.cell(start), pointer = start
#ifdef LOOP_CHECK
, passed = start, repeated = passed
#endif
;
		for (;(start != finish) 
#ifdef BASE_CHECK
|| ((state |= OUTOFMEMORY) == OK_STATE)
#endif
; finish = memory.cell(pointer = start)) {
			if (memory.cell(start=finish) < finish) start = memory.cell(finish);
			if (start >= *(memory.pActualVolume)) {
#ifdef BASE_CHECK
				state |= (INCONSISTENT|ADDRESS);
#endif
				break;
			}
#ifdef LOOP_CHECK
			if (passed == start) {
				state |= (INCONSISTENT|LOOP);
				break;
			}
			if (passed == repeated) {
				passed = start;
				repeated = root();
			}
			else if (memory.cell(repeated=memory.cell(repeated)) < repeated) repeated = memory.cell(repeated);
#endif
			if (finish-start+VOLUME_ONE<volume) continue;
			if (finish-start+VOLUME_ONE>volume) if ((pointer = start+volume) < finish) memory.cell(finish) = pointer;
			if (memory.cell(start) == start) memory.cell(pointer) = pointer;
			else memory.cell(pointer) = memory.cell(start);
//			this.nextIndex = undefined;
			break;
		}
//		return start;
		return ((start == finish) && (volume > VOLUME_ONE))? NULL_INDEX: start;

	};
	TINDEX allocateFirstSafe (TVOLUME volume,TINDEX& update_pointer,TVOLUME& update_value){
		TINDEX start = root(), finish = memory.cell(start), pointer = start
#ifdef LOOP_CHECK
, passed = start, repeated = passed
#endif
;
		for (;(start != finish) 
#ifdef BASE_CHECK
|| ((state |= OUTOFMEMORY) == OK_STATE)
#endif
; finish = memory.cell(pointer = start)) {
			if (memory.cell(start=finish) < finish) start = memory.cell(finish);
			if (start >= *(memory.pActualVolume)) {
#ifdef BASE_CHECK
				state |= (INCONSISTENT|ADDRESS);
#endif
				break;
			}
#ifdef LOOP_CHECK
			if (passed == start) {
				state |= (INCONSISTENT|LOOP);
				break;
			}
			if (passed == repeated) {
				passed = start;
				repeated = root();
			}
			else if (memory.cell(repeated=memory.cell(repeated)) < repeated) repeated = memory.cell(repeated);
#endif
			if (finish-start+VOLUME_ONE<volume) continue;
			if (finish-start+VOLUME_ONE>volume) if ((pointer = start+volume) < finish) memory.cell(finish) = pointer;
//			if (memory.cell(start) == start) memory.cell(pointer) = pointer;
//			else memory.cell(pointer) = memory.cell(start);
			update_pointer = pointer;
			update_value = (memory.cell(start) == start)? pointer: memory.cell(start);
//			this.nextIndex = undefined;
			break;
		}
//		return start;
		return ((start == finish) && (volume > VOLUME_ONE))? NULL_INDEX: start;

	};
	TINDEX reallocateFirst(TVOLUME volume,TINDEX index,TVOLUME width, const TCALLBACK p) {
		TINDEX start = memory.cell(index), finish = memory.cell(index+volume-VOLUME_ONE), address, update_pointer;
		TCELL update_value;
		tryDeallocate(volume,index);
		address = allocateFirstSafe(width,update_pointer,update_value);
		if (address != root()) {
			if (address != index) p(*(memory.memory),address,index,(volume<width)?volume:width);
			if ((volume > VOLUME_ZERO) && (width > VOLUME_ZERO)) memory.cell(address) = start;
			if ((volume > VOLUME_ONE) && (width >= volume)) memory.cell(address+volume-VOLUME_ONE) = finish;
			memory.cell(update_pointer) = update_value;
		}
		else {
			address = allocateFirstSafe(volume,update_pointer,update_value);
			if (address != index) p(*(memory.memory),address,index,volume);
			if (volume > VOLUME_ZERO) memory.cell(address) = start;
			if (volume > VOLUME_ONE) memory.cell(address+volume-VOLUME_ONE) = finish;
			memory.cell(update_pointer) = update_value;
			address = root();
#ifdef BASE_CHECK
			state |= OUTOFMEMORY;
#endif
		}
//    var address = this.allocateFirst(width);
//    if (address != this.rootIndex) {
//      this.copy(address,index,(volume<width)?volume:width);
//      this.tryDeallocate(volume,index);
//    }
	return address;
	};
	void free (TINDEX index, TVOLUME volume, TINDEX address){
		TINDEX start1 = memory.cell(address), finish1 = address, start2, finish2;
		if (start1 > index) start1 = address;
		else if (memory.cell(finish1=start1) < start1) start1 = memory.cell(start1);
		start2 = memory.cell(start1);
		if (memory.cell(finish2=start2) < start2) start2 = memory.cell(start2);
		if (index+volume == start2) {
//			if (this.nextIndex === start2) this.nextIndex = index;
			volume += finish2-start2+VOLUME_ONE;
			start2 = memory.cell(start2);
			if (memory.cell(finish2=start2) < start2) start2 = memory.cell(start2);
		}
		if (finish1+VOLUME_ONE == index) if (finish1 > address) {
//			if (this.nextIndex === index) this.nextIndex = start1;
			index = start1;
			volume += finish1-start1+VOLUME_ONE;
			start1 = address;
		}
		if (index+volume > start2) finish2 = index;
		memory.cell(index+volume-VOLUME_ONE) = index;
		memory.cell(index) = finish2;
		memory.cell(start1) = index+volume-VOLUME_ONE;
	};
	TVOLUME tryDeallocate (TVOLUME volume, TINDEX index){		
		return (volume == VOLUME_ZERO)? VOLUME_ZERO: deallocate (volume, index);
	};
	TVOLUME deallocate (TVOLUME volume, TINDEX index){
		TVOLUME v = VOLUME_ZERO;
		TINDEX start = root(), finish = memory.cell(start), pointer = start, address = pointer
#ifdef LOOP_CHECK
, passed = start, repeated = passed
#endif
;
		for (;start != finish; finish = memory.cell(pointer = start)) {
			if (memory.cell(start=finish) < finish) start = memory.cell(finish);
			if (start >= *(memory.pActualVolume)) {
#ifdef BASE_CHECK
				state |= (INCONSISTENT|ADDRESS);
#endif
				break;
			}
#ifdef LOOP_CHECK
			if (passed == start) {
				state |= (INCONSISTENT|LOOP);
				break;
			}
			if (passed == repeated) {
				passed = start;
				repeated = root();
			}
			else if (memory.cell(repeated=memory.cell(repeated)) < repeated) repeated = memory.cell(repeated);
#endif
			if (index+volume <= start) free(finish = index,v = volume,address);
			if (index <= finish) break;
			address = pointer;
		}
		if (index > finish) free(index,v = volume,address);
		return v;
	};
};
/*
var Manager = function(template) {
  if (template !== undefined) {
    if (template.NULL_INDEX !== undefined) this.NULL_INDEX = template.NULL_INDEX;
    if (template.VOLUME !== undefined) this.VOLUME = template.VOLUME;
  }
  var OK_STATE = 0, OUTOFMEMORY = 1, LOOP = 2, ADDRESS = 4, INCONSISTENT = 8;
  this.STATE = OK_STATE;

  this.isNullIndex = function(index) {
    return (index == (this.NULL_INDEX || 0));
  };

  this.getState = function(index) {
    var r = this.STATE;
    this.STATE = OK_STATE;
    return r;
  };

  this.maxAvailable = function() {
    var start = this.rootIndex, finish = this.memory[start], volume = 0, passed = start, repeated = passed;
    for (;start != finish; finish = this.memory[start]) {
      if (this.memory[start=finish] < finish) start = this.memory[finish];
      if (start === undefined) {
        this.STATE |= (INCONSISTENT|ADDRESS);
        break;
      }
      if (passed == start) {
        this.STATE |= (INCONSISTENT|LOOP);
        break;
      }
      if (passed == repeated) {
        passed = start;
        repeated = this.rootIndex;
      }
      else if (this.memory[repeated=this.memory[repeated]] < repeated) repeated = this.memory[repeated];
      if (finish-start+1>volume) volume = finish-start+1;
    }
    return volume;
  };

  this.allocateFirst = function(volume) {
    var start = this.rootIndex, finish = this.memory[start], pointer = start, passed = start, repeated = passed;
    for (;(start != finish) || ((this.STATE |= OUTOFMEMORY) === OK); finish = this.memory[pointer = start]) {
      if (this.memory[start=finish] < finish) start = this.memory[finish];
      if (start === undefined) {
        this.STATE |= (INCONSISTENT|ADDRESS);
        break;
      }
      if (passed == start) {
        this.STATE |= (INCONSISTENT|LOOP);
        break;
      }
      if (passed == repeated) {
        passed = start;
        repeated = this.rootIndex;
      }
      else if (this.memory[repeated=this.memory[repeated]] < repeated) repeated = this.memory[repeated];
      if (finish-start+1<volume) continue;
      if (finish-start+1>volume) if ((pointer = start+volume) < finish) this.memory[finish] = pointer;
      if (this.memory[start] == start) this.memory[pointer] = pointer;
      else this.memory[pointer] = this.memory[start];
      this.nextIndex = undefined;
      break;
    }
//    return start;
    return ((start == finish) && (volume > 1))? this.NULL_INDEX: start;
  };

  this.allocateFirstSafe = function(volume,update) {
    var start = this.rootIndex, finish = this.memory[start], pointer = start, passed = start, repeated = passed;
    for (;(start != finish) || ((this.STATE |= OUTOFMEMORY) === OK); finish = this.memory[pointer = start]) {
      if (this.memory[start=finish] < finish) start = this.memory[finish];
      if (start === undefined) {
        this.STATE |= (INCONSISTENT|ADDRESS);
        break;
      }
      if (passed == start) {
        this.STATE |= (INCONSISTENT|LOOP);
        break;
      }
      if (passed == repeated) {
        passed = start;
        repeated = this.rootIndex;
      }
      else if (this.memory[repeated=this.memory[repeated]] < repeated) repeated = this.memory[repeated];
      if (finish-start+1 < volume) continue;
      if (finish-start+1 > volume) if ((pointer = start+volume) < finish) this.memory[finish] = pointer;
      update.pointer = pointer;
      update.value = (this.memory[start] == start)? pointer: this.memory[start];
      this.nextIndex = undefined;
      break;
    }
//    return start;
    return ((start == finish) && (volume > 1))? this.NULL_INDEX: start;
  };

  this.allocateNext = function(volume) {
    var start = this.nextIndex = ((this.nextIndex === undefined)? this.rootIndex: this.nextIndex), finish = this.memory[start], pointer = start, passed = start, repeated = passed;
    for (;start != finish; finish = this.memory[pointer = start]) {
      if (this.memory[start=finish] < finish) start = this.memory[finish];
      if (start === undefined) {
        this.STATE |= (INCONSISTENT|ADDRESS);
        break;
      }
      if (passed == start) {
        this.STATE |= (INCONSISTENT|LOOP);
        break;
      }
      if (passed == repeated) {
        passed = start;
        repeated = this.rootIndex;
      }
      else if (this.memory[repeated=this.memory[repeated]] < repeated) repeated = this.memory[repeated];
      if (finish-start+1 < volume) continue;
      this.nextIndex = pointer;
      if (finish-start+1 > volume) if ((pointer = start+volume) < finish) this.memory[finish] = pointer;
      if (this.memory[start] == start) this.memory[pointer] = pointer;
      else this.memory[pointer] = this.memory[start];
      break;
    }
    if ((start == finish) && (volume > 1)) 
    for (finish = this.memory[repeated = passed = pointer = start = this.rootIndex]; (start != this.nextIndex) || ((this.STATE |= OUTOFMEMORY) === OK); finish = this.memory[pointer = start]) {
      if (this.memory[start=finish] < finish) start = this.memory[finish];
      if (start === undefined) {
        this.STATE |= (INCONSISTENT|ADDRESS);
        break;
      }
      if (passed == start) {
        this.STATE |= (INCONSISTENT|LOOP);
        break;
      }
      if (passed == repeated) {
        passed = start;
        repeated = this.rootIndex;
      }
      else if (this.memory[repeated=this.memory[repeated]] < repeated) repeated = this.memory[repeated];
      if (finish-start+1 < volume) continue;
      this.nextIndex = pointer;
      if (finish-start+1 > volume) if ((pointer = start+volume) < finish) this.memory[finish] = pointer;
      if (this.memory[start] == start) this.memory[pointer] = pointer;
      else this.memory[pointer] = this.memory[start];
      break;
    }
    if (this.nextIndex == this.rootIndex) this.nextIndex = undefined;
//    return start;
    return ((start == ((this.nextIndex === undefined)? this.rootIndex: this.nextIndex)) && (volume > 0))? this.NULL_INDEX: start;
  };

  this.allocateBest = function(volume) {
    var start = this.rootIndex, finish = this.memory[start], pointer = start, match = pointer, overhead = this.memory.length, passed = start, repeated = passed;
    for (;(start != finish) || ((this.STATE |= OUTOFMEMORY) === OK); finish = this.memory[pointer = start]) {
      if (this.memory[start=finish] < finish) start = this.memory[finish];
      if (start === undefined) {
        this.STATE |= (INCONSISTENT|ADDRESS);
        break;
      }
      if (passed == start) {
        this.STATE |= (INCONSISTENT|LOOP);
        break;
      }
      if (passed == repeated) {
        passed = start;
        repeated = this.rootIndex;
      }
      else if (this.memory[repeated=this.memory[repeated]] < repeated) repeated = this.memory[repeated];
      if (finish-start+1 < volume) continue;
      if (finish-start+2 > volume) if (finish-start+1-volume < overhead) {
        match = pointer;
        if ((overhead = finish-start+1-volume) == 0) break;
      }
    }
    if (overhead < this.memory.length) {
      finish = this.memory[pointer = match];
      if (this.memory[start=finish] < finish) start = this.memory[finish];
      if (finish-start+1 > volume) if ((pointer = start+volume) < finish) this.memory[finish] = pointer;
      if (this.memory[start] == start) this.memory[pointer] = pointer;
      else this.memory[pointer] = this.memory[start];
      this.nextIndex = undefined;
    }
//    return start;
    return ((start == finish) && (volume > 1))? this.NULL_INDEX: start;
  };

  this.allocateWorst = function(volume) {
    var start = this.rootIndex, finish = this.memory[start], pointer = start, match = pointer, overhead = 0, passed = start, repeated = passed;
    for (;(start != finish) || ((this.STATE |= OUTOFMEMORY) === OK); finish = this.memory[pointer = start]) {
      if (this.memory[start=finish] < finish) start = this.memory[finish];
      if (start === undefined) {
        this.STATE |= (INCONSISTENT|ADDRESS);
        break;
      }
      if (passed == start) {
        this.STATE |= (INCONSISTENT|LOOP);
        break;
      }
      if (passed == repeated) {
        passed = start;
        repeated = this.rootIndex;
      }
      else if (this.memory[repeated=this.memory[repeated]] < repeated) repeated = this.memory[repeated];
      if (finish-start+1 < volume) continue;
      if (finish-start+2 > volume) if (finish-start+2-volume > overhead) {
        match = pointer;
        overhead = finish-start+2-volume;
      }
    }
    if (overhead > 0) {
      finish = this.memory[pointer = match];
      if (this.memory[start=finish] < finish) start = this.memory[finish];
      if (finish-start+1 > volume) if ((pointer = start+volume) < finish) this.memory[finish] = pointer;
      if (this.memory[start] == start) this.memory[pointer] = pointer;
      else this.memory[pointer] = this.memory[start];
      this.nextIndex = undefined;
    }
//    return start;
    return ((start == finish) && (volume > 1))? this.NULL_INDEX: start;
  };

  this.reallocateFirst = function(volume,index,width) {
    var start = this.memory[index], finish = this.memory[index+volume-1], address, update = {};
    this.tryDeallocate(volume,index);
    address = this.allocateFirstSafe(width,update);
    if (address != this.rootIndex) {
      if (address != index) this.copy(address,index,(volume<width)?volume:width);
      if ((volume > 0) && (width > 0)) this.memory[address] = start;
      if ((volume > 1) && (width >= volume)) this.memory[address+volume-1] = finish;
      this.memory[update.pointer] = update.value;
    }
    else {
      address = this.allocateFirstSafe(volume,update);
      if (address != index) this.copy(address,index,volume);
      if (volume > 0) this.memory[address] = start;
      if (volume > 1) this.memory[address+volume-1] = finish;
      this.memory[update.pointer] = update.value;
      address = this.rootIndex;
      this.STATE |= OUTOFMEMORY;
    }
//    var address = this.allocateFirst(width);
//    if (address != this.rootIndex) {
//      this.copy(address,index,(volume<width)?volume:width);
//      this.tryDeallocate(volume,index);
//    }
    return address;
  };

  this.free = function(index,volume,address) {
    var start1 = this.memory[address], finish1 = address, start2, finish2;
    if (start1 > index) start1 = address;
    else if (this.memory[finish1=start1] < start1) start1 = this.memory[start1];
    start2 = this.memory[start1];
    if (this.memory[finish2=start2] < start2) start2 = this.memory[start2];
    if (index+volume == start2) {
      if (this.nextIndex === start2) this.nextIndex = index;
      volume += finish2-start2+1;
      start2 = this.memory[start2];
      if (this.memory[finish2=start2] < start2) start2 = this.memory[start2];
    }
    if (finish1+1 == index) if (finish1 > address) {
      if (this.nextIndex === index) this.nextIndex = start1;
      index = start1;
      volume += finish1-start1+1;
      start1 = address;
    }
    if (index+volume > start2) finish2 = index;
    this.memory[index+volume-1] = index;
    this.memory[index] = finish2;
    this.memory[start1] = index+volume-1;
  };

  this.tryDeallocate = this.deallocate = function(volume,index) {
    var start = this.rootIndex, finish = this.memory[start], pointer = start, address = pointer, passed = start, repeated = passed;
    for (;start != finish; finish = this.memory[pointer = start]) {
      if (this.memory[start=finish] < finish) start = this.memory[finish];
      if (start === undefined) {
        this.STATE |= (INCONSISTENT|ADDRESS);
        break;
      }
      if (passed == start) {
        this.STATE |= (INCONSISTENT|LOOP);
        break;
      }
      if (passed == repeated) {
        passed = start;
        repeated = this.rootIndex;
      }
      else if (this.memory[repeated=this.memory[repeated]] < repeated) repeated = this.memory[repeated];
      if (index+volume <= start) this.free(finish = index,volume,address);
      if (index <= finish) break;
      address = pointer;
    }
    if (index > finish) this.free(index,volume,address);
  };

  this.setRootIndex = function(rootIndex) {
    var r = this.rootIndex || this.NULL_INDEX || 0; this.rootIndex = rootIndex;
//    this.memory[rootIndex] = this.rootIndex;
//    this._deallocate(rootIndex + 1, this.VOLUME - rootIndex - 1, rootIndex);
    if ((this.NULL_INDEX === undefined) || (this.NULL_INDEX === r)) this.NULL_INDEX = rootIndex;
    this.nextIndex = undefined;
    return r;
  };

  return this;
};
var manager = Manager.call(machine.manager);
if (machine.scan(4) <= 1) machine.out(4,1); else alert('incompatible configuration');
var p = 5+machine.getProcessorIndex(this);
manager.setRootIndex(machine.scanPointer(p,p,p));
*/