/*!\file
*/

#include "stack_func.h"

/**
    * @param [in] stk Pointer to stack
    * 
    * @returns pointer to the begin canary in Stack data or NULL if stk or stk-data consists of NULL or BAD_PTR(elem_type *const, 
    *       defined in "stack_struct.h")
*/
static canary_type* get_begin_canary_pointer(Stack* stk);

/**
    * @param [in] stk Pointer to stack
    * 
    * @returns pointer to the end canary in Stack data or NULL if stk or stk-data consists of NULL or BAD_PTR(elem_type *const, 
    *       defined in "stack_struct.h")
*/
static canary_type* get_end_canary_pointer(Stack* stk);


static canary_type* get_begin_canary_pointer(Stack* stk)
{
	if(!stk || stk == (Stack*)BAD_PTR || !(stk->data) || stk->data == BAD_PTR)
		return NULL;

	return (canary_type*)stk->data;
}

static canary_type* get_end_canary_pointer(Stack* stk)
{
	if(!stk || stk == (Stack*)BAD_PTR || !(stk->data) || stk->data == BAD_PTR)
		return NULL;
    
    //printf("%p %p %p %p\n", stk->data, (char*)stk->data + sizeof(canary_type), (char*)stk->data + sizeof(canary_type) + sizeof(elem_type)*stk->capacity,
    //(canary_type*)((char*)stk->data + sizeof(canary_type) + sizeof(elem_type)*stk->capacity + stk->gap_after_begin_canary + stk->gap_before_end_canary));
    return (canary_type*)((char*)stk->data + sizeof(canary_type) + sizeof(elem_type)*(size_t)stk->capacity + stk->gap_after_begin_canary \
                            + stk->gap_before_end_canary);
}

elem_type* get_data_elem_pointer(Stack* stk, int num)
{
    if(num < 0)
        return NULL;

    if(!stk || stk == (Stack*)BAD_PTR || !(stk->data) || stk->data == BAD_PTR)
        return NULL;

    #ifdef DATA_USE_CANARY
        return (elem_type*)((char*)stk->data + sizeof(canary_type) + sizeof(elem_type)*(size_t)num + stk->gap_after_begin_canary);
    #else
        return (elem_type*)((char*)stk->data + sizeof(elem_type)*num);
    #endif //DATA_USE_CANARY
}



Stack* stack_ctor(errors* error)
{
    Stack* stk = (Stack*)calloc(1, sizeof(Stack));

	if(!stk)
    {
        *error = NOT_MEMORY;
		return NULL;
    }

    #ifdef STACK_USE_CANARY
    	stk->begin_canary = BEGIN_CANARY_VALUE;
    	stk->end_canary = END_CANARY_VALUE;
    #endif //STACK_USE_CANARY

	printf("%p\n", stk->data);
	
	#ifdef DATA_USE_CANARY
		printf("use\n");
        stk->gap_after_begin_canary = 0;
        stk->gap_before_end_canary = 0;

        if(sizeof(elem_type) > sizeof(canary_type))
            stk->gap_after_begin_canary = sizeof(elem_type) - sizeof(canary_type) % sizeof(elem_type);
        else
            stk->gap_before_end_canary = sizeof(canary_type) - (sizeof(elem_type) * (size_t)N_DATA_ELEMS_BEGIN) % sizeof(canary_type);

        printf("\nafter = %d before = %d\n", stk->gap_after_begin_canary, stk->gap_before_end_canary);

        size_t gap = (size_t)stk->gap_after_begin_canary + (size_t)stk->gap_before_end_canary; 
		stk->data = (elem_type*)calloc((size_t)N_DATA_ELEMS_BEGIN * sizeof(elem_type) + 2 * sizeof(canary_type) + gap, sizeof(char));
        //stk->data = (int*)calloc(N_DATA_ELEMS_BEGIN + 2, sizeof(int));

	#else
		printf("does not use\n");
		stk->data = (elem_type*)calloc(N_DATA_ELEMS_BEGIN, sizeof(elem_type));
        //stk->data = (int*)calloc(N_DATA_ELEMS_BEGIN, sizeof(int));

	#endif //DATA_USE_CANARY

	if(!stk->data)
	{
		*error = NOT_MEMORY;
        return stk;
	}

	stk->curr_size = 0;
	stk->capacity = N_DATA_ELEMS_BEGIN;
    stk->push_change = 2;
    stk->pop_change = 3;

	#ifdef DATA_USE_CANARY

		canary_type* beg_canary_ptr = get_begin_canary_pointer(stk);
		*(beg_canary_ptr) = BEGIN_CANARY_VALUE;

		canary_type* end_canary_ptr = get_end_canary_pointer(stk);
		//printf("%d\n", *end_canary_ptr);
		*(end_canary_ptr) = END_CANARY_VALUE;

		// printf(CANARY_SPECIFIER " " CANARY_SPECIFIER " %d %x\n", *get_begin_canary_pointer(stk), BEGIN_CANARY_VALUE, 0xBADDED, 0xBADDED);
		// printf(CANARY_SPECIFIER " " CANARY_SPECIFIER " %d %x\n", *get_end_canary_pointer(stk), END_CANARY_VALUE, 0xBADDED, 0xBADDED);

	#endif //DATA_USE_CANARY


    //CHECKSTACK(ALL_OK);


    return stk;

}


