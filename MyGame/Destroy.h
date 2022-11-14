#pragma once
#include<memory>

template <class T>
void Destroy(T*& p)
{
	delete p;
	p = nullptr;
}

template <class T>
void Destroy_unique(std::unique_ptr<T>& p)
{
	p.release();
	//p.relese();
	p = nullptr;
}
