/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2023                              *
 *  Student Version                          *
 *  Author: Leah Ranavat                     *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

/*
* Will traverse instructions in groups of 3, searching for a patten of LOADI, LOADI, and ADD | SUB | MUL instructions 
*/

Instruction *constant_folding(Instruction *head) {

	/* STUDENTS - BEGIN */

    Instruction *first;
    Instruction *second;
    Instruction *third;
    Instruction *temp = head;
    int c1, c2, product; 

    while(temp != NULL && temp->next != NULL && temp->next->next != NULL){
        first = temp;
        second = first->next;
        third = second->next;

        if(first->opcode == LOADI && second->opcode == LOADI){
            if(third->opcode == ADD){
                if ((first->field2 == third->field1 && second->field2 == third->field2) || (first->field2 == third->field2 && second->field2 == third->field1)) {
                    c1 = first->field1;
                    c2 = second->field1;
                    product = c1 + c2;
                    third->opcode = LOADI; 
                    third->field1 = product; 
                    third->field2 = third->field3; 
                    third->field3 = 0;
                }
            }
            if(third->opcode == SUB){
                if (first->field2 == third->field1 && second->field2 == third->field2) {
                    c1 = first->field1;
                    c2 = second->field1;
                    product = c1 - c2;
                    third->opcode = LOADI; 
                    third->field1 = product; 
                    third->field2 = third->field3; 
                    third->field3 = 0;
                }
            }
            if(third->opcode ==  MUL){
                if ((first->field2 == third->field1 && second->field2 == third->field2) || (first->field2 == third->field2 && second->field2 == third->field1)) {
                    c1 = first->field1;
                    c2 = second->field1;
                    product = c1 * c2;
                    third->opcode = LOADI; 
                    third->field1 = product; 
                    third->field2 = third->field3; 
                    third->field3 = 0;
                }
            }
        } 
        temp = temp->next;
    }
    /* STUDENTS - END */

	return head; 
}

int main()
{
	Instruction *head;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	head = constant_folding(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

