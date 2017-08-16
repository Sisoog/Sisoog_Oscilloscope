/*
 * Cpp_Heap_Hooks.cpp
 *
 *  Created on: ?? ÂÈÇä ???? åž.Ô.
 *      Author: mazarei
 */
#include <stdio.h>
#include<new>


extern "C" void *pvPortMalloc( size_t xWantedSize );
extern "C" void vPortFree( void *pv );

void *operator new (std::size_t  size)
{
	return pvPortMalloc(size);
}


void operator delete(void *ptr)
{
	vPortFree(ptr);
}



//void *
//operator new[] (size_t size)
//{
//   // do more stuff here
//}
//
//void
//operator delete[] (void *ptr)
//{
//   // undo more stuff here
//}



extern "C" void free(void *p)
{
	vPortFree(p);
}

extern "C" void *malloc(size_t s)
{
    return pvPortMalloc(s);
}
