#ifndef INTRUSIVE_LIST_H
#define INTRUSIVE_LIST_H

// TODO: Make this noncopyable.
template <typename T>
class ListHandle {
	private:
		ListHandle* prev;
		ListHandle* next;
		
		void insert_between(ListHandle& new_prev, ListHandle& new_next) {
			if(new_prev.p == p || new_next.p == p) {
				return;
			}
			
			// Remove this element from any previous position first.
			prev->next = next;
			next->prev = prev;
			
			// Insert this element into new position.
			prev = &new_prev;
			next = &new_next;
			prev->next = this;
			next->prev = this;
		}
	
	public:
		T* const p;
		
		ListHandle(T* ptr) : prev(this), next(this), p(ptr) {}
		
		ListHandle* get_prev() {
			return prev;
		}
		
		ListHandle* get_next() {
			return next;
		}
		
		void insert_before(ListHandle& node) {
			insert_between(*node.prev, node);
		}
		
		void insert_after(ListHandle& node) {
			insert_between(node, *node.next);
		}
};

template <typename T>
class List {
	private:
		ListHandle<T> root;
	
	public:
		class Iterator {
			private:
				ListHandle<T>* h;
			
			public:
				Iterator(ListHandle<T>* handle) : h(handle) {}
				
				ListHandle<T>* operator*() {
					return h;
				}
				
				ListHandle<T>* operator->() {
					return h;
				}
				
				bool operator==(const Iterator& i) {
					return h == i.h;
				}
				
				bool operator!=(const Iterator& i) {
					return h != i.h;
				}
				
				Iterator& operator++() {
					h = h->get_next();
					return *this;
				}
		};
		
		List() : root(nullptr) {}
		
		Iterator begin() {
			return {root.get_next()};
		}
		
		Iterator end() {
			return {&root};
		}
		
		bool empty() {
			return begin() == end();
		}
		
		void append(ListHandle<T>& handle) {
			//root.insert_before(&handle);
			handle.insert_before(root);
		}
};

#endif
