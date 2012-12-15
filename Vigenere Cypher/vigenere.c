/*********************************************************************
* vigenere.c
* Alexander Bukhta
* Problem Set 2
*
* This program will take a keyword that is entered, convert each of the letters into an integer
* and then encipher another entered phrase by using the caesar cipher but changing the "amount
* changed by" by the integers extracted from each of the letters in a cycle.
*
*********************************************************************/

//obvious libraries are obvious
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, string argv[]) 
{
    while (argc != 2) // Accepts a single argument (k), if user tries something else, it terminates
    {
        printf("Please retry with a single argument\n");
        return 1;
    }
    
    //store the keyword as the string "key"
    string key = argv[1]; 

    //converts all of the keyword into lowercase, since the alphabetical order doesn't change, though ascii does
    for(int i = 0; key[i]; i++)
    { 
        key[i] = tolower(key[i]);
    }

    //asks for and accepts the entered phrase
    printf("Please enter the phrase you want ciphered: "); 
    string p = GetString(); 
    
    //Define some necessary variables
    //this variablesis the number in the order of the array
    int j = 0; 
    
    //this becomes the shift in numbers of the key
    int k = 0; 
    
    //this will be used to convert the array into a char
    char x; 
    
    //this counts the total length of the entered keyword
    int length = strlen(argv[1]); 
    
    
    //This cycles through each of the characters and enciphers them
    for (int i = 0, n = strlen(p); i < n; i++) 
    {
        //Enciphers uppercase letters
        if(p[i] >= 65 && p[i] <= 90) 
        {
            //stores the array as a char, which in turn allows it to be used as an ascii number 
            x = key[j];  
            
            //turns the array into the number of shifts needed
            k = x - 'a'; 
            
            //added to orginial cipher in order to add cycling funcionality for keyword
            p[i] = (((p[i] - 'A' + k) % 26) + 'A'); 
            
            //iterates to the next array
            j++; 
            
            //makes sure j doesnt become greater than the length of the keyword provided
            j = j % length; 
        }
        
        //Enciphers lowercase letters
        else if(p[i] >= 97 && p[i] <= 122) 
        {
            //please refer to the comments used for uppercase
            x = key[j];
            k = x - 'a';
            p[i] = (((p[i] - 'a' + k) % 26) + 'a');
            j++; 
            j = j % length;
            
        }
        
        //leaves all the other keys unchanged
        else 
        {
            p[i] = p[i];
        }
        
        //prints out the enciphered code
        printf("%c", p[i]); 
    }  
    printf("\n");
return 0;
}
