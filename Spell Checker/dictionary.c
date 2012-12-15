/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

//Define a struct of node
typedef struct node
{
    char word[LENGTH+1];
    struct node* next;
}
node;

//Define the node for the hashtable
node* hash_table[26];

//Define the hashfunction to point to the array according to the first letter
int hash_function(const char* word)
{
    return word[0] - 'a'; 
} 

//Define a counter for the amount of nodes you add
int number_of_nodes = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    //Define temporary storage
    char temp_word[LENGTH+1] = {'\0'};

    //Copy the word to the temporary storage
    strcpy(temp_word, word);
    
    //Set the word to lowercase
    for(int i = 0; word[i] != '\0'; i++)
        temp_word[i] = tolower(word[i]);
      
    //Define the array of the number
    int temp_array = hash_function(temp_word);
    
    //Point to proper hashtable
    node* temp_node = hash_table[temp_array];
    
    //Compare the temp_word to words in the hashtable
    while(temp_node != NULL)
        {
            //Check if the word is equivalent to the string in the hash, if so return true
            if(strcmp(temp_word, temp_node->word) == 0)
                return true;
                
            //Move onto the next node
            temp_node = temp_node->next;
        }
    
    //Otherwise return false
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    //Open Dictionary
    FILE* fp = fopen(dictionary, "r");
    
    //Check if dictionary exists
    if(dictionary == NULL)
        return false;
    
    //Define a random String
    char* string = malloc(sizeof(char)*(LENGTH+1));
    
    //Iterate over every word in the dictionary
    while (fscanf(fp, "%s", string) != EOF)
        {
            //Allocate new space for a node
            node* new_node = malloc(sizeof(node));
            
            //Copy the string into the array
            strcpy(new_node->word, string);
            
            //Saves the array number into an int
            int hash_number = hash_function(string);
  
            //Point from the new node to where the table was pointing
            new_node->next = hash_table[hash_number];
            
            //Point the table to where the new node is
            hash_table[hash_number] = new_node;
            
            //Add to the word counter
            number_of_nodes++;
        }
   
    //Prevent memory leaks!
    free(string);
    
    //Close Dictionary
    fclose(fp);
    
    //Finish
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    //Check if the file was loaded
    if(number_of_nodes != 0)
    
        //Simply return the counter of the number of nodes implemented
        return number_of_nodes;
        
    //If not loaded, it has zero words
    return 0;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    //Loop through the hash table array
    for(int i=0;i<27;i++)
        {
            //Define the node you are freeing
            node* temp_node_1 = hash_table[i];
            
            //Loop through each individual node to free them
            while(temp_node_1 != NULL)
            {
                //Define the next node following the node you are freeing
                node* temp_node_2 = temp_node_1->next;
                
                //Free the first node in the loop
                free(temp_node_1);
                
                //Point the initial node to the next node
                temp_node_1 = temp_node_2;
            }
        } 
   
    return true;
}
