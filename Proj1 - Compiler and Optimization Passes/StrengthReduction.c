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
#include <math.h>
#include "InstrUtils.h"
#include "Utils.h"

/*
* Will return log2(n) if it is an integer (meaning n is a power of 2), and -1 otherwise
*/
int int_log2(int n) {

	int s = 1, i = 0;

	while (s < n) {
		s = s << 1;
		i++;
	}

	if (s == n)
		return i;
	else
		return -1;

}

/*
* Will traverse instructions in groups of two, searching for a patten of LOADI and MUL | DIV instructions 
* where LOADI is a power of 2, and collapse these instructions into on shift operation of LSHIFTI | RSHIFTI
*/
Instruction *strength_reduction(Instruction *head) {

		/* STUDENTS - BEGIN */
		Instruction *first; 
		Instruction *second; 
		Instruction *third;
		Instruction *temp = head; 
		int i;

		while(temp != NULL && temp->next != NULL && temp->next->next != NULL){
			first = temp; 
			second = first->next;
			third = second ->next;
			if(first->opcode == LOADI || second->opcode == LOADI){
				if(third->opcode == DIV || second->opcode == DIV ){
					if(second->opcode == LOADI && second->field2 == third->field2){
						i = int_log2(second->field1); 
						if(i != -1){
							third->opcode = RSHIFTI; 
							third->field2 = i; 
						}
					}
					if(first->opcode == LOADI){
						if(second->opcode == DIV && first->field2 == second->field2){
							i = int_log2(first->field1); 
							if(i != -1){
								second->opcode = RSHIFTI; 
								second->field2 = i;  
							}
						}
						if(third->opcode == DIV && first->field2 == third->field2){
							i = int_log2(first->field1); 
							if(i != -1){
								third->opcode = RSHIFTI; 
								third->field2 = i;
							}
						}
					}
				}
				if(third->opcode == MUL || second->opcode == MUL){
					if(second->opcode == LOADI && (second->field2 == third->field2 || second->field2 == third->field1) ) {
						i = int_log2(second->field1); 
						if(i != -1){
							third->opcode = LSHIFTI; 
							third->field2 = i; 
						}
					}
					if (first->opcode == LOADI){
						if(second->opcode == MUL && (first->field2 == second->field2 || first->field2 == second->field1)){
								i = int_log2(first->field1); 
								if(i != -1){
									second->opcode = LSHIFTI; 
									if(first->field2 == second->field1){
										second->field1 = second->field2; 
									}
									second->field2 = i;  
								}
						}
						if (third->opcode == MUL && (first->field2 == third->field2 || first->field2 == third->field1)){
							i = int_log2(first->field1); 
							if(i != -1){
								third->opcode = LSHIFTI; 
								if(first->field2 == third->field1){
										third->field1 = third->field2; 
								} 
								third->field2 = i; 
							}
						}
					}
				}
			}
			temp = temp->next; 
		}
 		/* STUDENTS - END */
	return head;
}

int main(){
	Instruction *head;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	head = strength_reduction(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

