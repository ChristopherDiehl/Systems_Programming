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
	//3 cases. 1)first node null 2)add to end of list 3)add to beginning 4)add to middle
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

	if(fList->head == 0){
		fList->head = temp;
		fList->tail = temp;
		fList->numOfNodes++;
		return 1;
	} 
	Node * currNode = fList->head;
	while(currNode != 0){
		if(strcmp(currNode->token,token) == 0){
			if(strcmp(currNode->filename, filename) == 0 ){
				currNode->frequency++;
				free(temp);
				return 1;
			} else if(strcmp(filename, currNode->filename) > 0) {
				temp->next = currNode;
				if(currNode->prev != 0){
					temp->prev = currNode->prev;
					temp->prev->next = temp;
				} else{
					fList->head = temp;
				}
				currNode->prev = temp;
				fList->numOfNodes++;
				return 1;
			}
		} else if(strcmp(token,currNode->token) < 0){
			//currNode is greater than prev Node
			temp->next = currNode;
			if(currNode->prev != 0){
				temp->prev = currNode->prev;
				temp->prev->next = temp;
			} else{
				fList->head = temp;
			}
			currNode->prev = temp;
			fList->numOfNodes++;
			return 1;
		} 
		currNode = currNode->next;
	}
	//if it makes it out of loop then append to end of list because token is larger then every element in list
	fList->tail->next = temp;
	temp->prev = fList->tail;
	fList->tail = temp;
	return 1;

}

void printList(FrequencyList * fList) {
	Node * temp = fList->head;
	while(temp != 0){
		printf("token: %s frequency: %d filename: %s \n", temp->token, temp->frequency, temp->filename);
		temp = temp->next;
	}
}

char * removeFromHead(FrequencyList * fList){
	Node * temp = fList->head;
	if(temp != 0){
		fList->head = temp->next;
		if(fList->head != 0){
			fList->head->prev = temp;
		}
		char * tempStr = temp->token;
		free(temp);
		return tempStr;
	}
}
int deleteList (FrequencyList * fList) {
	Node * temp = fList->head; 
	while(temp != 0) {
		fList->head = fList->head->next;
		free(temp->token);
		free(temp);
		temp = fList->head;
	}
	free(fList);
	return 1;
}