size_t stack_ok(Stack* stk)
{
    size_t error = ALL_OK;
    if(!stk || stk == (Stack*)BAD_PTR)
        error += 1 << abs(BAD_STACK_POINTER);

    #ifdef STACK_USE_CANARY
    if(stk->begin_canary != BEGIN_CANARY_VALUE)
        error += 1 << abs(WRONG_BEGIN_CANARY);

    if(stk->end_canary != END_CANARY_VALUE)
        error += 1 << abs(WRONG_END_CANARY);
    #endif // STACK_USE_CANARY

    if(stk->capacity < 0)
        error += 1 << abs(NEGATIVE_CAPASITY);

    //printf(",m,m = %d\n", stk->curr_size < 0);
    if(stk->curr_size < 0)
        error += 1 << abs(NEGATIVE_SIZE);

    if(stk->capacity < stk->curr_size)
        error += 1 << abs(CAP_SMALLER_SIZE);

    if(stk->data == BAD_PTR || !(stk->data))
        error += 1 << abs(BAD_DATA_POINTER);

    #ifdef DATA_USE_CANARY
        if(*get_begin_canary_pointer(stk) != BEGIN_CANARY_VALUE)
            error += 1 << abs(DATA_BEGIN_CANARY);

        if(stk->capacity > 0)
            if(*get_end_canary_pointer(stk) != END_CANARY_VALUE)
                error += 1 << abs(DATA_END_CANARY);
    #endif // DATA_USE_CANARY

    #ifdef STACK_USE_HASH
        long long previous_hash_value = stk->hash_value;
        long long hash_value = stack_hash(stk, ALL_OK);
        if(previous_hash_value != hash_value)
            error += 1 << abs(WRONG_HASH);
    #endif // STACK_USE_HASH

    return error;

}

/*errors stack_ok(Stack* stk)
{
    if(!stk || stk == (Stack*)BAD_PTR)
        return BAD_STACK_POINTER;

    #ifdef STACK_USE_CANARY
    if(stk->begin_canary != BEGIN_CANARY_VALUE)
        return WRONG_BEGIN_CANARY;

    if(stk->end_canary != END_CANARY_VALUE)
        return WRONG_END_CANARY;
    #endif // STACK_USE_CANARY

    if(stk->capacity < 0)
        return NEGATIVE_CAPASITY;

    //printf(",m,m = %d\n", stk->curr_size < 0);
    if(stk->curr_size < 0)
        return NEGATIVE_SIZE;

    if(stk->capacity < stk->curr_size)
        return CAP_SMALLER_SIZE;

    if(stk->data == BAD_PTR || !(stk->data))
        return BAD_DATA_POINTER;

    #ifdef DATA_USE_CANARY
        if(*get_begin_canary_pointer(stk) != BEGIN_CANARY_VALUE)
            return DATA_BEGIN_CANARY;

        if(*get_end_canary_pointer(stk) != END_CANARY_VALUE)
            return DATA_END_CANARY;
    #endif // DATA_USE_CANARY

    #ifdef STACK_USE_HASH
        previous_hash_value = hash_value;
        hash_value = stack_hash(stk, ALL_OK);
        if(previous_hash_value != hash_value)
            return WROMG_HASH;
    #endif // STACK_USE_HASH

    return ALL_OK;

}*/

