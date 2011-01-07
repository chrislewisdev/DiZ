/*************************************************************
*Diz-list.h by Chris Lewis
*Declares- and defines- the DIZ_LIST class
*************************************************************/

//Check that this header hasn't already been included
#ifndef DIZ_LIST_H
#define DIZ_LIST_H

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
	TYPE *addItem(int id);
	//This function will add a new item with a char-string ID
	TYPE *addItem(char id[]);
	//This function will add a new item's contents with both types of IDs
	TYPE *addItem(int id, char strId[]);
	//This function will retrieve an item's contents by its numeric ID
	TYPE *getItem(int id);
	//This function will retrieve an item by its char-string ID
	TYPE *getItem(char id[]);
	//This function will assist in iteration through our list
	TYPE *iterate(bool start);
	//This function will delete a specified item found by its numeric ID
	void deleteItem(int id);
	//This function will delete a specified item found by its char-string ID
	void deleteItem(char id[]);
	//This function will return the amount of items in our list
	int numberItems();
	//This function will clear out our entire list
	void kill();

	//Declare our public properties
	//Declare our external numeric ID
	int exId;
	//Declare our external char-string ID
	char strId[256];

private:
	//Declare our private functions
	//This function will add a new item our list
	DIZ_LIST<TYPE> *addItem();
	//This function will retrieve an item by its ordered ID
	DIZ_LIST<TYPE> *getOrderedItem(int id);
	//This function will retrieve an item by its numeric ID
	DIZ_LIST<TYPE> *getIDItem(int id);
	//This function will retrieve an item by its string ID
	DIZ_LIST<TYPE> *getIDItem(char id[]);
	//This function will delete a specified DIZ_LIST item
	void deleteItem(DIZ_LIST<TYPE> *target, bool join);
	//This function will set a specified item's ID
	void setID(DIZ_LIST<TYPE> *target, int id);
	//This function will compare two strings for equality
	bool checkString(char s1[], char s2[]);

	//Declare our private properties
	//Declare our ordered ID number for each item
	int id;
	//Declare an iteration counter for iterate()
	int iterator;
	//Declare our item contents of our specified TYPE
	TYPE item;
	//Declare our next item in our list
	DIZ_LIST<TYPE> *next;

	//Now declare a static variable for the amount of items we have
	static int numItems;
};

//Declare our static variable
template <class TYPE> int DIZ_LIST<TYPE>::numItems;

//Declare our Constructor
template <class TYPE> DIZ_LIST<TYPE>::DIZ_LIST() {
	//Set all our values to safe defaults
	exId = 0;
	strId[0] = '\0';
	id = 0;
	next = NULL;
	numItems = 0;
	iterator = 1;
}

//Declare our Destructor
template <class TYPE> DIZ_LIST<TYPE>::~DIZ_LIST() {

}

//This function will add a new item, setting its numeric ID
template <class TYPE> TYPE *DIZ_LIST<TYPE>::addItem(int id) {
	//Declare our conductor variable and set it to our added item
	DIZ_LIST<TYPE> *cdtr = addItem();

	//Assign it our ID value
	cdtr->exId = id;

	//Then return the item
	return &cdtr->item;
}

//This function will add a new item, setting its string ID
template <class TYPE> TYPE *DIZ_LIST<TYPE>::addItem(char id[]) {
	//Declare our conductor variable and set it to our new item
	DIZ_LIST<TYPE> *cdtr = addItem();
	//Declare an iterative variable
	int i = 0;

	//And loop through our externally-assigned ID to assign it to our item's ID
	while (id[i] != '\0') {
		cdtr->strId[i] = id[i];
		i++;
	}
	//Then null-terminate the string ID
	cdtr->strId[i] = '\0';

	//Now return our newly created item
	return &cdtr->item;
}

