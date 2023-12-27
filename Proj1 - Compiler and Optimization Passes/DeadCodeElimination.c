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

Instruction *dead_code_elimination(Instruction *head) {
   
    /* STUDENTS - BEGIN */
    Instruction *last;

	//Gets the last element of head
	Instruction *temp; 
	temp = head; 
	while(temp != NULL){
		last = temp;
		temp = temp->next; 
	}

	//Used for traversing the array in the while loop
	Instruction *temp2 = last; 
	Instruction *first; 

	int reg[100]; 
	for(int i = 0; i < sizeof(reg)/sizeof(reg[0]); i++){
		reg[i] = -1; 
	}
	int offset[100]; 
	for(int i = 0; i < sizeof(offset)/sizeof(offset[0]); i++){
		offset[i] = -1; 
	}

	while(temp2 != NULL && temp2->prev != NULL && temp2->prev->prev != NULL){
		first = last;
		if(first->opcode == OUTPUTAI){
			first->critical = 1; 
			for(int i = 0; i < sizeof(offset)/sizeof(offset[0]); i++){
				if(offset[i] == -1){
					offset[i] = first->field2; 
					break; 
				}
			}
		}
		if(first->opcode == STOREAI){
			for(int i = 0; i < sizeof(offset)/sizeof(offset[0]); i++){
				if(first->field3 == offset[i]){
					first->critical = 1; 
					offset[i] = -1; 
					break; 
				}
			}
			if(first->critical == 1){
				for(int j = 0; j < sizeof(reg)/sizeof(reg[0]); j++){
					if(reg[j] == -1){
						reg[j] = first->field1; 
						break; 
					}
				}
			}
		}
		else if(first->opcode == MUL || first->opcode == DIV){
			for(int i = 0; i < sizeof(reg)/sizeof(reg[0]); i++){
				if(first->field3 == reg[i]){
					first->critical = 1;
					reg[i] = -1; 
					break; 
				}
			}
			if(first->critical == 1){
				for(int j = 0; j < sizeof(reg)/sizeof(reg[0]); j++){
					if(reg[j] == -1 && reg[j+1] == -1){
						reg[j] = first->field1; 
						reg[j+1] = first->field2; 
						break; 
					} 
				}
			}
		}
		else if( (first->opcode == SUB || first->opcode == ADD)){
			for(int i = 0; i < sizeof(reg)/sizeof(reg[0]); i++){
				if(first->field3 == reg[i]){
					first->critical = 1;
					reg[i] = -1; 
					break; 
				}
			}
			if(first->critical == 1){
				for(int j = 0; j < sizeof(reg)/sizeof(reg[0]); j++){
					if(reg[j] == -1 && reg[j+1] == -1){
						reg[j] = first->field1; 
						reg[j+1] = first->field2; 
						break; 
					} 
				}
			}
		}
		else if(first->opcode == LSHIFTI || first->opcode == RSHIFTI) {
			for(int i = 0; i < sizeof(reg)/sizeof(reg[0]); i++){
				if(first->field3 == reg[i]){
					first->critical = 1; 
					reg[i] = -1; 
					break; 
				}
			}
			if(first->critical == 1){
				for(int i = 0; i < sizeof(reg)/sizeof(reg[0]);i++){
					if(reg[i] == -1){
						reg[i] = first->field1; 
						break; 
					}
				}
			}
		}
		else if(first->opcode == LOADI && first->field1 != 1024){
			for(int i = 0; i < sizeof(reg)/sizeof(reg[0]); i++){
				if(first->field2 == reg[i]){
					first->critical = 1;
					reg[i] = -1; 
					break; 
				}
			}
		}
		else if(first->opcode == LOADAI){
			for(int i = 0; i < sizeof(reg)/sizeof(reg[0]); i++){
				if(first->field3 == reg[i]){
					first->critical = 1; 
					reg[i] = -1; 
					break; 
				}
			}
			if(first->critical == 1){
				for(int j = 0; j < sizeof(offset)/sizeof(offset[0]); j++){
					if(offset[j] == -1){
						offset[j] = first->field2; 
						break; 
					}
				}
			}
		}
		else if(first->opcode == LOADI && first->field1 == 1024 && first->field2 == 0){
			first->critical = 1; 
			break; 
		}
		last = last->prev; 
	}

	Instruction *my_head; 
	my_head = head;
	Instruction *temp1;

	while(my_head != NULL) {
		if(my_head->critical != 1) {
			temp1 = my_head->next;
			my_head->next->prev = my_head->prev;
			my_head->prev->next = my_head->next;
			free(my_head);		
			my_head = temp1;
			continue;
		}
		my_head = my_head->next;
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

	head = dead_code_elimination(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