int stack_dump(Stack* stk, errors reason)
{
    if(!stk || stk == (Stack*)BAD_PTR)
    {
        return BAD_STACK_POINTER;
    }

    if(!(stk->file_with_errors))
    {
        stk->file_with_errors = stderr;
        //return BAD_FILE_POINTER;
    }

    if(reason != 0)
    {
        //fprint_red(stk->file_with_errors, "Dump was called because %s(error = %d)\n", error_names[abs(reason)], reason);
        if(stk->file_with_errors != stderr && stk->file_with_errors != stdout)
        {
            fprintf(stk->file_with_errors, "Dump was called because %s(error = %d)\n", error_names[abs(reason)], reason);
        }
        else
        {
            fprintf(stk->file_with_errors, RED "Dump was called because %s(error = %d)" RST "\n", error_names[abs(reason)], reason);
        }
    }
        

    #ifdef STACK_USE_HASH
        long long previous_hash_value = stk->hash_value;
        long long hash_value = stack_hash(stk, ALL_OK);
        if(previous_hash_value == hash_value)
            fprintf(stk->file_with_errors, "Hash is ok\n");
        else
            fprintf(stk->file_with_errors, "Hash is not ok: previous hash was %lld, current hash is %lld\n", previous_hash_value, hash_value);
    #endif // STACK_USE_HASH

    fprintf(stk->file_with_errors, "Stack[%p]", stk);

    /*errors error = stack_ok(stk);
    if(error == ALL_OK)
        fprintf(stk->file_with_errors, "(ok)\n");
    else
        fprintf(stk->file_with_errors, "\nERROR = %d. This means: %s\n", error, error_names[abs(error)]);*/

    size_t error = stack_ok(stk);
    if(error == ALL_OK)
        fprintf(stk->file_with_errors, "(ok)\n");
    else
    {   
        size_t n_errors = 0;
        size_t error_mask = 1;
        size_t n_bit = 0;
        while(error)
        {
            if(error & error_mask)
            {
                if(!n_errors)
                    fprintf(stk->file_with_errors, "\n\n");
                if(stk->file_with_errors == stderr || stk->file_with_errors == stdin)
                    fprintf(stk->file_with_errors, RED "ERROR_%lu = -%lu. This means: %s" RST "\n", ++n_errors, n_bit, error_names[n_bit]);
                else
                    fprintf(stk->file_with_errors, "ERROR_%lu = -%lu. This means: %s\n", ++n_errors, n_bit, error_names[n_bit]);

                error -= error_mask;
            }

            ++n_bit;
            error_mask <<= 1;
        }

        fprintf(stk->file_with_errors, "\n");
    }


    fprintf(stk->file_with_errors, "{\n");

    #ifdef STACK_USE_CANARY
        fprintf(stk->file_with_errors, "   stack_begin_canary = " CANARY_SPECIFIER "\n", stk->begin_canary);
    #endif // STACK_USE_CANARY

    fprintf(stk->file_with_errors, "   capacity = %d\n   current size = %d\n", stk->capacity, stk->curr_size);

    fprintf(stk->file_with_errors, "\n   data[%p] = %p\n   {\n", &stk->data, stk->data);

    if(stk->data && stk->data != BAD_PTR)
    {
        #ifdef DATA_USE_CANARY
            fprintf(stk->file_with_errors, "      begin data canary[%p] = " CANARY_SPECIFIER "\n", get_begin_canary_pointer(stk), 
                                                                            *get_begin_canary_pointer(stk));

            for(int num = 0; num < stk->capacity; ++num)
            {
                if(num < stk->curr_size)
                    fprintf(stk->file_with_errors, "      *");
                else
                    fprintf(stk->file_with_errors, "      ");
                
                fprintf(stk->file_with_errors, "[%d](%p) = " ELEM_SPECIFIER "\n", num, get_data_elem_pointer(stk, (size_t)num), *get_data_elem_pointer(stk, num));
            }

            fprintf(stk->file_with_errors, "      end data canary[%p] = " CANARY_SPECIFIER "\n", get_end_canary_pointer(stk), *get_end_canary_pointer(stk));
        #else
            for(int num = 0; num < stk->capacity; ++num)
            {
                if(num <= stk->curr_size - 1)
                    fprintf(stk->file_with_errors, "      *");
                else
                    fprintf(stk->file_with_errors, "      ");
                
                fprintf(stk->file_with_errors, "[%d](%p) = " ELEM_SPECIFIER "\n", num, get_data_elem_pointer(stk, (size_t)num), *get_data_elem_pointer(stk, num));
            }
        #endif // DATA_USE_CANARY
    }

    

    fprintf(stk->file_with_errors, "   }\n\n");

    if(stk->file_with_errors == stdin)
        fprintf(stk->file_with_errors, "   file_with_errors[%p] = stdin\n", &stk->file_with_errors);
    else if(stk->file_with_errors == stderr)
        fprintf(stk->file_with_errors, "   file_with_errors[%p] = stderr\n", &stk->file_with_errors);
    else
        fprintf(stk->file_with_errors, "   file_with_errors[%p] = %p\n", &stk->file_with_errors, stk->file_with_errors);


    #ifdef STACK_USE_CANARY
        fprintf(stk->file_with_errors, "   stack_end_canary = " CANARY_SPECIFIER "\n", stk->end_canary);
    #endif // STACK_USE_CANARY

    fprintf(stk->file_with_errors, "}\n\n");

   return ALL_OK;
}


