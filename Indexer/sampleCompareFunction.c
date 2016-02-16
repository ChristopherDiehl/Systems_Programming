//sample compareInt function
compareint(void * num0, void *num1) {
	int *compare0 = (int*)num0;
	int *compare1 = (int*)num1;
	if(* compare1 > *compare0)
		return 1;
	if (*compare1 < *compare0)
		return -1;
	return 0;
}

void intDestructFunc(void * thingToDestroy){
	free(thingToDestroy);
	return;
}

