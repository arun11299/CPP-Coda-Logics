#include <iostream>
#include <cstdlib>

struct Base;
struct base_vtable {
	// Virtual table holding the 
    // virtual functions for
    // run time polymorphism

	void (*virtual_func1)(Base*);
	int  (*virtual_func2)(Base*, int);
	
};

struct Base {
	base_vtable* bvptr_;
	int m_a;
};

void base_cons(Base* thisp) {
	std::cout << "Base class constructor" << std::endl;
	thisp->m_a = 0;
}

void func1(Base* thisp) {
	std::cout << "Base func1" << std::endl;
}

int func2(Base* thisp, int x) {
	std::cout << "Base func2" << std::endl;
}

int non_virtual_func(Base* thisp) {
	std::cout << "Base non virtual function" << std::endl;
}

base_vtable b_vtable = {&func1, &func2};

Base* new_base() {
	Base* ptr = (Base*)malloc(sizeof(Base));
	ptr->bvptr_ = &b_vtable;
	base_cons(ptr);

	return ptr;
}

// Derived section
struct Derived;
struct derived_vtable {
	base_vtable bvtable;
	void (*virtual_func3)(Derived*);
};

struct Derived {
	Base base;

	// In C++ per class has only 1 vtable pointer.
    // Here we have 2 vtable pointers, 1 in base
    // and another down as dvptr_
	derived_vtable* dvptr_;
};

void der_cons(Derived* thisp) {
	base_cons((Base*)thisp);
	std::cout << "Derived constructor" << std::endl;
}

// Overridden method from Base
// Leaving its implementation in C
// since C++ resolves it to a different symbol
// using name mangling. So there wont be any conflict 
// with func2 of Base
/*
void func2(Derived* thisp) {
	std::cout << "Derived func2" << std::endl;
}
*/

void func3(Derived* thisp) {
	std::cout << "Derived func3" << std::endl;
}

derived_vtable d_vtable = {b_vtable, &func3};

Derived* new_derived() {
	Derived* ptr = (Derived*)malloc(sizeof(Derived));	
	ptr->dvptr_ = &d_vtable;
	der_cons(ptr);

	return ptr;
}

int main() {
	Derived* d = new_derived();
	// This is hidden by C++ compiler
	d->dvptr_->bvtable.virtual_func1((Base*)d);
	return 0;
}
