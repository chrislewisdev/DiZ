/***************************************************************
*Diz-listitem.h by Chris Lewis
*Declares and defines the DIZ_LISTITEM template class
***************************************************************/

//Check we haven't already used this header
#ifndef DIZ_LISTITEM_H
#define DIZ_LISTITEM_H

//Make sure NULL is defined
#ifndef NULL
#define NULL	0
#endif

//Declare our class
template <class TYPE> class DIZ_LISTITEM
{
public:
	//Declare our Constructor and Destructor
	DIZ_LISTITEM();
	~DIZ_LISTITEM();

	//Declare our public functions
	//Declare functions to return next/previous items
	DIZ_LISTITEM<TYPE> *nextItem();
	DIZ_LISTITEM<TYPE> *previousItem();

	//Declare our public properties
	//Declare an ID value
	int id;
	//Declare our contained item
	TYPE item;

private:
	//Declare our private properties
	//Declare next and previous pointer
	DIZ_LISTITEM<TYPE> *next;
	DIZ_LISTITEM<TYPE> *prev;
};

//Define our Constructor
template <class TYPE> DIZ_LISTITEM<TYPE>::DIZ_LISTITEM()
{
	id = 0;
	next = NULL; prev = NULL;
}

//Define our Destructor
template <class TYPE> DIZ_LISTITEM<TYPE>::~DIZ_LISTITEM()
{

}

//This function will return our next item in the list
template <class TYPE> DIZ_LISTITEM<TYPE>::nextItem()
{
	return next;
}

//This function will return our previous item in the list
template <class TYPE> DIZ_LISTITEM<TYPE>::previousItem()
{
	return prev;
}

#endif