//This function will add a new item, setting both its numeric and string IDs
template <class TYPE> TYPE *DIZ_LIST<TYPE>::addItem(int id, char strId[]) {
	//Declare our conductor variable and set it to our new item
	DIZ_LIST<TYPE> *cdtr = addItem();
	//Declare an iterative variable
	int i = 0;

	//And set our numeric ID to the specified value
	cdtr->exId = id;
	//And loop through our externally-assigned ID to assign it to our item's ID
	while (strId[i] != '\0') {
		cdtr->strId[i] = strId[i];
		i++;
	}
	//Then null-terminate the string ID
	cdtr->strId[i] = '\0';

	//Now return our newly created item
	return &cdtr->item;
}

//This function will retrieve a specified item by its numeric ID
template <class TYPE> TYPE *DIZ_LIST<TYPE>::getItem(int id) {
	//Create a conductor variable set to our desired item
	DIZ_LIST<TYPE> *cdtr = getIDItem(id);

	//Make sure it exists before returning its contents
	if (cdtr != NULL) {
		return &cdtr->item;
	}else {
		return NULL;
	}
}

//This function will retrieve a specified item by its string ID
template <class TYPE> TYPE *DIZ_LIST<TYPE>::getItem(char id[]) {
	//Create a conductor variable set to our desired item
	DIZ_LIST<TYPE> *cdtr = getIDItem(id);

	//Make sure it exists before returning its contents
	if (cdtr != NULL) {
		return &cdtr->item;
	}else {
		return NULL;
	}
}

//This function will assist in iteration through our list
template <class TYPE> TYPE *DIZ_LIST<TYPE>::iterate(bool start) {
	//Check if we want to start a new iteration- if so, set our ID back to 1
	if (start) {
		iterator = 1;
	}

	//Attempt to retrieve our current desired item and increment our iterator
	TYPE *cdtr = getOrderedItem(iterator);
	iterator++;

	//Then return our result
	return cdtr;
}

//This function will delete a specified item, found by its numeric ID
template <class TYPE> void DIZ_LIST<TYPE>::deleteItem(int id) {
	//Create our target handle for searching for our ID
	DIZ_LIST<TYPE> *cdtr = this;

	//Loop as long as there are any items left and we haven't found our ID
	while (cdtr != NULL && cdtr->exId != id) {
		cdtr = cdtr->next;
	}
	//Make sure we found our target properly- if we did, go ahead and delete it
	if (cdtr != NULL && cdtr->exId == id) {
		deleteItem(cdtr, true);
	}
}

//This function will delete a specified item, found by its string ID
template <class TYPE> void DIZ_LIST<TYPE>::deleteItem(char id[]) {
	//Create our target handle for searching for our ID
	DIZ_LIST<TYPE> *cdtr = this;

	//Loop as long as there are any items left and we haven't found our ID
	while (cdtr != NULL && !checkString(cdtr->strId, id)) {
		cdtr = cdtr->next;
	}
	//Make sure we found our target properly- if we did, go ahead and delete it
	if (cdtr != NULL && checkString(cdtr->strId, id)) {
		deleteItem(cdtr, true);
	}
}

//This function will return the number of items in our list
template <class TYPE> int DIZ_LIST<TYPE>::numberItems() {
	return numItems;
}

//This function will kdelete our entire list of items
template <class TYPE> void DIZ_LIST<TYPE>::kill() {
	//Check if we have a base item- and if we do, set off our deletion recursion
	if (next != NULL) {
		deleteItem(next, false);
	}
}

//This function will add a new item onto our list
template <class TYPE> DIZ_LIST<TYPE> *DIZ_LIST<TYPE>::addItem() {
	//Declare our conductor variable, starting at our base item
	DIZ_LIST<TYPE> *cdtr = this;
	//Declare storage for a temporary ID value
	int tempID;

	//Now loop until we reach the end of our list
	while (cdtr->next != NULL) {
		cdtr = cdtr->next;
	}
	//Now store our current ID value as our end-value
	tempID = cdtr->id;
	//Then create our new item at the end of our list, and move onto it
	cdtr->next = new DIZ_LIST<TYPE>;
	cdtr = cdtr->next;
	//Assign our new object our next ID value in line
	cdtr->id = tempID + 1;

	//Then increment our item counter
	numItems++;

	//Now return our newly created item
	return cdtr;
}

