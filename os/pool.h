#ifndef POOL_H
#define POOL_H

#include "stdint.h"

template<class T>
class BasePool {
	public:
		struct Element {
			unsigned int use_count;
			BasePool* pool;
			
			char data[sizeof(T)];
		};
		
		virtual void free(Element* e) = 0;
};

template<class T>
class P {
	private:
		typedef typename BasePool<T>::Element Element;
		
		Element* e;
		
		void inc() {
			e->use_count++;
		}
		
		void dec() {
			e->use_count--;
			if(!e->use_count) {
				T* p = (T*)e->data;
				p->~T();
				e->pool->free(e);
			}
		}
	
	public:
		P() : e(0) {}
		
		explicit P(Element* ep) : e(ep) {
			inc();
		}
		
		P(const P& p) : e(p.e) {
			inc();
		}
		
		~P() {
			if(e) {
				dec();
			}
		}
		
		void operator=(const P& p) {
			if(e) {
				dec();
			}
			
			e = p.e;
			
			if(e) {
				inc();
			}
		}
		
		void reset() {
			if(e) {
				dec();
			}
			
			e = 0;
		}
		
		T* operator->() {
			return (T*)e->data;
		}
		
		T* operator*() {
			return (T*)e->data;
		}
		
		operator bool() {
			return bool(e);
		}
};

template<class T, unsigned int size>
class Pool : public BasePool<T> {
	private:
		typedef typename BasePool<T>::Element Element;
		
		union Node {
			Element e;
			Node* next;
		};
		
		Node elements[size];
		
		Node* next_free;
		
		void free(Element* e) {
			Node* n = (Node*)e;
			
			n->next = next_free;
			next_free = n;
		}
		
		Element* alloc() {
			if(!next_free) {
				return 0;
			}
			
			Element* e = &next_free->e;
			next_free = next_free->next;
			
			e->use_count = 0;
			e->pool = this;
			
			return e;
		}
	
	public:
		Pool() : next_free(0) {
			for(unsigned int i = 0; i < size; i++) {
				free(&elements[i].e);
			}
		}
		
		P<T> create() {
			Element* e = alloc();
			
			if(!e) {
				return P<T>();
			}
			
			new (e->data) T;
			
			return P<T>(e);
		}
};

void* operator new(unsigned int, char* buf);

#endif
