// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Yash Shah

#include <iostream> // debugging
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
{
	count = 0;
	elements = NULL;
	reserved = 0;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{
	count = rhs.count;
	reserved = rhs.reserved;
	elements = (T*)malloc(count * sizeof(T));

	for(int i = 0; i < count; i++){
		new (elements + i)T(rhs.elements[i]);
	}
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
	reserved = nReserved;
	elements = (T*)malloc(reserved * sizeof(T));
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"

	elements = (T*)malloc(n * sizeof(T));

	for(int i = 0; i < n; i++){
		new (elements + i)T(t);
	}

	count = n;
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
	for(int i = 0; i<count; i++){
		elements[i].~T();
	}
	free(elements);
	count =0;
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
	if(count >= reserved){

		T* newElement = (T*)malloc((count+1)* sizeof(T));
		for(int i=0; i<count; i++){
			new (newElement + i)T(elements[i]);
			elements[i].~T();
		}

		free(elements);
		elements = newElement;
		reserved = count+1;
	}

	new (elements+count)T(rhs);
	count++;

}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
	if(count < reserved){
		for(int i=count; i>0; i--){
			elements[i] = elements[i-1];
		}
	}
	else{
		T* newElement = (T*)malloc((count+1) * sizeof(T));
		for(int i=count; i>0; i--){
			new (newElement + i)T(elements[i-1]);
			elements[i-1].~T();
		}
		free(elements);
		elements = newElement;
		reserved = count+1;
	}

	new (elements)T(rhs);
	count++;
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
	if(count > 0){
		elements[count-1].~T();
		count--;
	}
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
	if(count>0){
		elements[0].~T();

		for(int i=0; i<count-1; i++){
			new (elements+i)T(elements[i+1]);
			elements[i+1].~T();
		}

		count--;
	}
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
	return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
	return elements[count-1];
}

template <typename T>
T& Vector<T>::operator[](size_t i)
{
	return elements[i];
}

template <typename T>
size_t Vector<T>::Size() const
{
	return count;
}

template <typename T>
bool Vector<T>::Empty() const
{	
	if(count == 0){
		return true;
	}

	return false;
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
	for(int i=0; i<count; i++){
		elements[i].~T();
	}
	count = 0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
	return VectorIterator<T>(elements + count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
	count--; 
	VectorIterator<T> itemp;  
	int pos = 0;

	for(itemp=elements; itemp != (elements+count-1); itemp++){
		pos++;
	    if(itemp == it){
	    	break;
	    }
	}

  	elements[pos - 1].~T();
  	
  	for(int i = (pos - 1); i < count; i++){
  		new (elements + i)T(elements[i + 1]); 
  		elements[i + 1].~T();
  	}   
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
	count++;
	T* temp = (T*)malloc((count) * sizeof(T));
	VectorIterator<T> itemp;
	int pos = 0;


	for(itemp=elements; itemp!=(elements+count-1); itemp++){ 
	    pos++;
	    if(itemp == it){
	    	break;
	    }
	}
  
	for(int i = 0; i < (pos - 1); i++){

		new (temp + i)T(elements[i]); 
		elements[i].~T();
	}

	new(temp + pos - 1)T(rhs);

	for(int i = pos; i < count; i++)
	{
		new (temp + i)T(elements[i - 1]);  
		elements[i - 1].~T();
	}


	free(elements);
	elements = temp;

}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
	current = NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
	current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
	current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
	return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
	current++;
	return *this;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
	VectorIterator<T> temp(current);
	current++;
	return temp;
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
	if(current != rhs.current){
		return true;
	}
	return false;
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
	if(current == rhs.current){
		return true;
	}
	return false;
}


template <typename T>
void Vector<T>::Reserve(size_t n)
{
	reserved = n;
	elements = (T*)malloc(reserved * sizeof(T));
}

