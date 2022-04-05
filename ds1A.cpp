/*
*FILE: dsA1.c
* PROGRAMMER: Kristian Biviens
* FIRST VERSION: 2021-02-26
* DESCRIPTION: This program will gather movie information from the user and display it in a neat fashion in two lists
* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable : 4996)
#pragma warning(disable : 6001)

struct MovieInfo
{
	char* genre;
	char* title;
	int rating;
};

typedef struct MovieNode
{
	struct MovieInfo movieData;
	struct MovieNode* previous;
	struct MovieNode* next;
} MovieNode;

void getMovieInfo(struct MovieInfo* movieInfo, char* genre, char* title, int rating);
void printMovieInfo(MovieNode* head);
void clearCR(char* buf);
int getNum(void);
void insertbyRating(MovieNode* newNode, MovieNode** tail, MovieNode** head);
void insertbyGenre(MovieNode* newNode, MovieNode** tail, MovieNode** head);
void deleteNode(MovieNode* node, MovieNode** head, MovieNode** tail);
MovieNode* findMovie(MovieNode* head, char* genre, char* title);

int main()
{
	MovieNode* newnodeRating = NULL;
	MovieNode* nodeheadRating = NULL;
	MovieNode* nodetailRating = NULL;
	MovieNode* newnodeGenre = NULL;
	MovieNode* nodeheadGenre = NULL;
	MovieNode* nodetailGenre = NULL;
	MovieNode* genreStorage = NULL;
	MovieNode* ratingStorage = NULL;
	MovieNode* reinsertStorage = NULL;

	int totalCharacters = 30;
	char inputGenre[30] = "";
	char inputTitle[30] = "";
	int inputRating = 0;


	for (;;)
	{

		printf("Please input a genre. Press '.' to print:\n");
		fgets(inputGenre, totalCharacters, stdin);
		clearCR(inputGenre);

		if (inputGenre[0] == '.')
		{
			printMovieInfo(nodeheadRating);
			printf("\n");
			printMovieInfo(nodeheadGenre);
			printf("\n");
			break;
		}

		printf("Please input a title. Press '.' to print:\n");
		fgets(inputTitle, totalCharacters, stdin);
		clearCR(inputTitle);

		if (inputTitle[0] == '.')
		{
			printMovieInfo(nodeheadRating);
			printf("\n");
			printMovieInfo(nodeheadGenre);
			printf("\n");
			break;
		}

		printf("Please input a rating.\n");
		inputRating = getNum();
		if (inputRating < 1 || inputRating > 5)
		{
			printf("Error, invalid rating.\n");
			return 0;
		}

		newnodeGenre = (MovieNode*)malloc(sizeof(MovieNode));
		newnodeRating = (MovieNode*)malloc(sizeof(MovieNode));

		if (newnodeGenre == NULL || newnodeRating == NULL)
		{
			printf("Not enough memory!\n");
			return 0;
		}

		getMovieInfo(&(newnodeGenre->movieData), inputGenre, inputTitle, inputRating);
		getMovieInfo(&(newnodeRating->movieData), inputGenre, inputTitle, inputRating);

		insertbyGenre(newnodeGenre, &nodetailGenre, &nodeheadGenre);
		insertbyRating(newnodeRating, &nodetailRating, &nodeheadRating);
	}
	printf("Please input one genre and title pair.\n");
	fgets(inputGenre, totalCharacters, stdin);
	fgets(inputTitle, totalCharacters, stdin);
	clearCR(inputGenre);
	clearCR(inputTitle);

	genreStorage = findMovie(nodeheadGenre, inputGenre, inputTitle);

	if (genreStorage != NULL)
	{
		printf("Rating: %d\n", genreStorage->movieData.rating);
		printf("Please input a new rating.\n");
		inputRating = getNum();
		if (inputRating < 1 || inputRating > 5)
		{
			printf("Error, invalid rating.\n");
			return 0;
		}
		else if (inputRating != genreStorage->movieData.rating)
		{
			genreStorage->movieData.rating = inputRating;

			ratingStorage = findMovie(nodeheadRating, inputGenre, inputTitle);

			deleteNode(ratingStorage, &nodeheadRating, &nodetailRating);

			reinsertStorage = (MovieNode*)malloc(sizeof(MovieNode));

			if (reinsertStorage == NULL)
			{
				printf("Not enough memory!\n");
				return 0;
			}

			getMovieInfo(&(reinsertStorage->movieData), inputGenre, inputTitle, inputRating);

			insertbyRating(reinsertStorage, &nodetailRating, &nodeheadRating);


		}

	}

	printMovieInfo(nodeheadRating);

	printf("\n");

	printMovieInfo(nodeheadGenre);

	MovieNode* currentPointer = nodeheadRating;
	MovieNode* newPointer = NULL;

	while (currentPointer != NULL)
	{
		newPointer = currentPointer;
		currentPointer = currentPointer->next;
		free(newPointer->movieData.genre);
		free(newPointer->movieData.title);
		free(newPointer);
	}

	currentPointer = nodeheadGenre;
	newPointer = NULL;

	while (currentPointer != NULL)
	{
		newPointer = currentPointer;
		currentPointer = currentPointer->next;
		free(newPointer->movieData.genre);
		free(newPointer->movieData.title);
		free(newPointer);
	}

	return 0;
}


/*
*Function:getMovieInfo()
* Description: This function will fill the struct fields
* Parameters: movieInfo, genre, title, rating
* Returns: void
*/
void getMovieInfo(struct MovieInfo* movieInfo, char* genre, char* title, int rating)
{

	movieInfo->genre = (char*)malloc((strlen(genre) + 1) * sizeof(char));
	movieInfo->title = (char*)malloc((strlen(title) + 1) * sizeof(char));
	movieInfo->rating = rating;


	if (movieInfo->genre == NULL || movieInfo->title == NULL)
	{
		printf("Not enough memory!\n");
		return;
	}

	strcpy(movieInfo->genre, genre);
	strcpy(movieInfo->title, title);
}

