/*********************************************************************
* caesar.c
* Alexander Bukhta
* Problem Set 2
*
* This program will take an input of k (the amount you want to shift
* your phrase by) and then it will shift each character in the phrase
* the user enters by that amount k. Cipher encoding! Go!
*
*********************************************************************/

//obvious libraries are obvious
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, string argv[]) 
{
    // Accepts a single argument (k), if user tries something else, it terminates
    while (argc != 2) 
    {
        printf("Please retry with a single argument\n");
        return 1;
    }

    // Converts the argument into an integer
    int k = atoi(argv[1]); 

    // Accepts the phrase that was entered and stores it as p
    printf("Please enter the phrase you want ciphered: "); 
    string p = GetString(); 
    
    // Cycles through each of the characters and enciphers them
    for (int i = 0, n = strlen(p); i < n; i++) 
    {
        // Enciphers Uppercase Letters
        if(p[i] >= 65 && p[i] <= 90) 
        {
            p[i] = (((p[i] - 'A' + k) % 26) + 'A'); 
        }
        
        // Enciphers Lowercase Letters
        if(p[i] >= 97 && p[i] <= 122) 
        {
            p[i] = (((p[i] - 'a' + k) % 26) + 'a'); 
        }
        
        // Leaves all other keys unchanged
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