/*int stack_dtor(Stack* stk)
{
    if(!stk || stk == BAD_PTR)
        return stk;

    if(!(stk->data) || stk->data == BAD_PTR)
    {
        stk->data = BAD_PTR;
        free(stk);
        stk = BAD_PTR;

        return stk;
    }

    #ifdef DATA_USE_CANARY
        memset(stk->data, POISON, (stk->capacity) * sizeof(elem_type) + 2 * sizeof(canary_type));
    #else
        memset(stk->data, POISON, (stk->capacity) * sizeof(elem_type));
    #endif // DATA_USE_CANARY


    free(stk->data);

    stk->data = BAD_PTR;
    stk->curr_size = POISON;
    stk->capacity = POISON;
}*/

Stack* stack_dtor(Stack* stk)
{
    if(!stk || stk == (Stack*)BAD_PTR)
    {
        return stk;
    }

    if(!(stk->data) || stk->data == BAD_PTR)
    {
        stk->data = BAD_PTR;
        if(stk->file_with_errors)
            fclose(stk->file_with_errors);

        free(stk);
        //stk = (Stack*)BAD_PTR;

        return (Stack*)BAD_PTR;
    }

    #ifdef DATA_USE_CANARY
        memset(stk->data, POISON, (size_t)(stk->capacity) * sizeof(elem_type) + 2 * sizeof(canary_type) + (size_t)stk->gap_after_begin_canary + (size_t)stk->gap_before_end_canary);
    #else
        memset(stk->data, POISON, (size_t)(stk->capacity) * sizeof(elem_type));
    #endif // DATA_USE_CANARY


    free(stk->data);
    //stk->data = BAD_PTR;
    
    stk->curr_size = POISON;
    stk->capacity = POISON;

    if(stk->file_with_errors)
        fclose(stk->file_with_errors);
   
    free(stk);
    //stk = (Stack*)BAD_PTR;

    return (Stack*)BAD_PTR;
}


elem_type* change_capacity(Stack* stk, size_t new_capacity)
{
    #ifdef DATA_USE_CANARY
        if(sizeof(elem_type) > sizeof(canary_type))
                stk->gap_after_begin_canary = sizeof(elem_type) - sizeof(canary_type) % sizeof(elem_type);
        else
            stk->gap_before_end_canary = sizeof(canary_type) - (sizeof(elem_type) * new_capacity) % sizeof(canary_type);


        elem_type* new_memory = NULL;
        new_memory = (elem_type*)realloc(stk->data, new_capacity * sizeof(elem_type) + 
                                (size_t)stk->gap_after_begin_canary + (size_t)stk->gap_before_end_canary + 2 * sizeof(canary_type));
    #else
        elem_type* new_memory = NULL;
        new_memory = (elem_type*)realloc(stk->data, new_capacity * sizeof(elem_type));
    #endif //DATA_USE_CANARY
    
    return new_memory;
}

