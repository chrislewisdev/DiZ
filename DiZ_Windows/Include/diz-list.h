/*************************************************************
*Diz-list.h by Chris Lewis
*Declares- and defines- the DIZ_LIST class
*************************************************************/

//Check that this header hasn't already been included
#ifndef DIZ_LIST_H
#define DIZ_LIST_H

//Include string.h and our DIZ_LISTITEM class
#include <string.h>
#include "diz-listitem.h"

//Since we're not including windows.h in this file, make sure NULL is defined
#ifndef NULL
#define NULL	0
#endif

//This template class will handle the creation of linked lists using class TYPE
template <class TYPE> class DIZ_LIST {
public:
	//Declare our Constructor and Destructor
	DIZ_LIST();
	~DIZ_LIST();

	//Declare our public functions
	//These functions will add a new item to our list, with an integer or string ID
	DIZ_LISTITEM<TYPE> *addItem(int id);
	DIZ_LISTITEM<TYPE> *addItem(char id[]);
	//This function will return our root node
	DIZ_LISTITEM<TYPE> *rootItem();
	//These functions will retrieve an item specified by its integer or string ID
	DIZ_LISTITEM<TYPE> *getItem(int id);
	DIZ_LISTITEM<TYPE> *getItem(char id[]);
	//This function will return the number of elements in our list
	int count();
	//This function will delete an item (and all its following nodes)
	void deleteItem(DIZ_LISTITEM<TYPE> *target);
	//This function will clear out our entire list
	void kill();

private:
	//Declare our private properties
	//Declare our root DIZ_LISTITEM
	DIZ_LISTITEM<TYPE> *root;

protected:
	//Declare protected functions
	//This function will add a generic new item
	DIZ_LISTITEM<TYPE> *addNewItem();
};

//Declare our Constructor
template <class TYPE> DIZ_LIST<TYPE>::DIZ_LIST() {
	//Set all our values to safe defaults
	root = NULL;
}

//Declare our Destructor
template <class TYPE> DIZ_LIST<TYPE>::~DIZ_LIST() {

}

//This function will add a new item into our list
template <class TYPE> DIZ_LISTITEM<TYPE> *DIZ_LIST<TYPE>::addItem(int id)
{
	//Retrieve a newly added item, set its ID, and return it
	DIZ_LISTITEM<TYPE> *newItem = addNewItem();
	newItem->ID = id;

	return newItem;
}

//This function will add a new item with a string ID
template <class TYPE> DIZ_LISTITEM<TYPE> *DIZ_LIST<TYPE>::addItem(char id[])
{
	//Retrieve a newly added item, set its ID with strncpy, and return it
	DIZ_LISTITEM<TYPE> *newItem = addNewItem();
	strncpy(newItem->strID, id, strlen(id));

	return newItem;
}

//This function will return our root item
template <class TYPE> DIZ_LISTITEM<TYPE> *DIZ_LIST<TYPE>::rootItem()
{
	return root;
}

//This function will retrieve a specified item
template <class TYPE> DIZ_LISTITEM<TYPE> *DIZ_LIST<TYPE>::getItem(int id)
{
	//Create a conductor set to our root node
	DIZ_LISTITEM<TYPE> *cdtr = root;

	//Make sure we HAVE a root node...
	if (cdtr)
	{
		//Loop through until we've found our ID or have reached the end
		while (cdtr->nextItem() && cdtr->ID != id)
		{
			cdtr = cdtr->nextItem();
		}

		//Once we finish, check whether or not we actually found it
		if (cdtr->ID == id)
		{
			return cdtr;
		}
	}

	//Otherwise, we return NULL
	return NULL;
}

//This function will retrieve an item specified by its string ID
template <class TYPE> DIZ_LISTITEM<TYPE> *DIZ_LIST<TYPE>::getItem(char id[])
{
	//Create a conductor set to our root node
	DIZ_LISTITEM<TYPE> *cdtr = root;

	//Make sure we HAVE a root node...
	if (cdtr)
	{
		//Loop through until we've found our ID or have reached the end
		while (cdtr->nextItem() && strncmp(cdtr->strID, id, strlen(id)))
		{
			cdtr = cdtr->nextItem();
		}

		//Once we finish, check whether or not we actually found it
		if (!strncmp(cdtr->strID, id, strlen(id)))
		{
			return cdtr;
		}
	}

	//Otherwise, we return NULL
	return NULL;
}

//This function will return the no. of elements in the list
template <class TYPE> int DIZ_LIST<TYPE>::count()
{
	//Declare a conductor variable and counter variable
	DIZ_LISTITEM<TYPE> *cdtr = root;
	int counter = 1;
	
	//If we have no nodes, return 0
	if (!cdtr)
	{
		return 0;
	}
	else
	{
		//Otherwise, loop through our list and count the items
		while (cdtr->nextItem())
		{
			cdtr = cdtr->nextItem();
			counter++;
		}

		//Then return the count
		return counter;
	}
}

//This function will delete a specified node (and all following nodes)
template <class TYPE> void DIZ_LIST<TYPE>::deleteItem(DIZ_LISTITEM<TYPE> *target)
{
	//Check we have a target
	if (target)
	{
		//If we have any more nodes, delete them
		if (target->nextItem())
		{
			deleteItem(target->nextItem());
		}
		
		//Then call the item's kill function
		target->kill();
	}
}

//This function will kill our entire list
template <class TYPE> void DIZ_LIST<TYPE>::kill()
{
	deleteItem(root);
}

//This function will add a new generic item to our list
template <class TYPE> DIZ_LISTITEM<TYPE> *DIZ_LIST<TYPE>::addNewItem()
{
	//Declare a pointer for our target node and a conductor pointer
	DIZ_LISTITEM<TYPE> **target = NULL;
	DIZ_LISTITEM<TYPE> *cdtr = NULL;

	//If root doesn't exist, we create a new node right there
	if (!root)
	{
		target = &root;
	}
	else
	{
		//Otherwise we have to find the end of the list
		cdtr = root;
		//Loop until we reach the end of our list
		while (cdtr->nextItem())
		{
			cdtr = cdtr->nextItem();
		}
		//Target that node's connection
		target = &cdtr->next;
	}

	//Then create our new item and return it
	*target = new DIZ_LISTITEM<TYPE>(cdtr, NULL);

	return cdtr;
}

#endif