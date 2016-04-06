#include "frequencyList.h"

//returns empty frequency List Struct
FrequencyList * getFrequencyList() {
	FrequencyList * fList = malloc(sizeof(FrequencyList));
	fList->numOfNodes = 0;
	fList->head = 0;
	fList->tail = 0;
	return fList;
}

//return 0 if failed
int addToList(char * filename, char * token, FrequencyList * fList) {

	if(filename == 0 || token == 0 || fList == 0){
		printf("Please enter valid parameters\n");
		return 0;
	}

	Node * temp = malloc(sizeof(Node));
	temp->token = token;
	temp->filename = filename;
	temp->frequency = 1;
	temp->next = 0;
	temp->prev = 0;
	temp->isTrailing = FALSE;

	if(fList->head == 0){

		fList->head = temp;
		fList->tail = temp;
		fList->numOfNodes++;
		return 1;
	} 

	Node * currNode = fList->head;
	while(currNode != 0){
		//compare token to currNode token
		if(strcmp(currNode->token,token) == 0)
		{
			//if same token and same filename then +1
			if(strcmp(currNode->filename, filename) == 0 )
			{
				currNode->frequency++;
				free(temp);
				free(token);
				return 1;
			}
			//if same token but different filenames then temp is now trailing... 
			else if(strcmp(filename, currNode->filename) > 0) 
			{
				temp->isTrailing = TRUE;
				temp->next = currNode;
				if(currNode->prev != 0)
				{
					temp->prev = currNode->prev;
					temp->prev->next = temp;
				} 
				else
				{
					fList->head = temp;
				}
				currNode->prev = temp;
				fList->numOfNodes++;
				return 1;
			}
			
		} else if(strcmp(token,currNode->token) < 0)
		{
			//currNode is greater than prev Node
			//add before currNode
			temp->next = currNode;
			if(currNode->prev != 0){
				temp->prev = currNode->prev;
				temp->prev->next = temp;
			} else
			{
				fList->head = temp;
			}

			currNode->prev = temp;
			fList->numOfNodes++;
			return 1;
		} 
		currNode = currNode->next;
	}
	//if it makes it out of loop then append to end of list because token is larger then every element in list
	//handles adding to end of list
	fList->tail->next = temp;
	temp->prev = fList->tail;
	fList->tail = temp;
	fList->numOfNodes++;
	return 1;

}

/*FOR USE IN DEBUGGING ONLY*/
void printList(FrequencyList * fList) 
{
	printf("NumOfNodes: %d\n", fList->numOfNodes);
	Node * temp = fList->head;
	while(temp != 0){
		printf("token: %s frequency: %d filename: %s \n", temp->token, temp->frequency, temp->filename);
		temp = temp->next;
	}
}

/*USED by jsonWriter.... can wipew fList so be careful*/
Json * removeFromHead(FrequencyList * fList)
{
	Node * temp = fList->head;
	if(temp != 0){
		fList->head = temp->next;
		if(fList->head != 0){
			fList->head->prev = temp;
		}
		Json * returnVal = malloc(sizeof(Json));
		returnVal->filename = temp->filename;
		returnVal->token = temp->token;
		returnVal->frequency = temp->frequency;
		returnVal->isTrailing = temp->isTrailing;
		fList->numOfNodes--;
		free(temp);
		return returnVal;
	}
}

/*deletes list*/
int deleteList (FrequencyList * fList) 
{
	Node * temp = fList->head; 
	while(temp != 0) {
		fList->head = fList->head->next;
//	    printf("Freeing : %s\n",temp->token );

		free(temp->token);
		free(temp);
		temp = fList->head;
	}
	free(fList);
	return 1;
}

//Returns if empty
int isEmpty(FrequencyList * fList) {
	if(fList->numOfNodes > 0){
		return 1;
	}
	return 0;
}

//destroys JSON tokens used by jsonWriter
int destroyJson (Json * json)
{
	free(json->token);
	free(json);
}