/*
*Function:printMovieInfo()
* Description: This function will print the movie information into a neat list
* Parameters: MovieNode* nodeHead
* Returns: void
*
*/
void printMovieInfo(MovieNode* nodeHead)
{
	MovieNode* current = nodeHead;

	while (current != NULL)
	{
		printf("%-35s%-35s%d\n", current->movieData.genre, current->movieData.title, current->movieData.rating);
		current = current->next;
	}
}


/*
*Function:findMovie()
* Description: This function will find a movie within a node.
* Parameters: MovieNode* head, char* genre, char* title
* Returns: MovieNode*
*
*/
MovieNode* findMovie(MovieNode* head, char* genre, char* title)
{
	MovieNode* current = head;

	while (current != NULL)
	{
		if (strcmp(current->movieData.genre, genre) == 0 && strcmp(current->movieData.title, title) == 0)
		{
			return current;
		}
		else
		{
			current = current->next;
		}
	}
	return NULL;
}

/*
*Function:deleteNode()
* Description: This function will find and delete a node!
* Parameters: MovieNode* node, MovieNode** head, MovieNode** tail
* Returns: void
*/
void deleteNode(MovieNode* node, MovieNode** head, MovieNode** tail)
{

	if (node == NULL || *head == NULL)
	{
		return;
	}
	else if (*head == *tail && *head != NULL && *head == node)
	{
		*head = *tail = NULL;
	}
	else if (*head == node)
	{
		*head = (*head)->next;
		(*head)->previous = NULL;
	}
	else if (*tail == node)
	{
		*tail = (*tail)->previous;
		(*tail)->next = NULL;
	}
	else
	{
		MovieNode* current = *head;
		while (node != current)
		{
			current = current->next;
			if (current == NULL)
			{
				return;
			}

		}
		current->next->previous = current->previous;
		current->previous->next = current->next;

	}
	free(node->movieData.genre);
	free(node->movieData.title);
	free(node);
}