//This function will retrieve an item by its internal ordered ID
template <class TYPE> DIZ_LIST<TYPE> *DIZ_LIST<TYPE>::getOrderedItem(int id) {
	//Declare a conductor variable and set it to our base item
	DIZ_LIST<TYPE> *cdtr = this;

	//Loop as long as we have items and haven't found our desired ID
	while (cdtr != NULL && cdtr->id != id) {
		cdtr = cdtr->next;
	}
	//Once we've hit the end, check if we've found our desired ID- if we do, return it
	if (cdtr != NULL && cdtr->id == id) {
		return cdtr;
	}else {
		//Otherwise, return NULL as no result
		return NULL;
	}
}

//This function wil retrieve an item by its numeric ID
template <class TYPE> DIZ_LIST<TYPE> *DIZ_LIST<TYPE>::getIDItem(int id) {
	//Create a conductor variable set to our base item
	DIZ_LIST<TYPE> *cdtr = this;

	//Loop as long as there are any items left and we haven't found our ID
	while (cdtr != NULL && cdtr->exId != id) {
		cdtr = cdtr->next;
	}
	//Once we're here, check if we have our desired ID or not
	if (cdtr != NULL && cdtr->exId == id) {
		//If we do, return it
		return cdtr;
	}else {
		//Otherwise, return NULL as no result
		return NULL;
	}
}

//This function will retrieve an item by its string ID
template <class TYPE> DIZ_LIST<TYPE> *DIZ_LIST<TYPE>::getIDItem(char id[]) {
	//Create a conductor variable set to our base item
	DIZ_LIST<TYPE> *cdtr = this;

	//Loop as long as there are any items left and we haven't found our ID
	while (cdtr != NULL && !checkString(cdtr->strId, id)) {
		cdtr = cdtr->next;
	}
	//Once we're here, check if we have our desired ID or not
	if (cdtr != NULL && checkString(cdtr->strId, id)) {
		//If we do, return it
		return cdtr;
	}else {
		//Otherwise, return NULL as no result
		return NULL;
	}
}

//This function will delete an actual specified item
template <class TYPE> void DIZ_LIST<TYPE>::deleteItem(DIZ_LIST<TYPE> *target, bool join) {
	//Retrieve handles for our previous and next items in the list
	DIZ_LIST<TYPE> *prev = getOrderedItem(target->id - 1);
	DIZ_LIST<TYPE> *next = getOrderedItem(target->id + 1);

	//Check if we have any items ahead to worry about
	if (next != NULL) {
		//Now check if we want to join our list items or not
		if (!join) {
			//If we don't, go ahead and delete our next item
			deleteItem(next, false);
			//And check if we have a previous item to take care of
			if (prev != NULL) {
				prev->next = NULL;
			}
		}else {
			//If we do, we connect our previous and next items together in the list
			prev->next = next;
			//Then re-set our next item's ID to go in line with our new order
			setID(next, prev->id + 1);
		}
		//Then we delete our target item
		delete target;
		target = NULL;
	}else {
		//If there's nothing to worry about, just delete our target item
		delete target;
		target = NULL;
		//And then, if there's any previous item, make sure that isn't pointing to our target anymore
		if (prev != NULL) {
			prev->next = NULL;
		}
	}

	//Now finally, decrement our item counter
	numItems--;
}

//This function will set the internal ID of a specified item
template <class TYPE> void DIZ_LIST<TYPE>::setID(DIZ_LIST<TYPE> *target, int id) {
	//Set our specified ID
	target->id = id;
	//Then check if the ordered ID of the next item is actually still in order
	if (target->next != NULL && target->next->id != target->id + 1) {
		//If it's not, correct that item's ID
		setID(target->next, target->id + 1);
	}
}

//This function will compare two strings for equality
template <class TYPE> bool DIZ_LIST<TYPE>::checkString(char s1[], char s2[]) {
	//Declare an iterative variable for our string progression
	int i = 0;

	//Now loop through our string as long as the characters are equal and we haven't reached the end of either
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0') {
		i++;
	}
	//Then, check our very last character to check if we're still equal or not
	if (s1[i] == s2[i]) {
		//If we are, the two strings are equal
		return true;
	}else {
		//If not, well, yeah
		return false;
	}
}

#endif