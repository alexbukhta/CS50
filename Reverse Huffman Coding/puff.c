/*************************************************************************************
* puff.c
* Alexander Bukhta
* Computer Science 50
* Pset 6
*
* Puffs a huffed file, restoring it to its original form
*************************************************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "forest.h"
#include "huffile.h"
#include "tree.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: %s input output\n", argv[0]);
        return 1;
    }

    // create the output file
    FILE* output = fopen(argv[2], "w");

    //check that file was created
    if (output == NULL)
    {
        printf("File could not be created!");
        fclose(output);
        return 1;
    }

    // open input
    Huffile* input = hfopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open %s for reading.\n", argv[1]);
        hfclose(input);
        fclose(output);
        return 1;
    }

    // read in header
    Huffeader header;
    if (hread(&header, input) == false)
    {
        hfclose(input);
        printf("Could not read header.\n");
        fclose(output);
        return 1;
    }

    // check for magic number
    if (header.magic != MAGIC)
    {
        hfclose(input);
        printf("File was not huffed.\n");
        fclose(output);
        return 1;
    }

    // check checksum

    int checksum = header.checksum;
    for (int i = 0; i < SYMBOLS; i++)
    {
        checksum -= header.frequencies[i];
    }
    if (checksum != 0)
    {
        hfclose(input);
        printf("File was not huffed.\n");
        fclose(output);
        return 1;
    }

    //Define Forest!
    Forest* puff_forest = mkforest();

    //Define Tree!
    Tree* puff_tree;

    //Run through the header and scan for non-0 frequency symbols and plant them
    for (int i = 0; i < SYMBOLS; i++)
    {
        //Plant every frequency as its own tree
        if (header.frequencies[i] != 0)
        {
            //Allocate memory for the tree
            puff_tree = mktree();

            //Insert the frequency of the symbol
            puff_tree->frequency = header.frequencies[i];

            //Insert the actual symbol
            puff_tree->symbol = i;

            //Plant the tree in that memory
            plant(puff_forest, puff_tree);
        }
    }

    //Join the newly planted trees into a single tree
    while (puff_tree->frequency != header.checksum)
    {
        //Allocate memory for a new tree
        puff_tree = mktree();

        //Take the lowest frequency from the forest and plant it to the left struct of the new tree
        puff_tree->left = pick(puff_forest);

        //Take the new lowest frequency from the forest and plant it to the right struct of the new tree
        puff_tree->right = pick(puff_forest);

        //Update the tree frequency
        puff_tree->frequency = puff_tree->left->frequency + puff_tree->right->frequency;

        //Plant this new tree back into the forest
        plant(puff_forest, puff_tree);
    }

    //Define the bit
    int bit;

    //Define the new single tree
    Tree* main_tree = puff_forest->first->tree;

    //Write all the bits!
    while ((bit = bread(input)) != EOF)
    {
        //Point to the left if the bit is 0
        if (bit == 0)
        {
            main_tree = main_tree->left;
        }

        //Point to the right if the bit is 1
        else if (bit == 1)
        {
            main_tree = main_tree->right;
        }

        //If it cannot point left or right
        if (main_tree->left == NULL && main_tree->right == NULL)
        {
            //Write the character to the file
            fwrite(&main_tree->symbol, sizeof(char), 1, output);

            //Return the beginning of the tree
            main_tree = puff_forest->first->tree;
        }
    }

    // Close forest (and consequently trees)
    rmforest(puff_forest);

    //Close File I/Os
    hfclose(input);
    fclose(output);

    printf("Success!\n");
    return 0;
}
