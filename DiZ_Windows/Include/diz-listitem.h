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
	DIZ_LISTITEM(DIZ_LISTITEM<TYPE> *_prev, DIZ_LISTITEM<TYPE> *_next);
	~DIZ_LISTITEM();

	//Declare our public functions
	//Declare functions to return next/previous items
	DIZ_LISTITEM<TYPE> *nextItem();
	DIZ_LISTITEM<TYPE> *previousItem();
	//These functions will set our int or string IDs
	void setID(int id);
	void setID(char id[]);
	//Declare a function to delete this node
	void kill();

	//Declare our public properties
	//Declare our contained item
	TYPE item;

private:
	//Declare our private properties
	//Declare next and previous pointer
	DIZ_LISTITEM<TYPE> *next;
	DIZ_LISTITEM<TYPE> *prev;

	//Declare private properties
	//Declare an ID value
	int ID;
	//Declare a string-ID
	char strID[256];

	//Declare a static item counter
	static int total;

	//Declare DIZ_LIST a friend class
	template <class TYPE> friend class DIZ_LIST;
};

//Initialise our total items counter
template <class TYPE> int DIZ_LISTITEM<TYPE>::total = 0;

//Define our Constructor
template <class TYPE> DIZ_LISTITEM<TYPE>::DIZ_LISTITEM(DIZ_LISTITEM<TYPE> *_prev, DIZ_LISTITEM<TYPE> *_next)
{
	ID = 0;
	strID[0] = '\0';
	next = _next; prev = _prev;
	total++;
}

//Define our Destructor
template <class TYPE> DIZ_LISTITEM<TYPE>::~DIZ_LISTITEM()
{

}

//This function will return our next item in the list
template <class TYPE> DIZ_LISTITEM<TYPE> *DIZ_LISTITEM<TYPE>::nextItem()
{
	return next;
}

//This function will return our previous item in the list
template <class TYPE> DIZ_LISTITEM<TYPE> *DIZ_LISTITEM<TYPE>::previousItem()
{
	return prev;
}

//This function will set the item's integer ID
template <class TYPE> void DIZ_LISTITEM<TYPE>::setID(int id)
{
	ID = id;
}

//This function will set the item's string-ID
template <class TYPE> void DIZ_LISTITEM<TYPE>::setID(char id[])
{
	strncpy_s(strID, sizeof(char) * 256, id, strlen(id));
}

//This function will delete this particular node
template <class TYPE> void DIZ_LISTITEM<TYPE>::kill()
{
	//Start off changing our next and previous' connections (where necessary)
	if (prev)
	{
		prev->next = next;
	}
	if (next) 
	{
		next->prev = prev;
	}

	total--;

	//Then delete this node
	delete this;
}

#endif