/*
*Function:getNum()
* Description: This function will allow the user to input a number into the program
* Parameters: None
* Returns: The number the user inputs
*/
#pragma warning(disable: 4996)
int getNum(void)
{
	char record[121] = { 0 };
	int number = 0;

	fgets(record, 121, stdin);

	if (sscanf(record, "%d", &number) != 1)
	{
		number = -1;
	}
	return number;
}

/*
*Function:insertbyRating()
* Description: This function will insert user input into nodes
* Parameters: MovieNode* newNode, MovieNode** tail, MovieNode** head
* Returns: void
*/
void insertbyRating(MovieNode* newNode, MovieNode** tail, MovieNode** head)
{

	if (*head == NULL)  // Only one node so we set it as the head
	{
		*head = newNode;
		*tail = newNode;
		newNode->previous = NULL;
		newNode->next = NULL;
	}
	else if (newNode->movieData.rating >= (*tail)->movieData.rating) // The new node would go at the end of the list here
	{
		(*tail)->next = newNode;								     // Tail next pointer will point to the new node		
		newNode->previous = *tail;								 // The new node previous pointer will point to the tail
		*tail = newNode;											 // The new node will become the tail 
		(*tail)->next = NULL;
	}
	else if (newNode->movieData.rating < (*head)->movieData.rating) // The new node would go to the beginning of the list here 
	{
		(*head)->previous = newNode;
		newNode->next = *head;
		*head = newNode;
		(*head)->previous = NULL;
	}
	else
	{
		MovieNode* current = *head;
		while (!(newNode->movieData.rating >= current->movieData.rating && newNode->movieData.rating < current->next->movieData.rating)) // The new node would go somewhere in between the head and the tail here
		{
			current = current->next;
		}
		newNode->previous = current;
		current->next->previous = newNode;
		newNode->next = current->next;
		current->next = newNode;
	}

}

/*
*Function:insertbyGenre()
* Description: This function will insert user input into nodes
* Parameters: MovieNode* newNode, MovieNode** tail, MovieNode** head
* Returns: void
*/
void insertbyGenre(MovieNode* newNode, MovieNode** tail, MovieNode** head)
{


	if (*head == NULL)  // Only one node so we set it as the head
	{
		*head = newNode;
		*tail = newNode;
		newNode->previous = NULL;
		newNode->next = NULL;
	}
	else if (strcmp(newNode->movieData.genre, (*tail)->movieData.genre) >= 0)  // The new node would go at the end of the list here
	{
		(*tail)->next = newNode;								     // Tail next pointer will point to the new node		
		newNode->previous = *tail;								 // The new node previous pointer will point to the tail
		*tail = newNode;											 // The new node will become the tail 
		(*tail)->next = NULL;
	}
	else if (strcmp(newNode->movieData.genre, (*head)->movieData.genre) < 0)  // The new node would go to the beginning of the list here 
	{
		(*head)->previous = newNode;
		newNode->next = *head;
		*head = newNode;
		(*head)->previous = NULL;
	}
	else
	{
		MovieNode* current = *head;
		while (!(strcmp(newNode->movieData.genre, current->movieData.genre) >= 0 && (strcmp(newNode->movieData.genre, current->next->movieData.genre) < 0)))  // The new node would go somewhere in between the head and the tail here
		{
			current = current->next;
		}
		newNode->previous = current;
		current->next->previous = newNode;
		newNode->next = current->next;
		current->next = newNode;
	}

}


/*
*Function: clearCRLF()
* Description: This function will get rid of a \n at the end of a string.
*	           If there isn't one, it doesn't change the string.
* Parameters: char *str: start of the string
* Returns: void
*/
void clearCR(char* buf)
{
	char* whereCR = strchr(buf, '\n');
	if (whereCR != NULL)
	{
		*whereCR = '\0';
	}
}

