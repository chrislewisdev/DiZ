/*************************************************************
*Diz-list.h by Chris Lewis
*Declares- and defines- the DIZ_LIST class
*************************************************************/

//Check that this header hasn't already been included
#ifndef DIZ_LIST_H
#define DIZ_LIST_H

//Include our DIZ_LISTITEM class
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
	//This function will add a new item to our list, with an integer ID
	DIZ_LISTITEM<TYPE> *addItem(int id);
	//This function will return our root node
	DIZ_LISTITEM<TYPE> *rootItem();
	//This function will retrieve an item's contents by its numeric ID
	DIZ_LISTITEM<TYPE> *getItem(int id);
	//This function will delete an item (and all its following nodes)
	void deleteItem(DIZ_LISTITEM<TYPE> *target);
	//This function will clear out our entire list
	void kill();

private:
	//Declare our private properties
	//Declare our root DIZ_LISTITEM
	DIZ_LISTITEM<TYPE> *root;
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
	*target = new DIZ_LISTITEM<TYPE>;
	(*target)->id = id;
	(*target)->prev = cdtr;

	return *target;
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
		while (cdtr->nextItem() && cdtr->id != id)
		{
			cdtr = cdtr->nextItem();
		}

		//Once we finish, check whether or not we actually found it
		if (cdtr->id == id)
		{
			return cdtr;
		}
	}

	//Otherwise, we return NULL
	return NULL;
}

//This function will delete a specified node (and all following nodes)
template <class TYPE> void DIZ_LIST<TYPE>::deleteItem(DIZ_LISTITEM<TYPE> *target)
{
	//Declare a pointer for a previous node (in case)
	DIZ_LISTITEM<TYPE> *previous = NULL;

	//Check we have a target
	if (target)
	{
		//If we have any more nodes, delete them
		if (target->nextItem())
		{
			deleteItem(target->nextItem());
		}
		
		//Check if there's a previous item, and if there is, set its next to NULL
		if (target->previousItem())
		{
			previous = target->previousItem();
			previous->next = NULL;
		}

		//Now that all that's taken care of, we can delete our target
		delete target;
	}
}

//This function will kill our entire list
template <class TYPE> void DIZ_LIST<TYPE>::kill()
{
	deleteItem(root);
}

#endif