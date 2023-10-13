/*!\file
*/

#ifndef STACK_FUNC_H_INCLUDED
#define STACK_FUNC_H_INCLUDED

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "stack_struct.h"
	#include "errors_enum_naming.h"
	
	

	/**
     * Creates(allocates memory) data(massif with numbers which we push and pop from stack).
     *
     * @param [in] *error  Pointer to error. At the end of the program 0 if all is normal, NOT_MEMORY if we have not enough memory, 
     * BAD_POINTER if stk == NULL.
     *
     * @returns Pointer to stack
     */
	Stack* stack_ctor(errors* error);

	/**
     * Puts poison in the data, free data's memory. Puts poison in the pointer to data, in the current_size of data, in the capacity.
     * DO NOT free stk
     * 
     * @param [in] stk Pointer to stack
     *
     * @returns NULL if stack pointer already consists of NULL else it returns BAD_PTR.
     */
    Stack* stack_dtor(Stack* stk);

	/**
     * Puts a new value in the massif data, if it needs then allocates new memory. Adds 1 to the current_size in stack, changes 
     * capacity if we have allocated new memory.
     *
     * @param [in] stk Pointer to stack
     * @param [in] value Value we need to push in stack
     *
     * @returns ALL_OK if all is normal, NOT_MEMORY if we have not enough memory, BAD_STACK_POINTER if stk consists of NULL or 
     * BAD_PTR(which is written in constants), BAD_DATA_POINTER if stk->data consists of NULL or BAD_PTR.
     */
    int stack_push(Stack* stk, elem_type value);

    /**
     * Deletes last value in the massif data if it it needs then allocates new memory. Subs 1 from the current_size in stack, 
     * changes capacity if we have allocated new memory.
     *
     * @param [in] stk Pointer to stack
     * @param [in] error Pointer to error. At the end it consists of the ALL_OK if all is normal, NOT_MEMORY if something
     * went wrong while allocating memory.
     *
     * @returns Elem which has been poped 
     */
    elem_type stack_pop(Stack* stk, errors* error);


    // /**
    //  * Checks all in stack.
    //  *
    //  * @param [in] stk Pointer to stack
    //  *
    //  * @returns ALL_OK if all is normal, code of the error if something is wrong.
    //  */
    // errors stack_ok(Stack* stk);   //enum errors stack_ok

    /**
     * Checks all in stack.
     *
     * @param [in] stk Pointer to stack
     *
     * @returns ALL_OK(0) if all is normal, mask of errors if something is wrong.
     */
    size_t stack_ok(Stack* stk);   //enum errors stack_ok


     /**
     * Prints all infornation about the stack in file_with_errors.
     *
     * @param [in] stk Pointer to stack
     *
     * @returns ALL_OK if all is normal, BAD_STACK_POINTER if stk consists of NULL or BAD_PTR(which is written in constants), 
     * BAD_FILE_POINTER if stk->file_with_errors consists of NULL.
     */
    int stack_dump(Stack* stk, errors reason);


    /**
     * @param [in] stk Pointer to stack
     * @param [in] num Index of element we need in Stack data (numeration from 0)
     * 
     * @returns pointer to element with number num in Stack data (numeration from 0) or NULL if stk or stk-data consists of NULL or 
     * BAD_PTR(elem_type *const, defined in "stack_struct.h"), returns NULL also if num < 0
     */
	elem_type* get_data_elem_pointer(Stack* stk, int num);


	elem_type* change_capacity(Stack* stk, size_t new_capacity);

	/**
     * Counts massif data hash.
     *
     * @param [in] stk Pointer to stack
     *
     * @returns counted hash value.
     */
    long long stack_hash(Stack* stk, errors reason);
   
#endif