int stack_push(Stack* stk, elem_type value)
{
    CHECKSTACK(ALL_OK);

    if(!stk || stk == (Stack*)BAD_PTR)
        return BAD_STACK_POINTER;

    if(stk->data == BAD_PTR || !(stk->data))
        return BAD_DATA_POINTER;


    elem_type* new_memory = NULL;


    #ifdef DATA_USE_CANARY
        if(stk->curr_size + 1 > stk->capacity)
        {
            canary_type old_end_canary_ptr = *get_end_canary_pointer(stk);
            
            if(!(new_memory = change_capacity(stk, (size_t)(stk->curr_size * stk->push_change))))
            {
                CHECKSTACK(NOT_MEMORY);
                return NOT_MEMORY;
            }
            else
            {
                stk->data = new_memory;


                stk->capacity = stk->curr_size * stk->push_change;


                canary_type* new_end_canary_ptr = get_end_canary_pointer(stk);
                if(!new_end_canary_ptr)
                {
                    CHECKSTACK(BAD_POINTER);
                    return BAD_POINTER;
                }

                *new_end_canary_ptr = old_end_canary_ptr; //replace end canary


                if(stk->push_change == 2)
                    stk->pop_change = 3;
                else
                    stk->pop_change = 2;
            }
        }

        ++stk->curr_size;
        *get_data_elem_pointer(stk, stk->curr_size - 1) = value;  //add new value in stack
    #else
        if(stk->curr_size + 1 > stk->capacity)
        {
            if(!(new_memory = change_capacity(stk, stk->curr_size * stk->push_change)))
            {
                CHECKSTACK(NOT_MEMORY);
                return NOT_MEMORY;
            }
            else
            {
                stk->data = new_memory;

                stk->capacity = stk->curr_size * stk->push_change;

                if(stk->push_change == 2)
                    stk->pop_change = 3;
                else
                    stk->pop_change = 2;

            }
        }

        ++stk->curr_size;

        *get_data_elem_pointer(stk, stk->curr_size - 1) = value;  //add new value in stack

    #endif // DATA_USE_CANARY

    CHECKSTACK(ALL_OK);

    return ALL_OK;
}


elem_type stack_pop(Stack* stk, errors* error)
{
    CHECKSTACK(ALL_OK);

    if(stk->curr_size == 0)
    {
        CHECKSTACK(POP_EMPTY_STK);
        *error = POP_EMPTY_STK;
        return POISON;
    }

    elem_type pop_element = *get_data_elem_pointer(stk, stk->curr_size - 1);
    *get_data_elem_pointer(stk, stk->curr_size - 1) = POISON;

    --stk->curr_size;

    if(stk->curr_size <= stk->capacity / stk->pop_change)
    {
        #ifdef DATA_USE_CANARY
            canary_type old_end_canary_ptr = *get_end_canary_pointer(stk);

            stk->data = change_capacity(stk, (size_t)(stk->capacity / stk->pop_change));
            if(!stk->data)
            {
                CHECKSTACK(NOT_MEMORY);
                *error = NOT_MEMORY;
                return POISON;
            }

            stk->capacity /= stk->pop_change;

            canary_type* new_end_canary_ptr = get_end_canary_pointer(stk);
            *new_end_canary_ptr = old_end_canary_ptr; //change end canary

        #else
            stk->capacity /= stk->pop_change;
            stk->data = change_capacity(stk, stk->capacity);
            if(!stk->data)
            {
                CHECKSTACK(NOT_MEMORY);
                *error = NOT_MEMORY;
            }
        #endif // DATA_USE_CANARY

        

        if(stk->pop_change == 2)
            stk->push_change = 3;
        else
            stk->push_change = 2;
    }

    CHECKSTACK(ALL_OK)

    return pop_element;
}


long long stack_hash(Stack* stk, errors reason)
{
    const long long coeff = 13;
    long long hash = 0, coeff_pow = 1;
    for (int index = 0; index < stk->curr_size; ++index)
    {
        hash += stk->data[index] * coeff_pow;
        coeff_pow *= coeff;
    }
    return hash;
}



