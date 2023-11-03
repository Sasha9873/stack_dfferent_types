/*!\file
*/

#ifndef STACK_STRUCT_H_INCLUDED
#define STACK_STRUCT_H_INCLUDED

    #define DEBAG 1

    #define CHECK 2

    #if CHECK >= 2
        #define STACK_USE_HASH
        #define STACK_USE_CANARY
        #define DATA_USE_CANARY
    #elif CHECK >= 1
        #define STACK_USE_CANARY
        #define DATA_USE_CANARY
    #endif // CHECK

    #undef CHECK


    #if DEBAG == 1
        #define CHECKSTACK(reason)\
            if(stack_ok(stk) != ALL_OK || reason != ALL_OK){\
                    if(stk && stk->file_with_errors)\
                        fprintf(stk->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
                    else\
                        fprintf(stderr, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
                stack_dump(stk, reason);\
            }
    #else
        #define CHECKSTACK(reason)\
            ;
    #endif //DEBAG

    #undef DEBAG


    typedef int elem_type;   ///< type of elements in stack data(array)
    typedef size_t canary_type;  ///< type of canaries in struct Stack and in data of Stack, if they are

    #define ELEM_SPECIFIER "%d"  ///< specifier to write data elements
    #define CANARY_SPECIFIER "%lx"  ///< specifier to write canaries

    static elem_type* const BAD_PTR = (elem_type* const)13;
    static const canary_type BEGIN_CANARY_VALUE = 0xBADC0FFEE;
    static const canary_type END_CANARY_VALUE = 0xBADDED;
    static const size_t N_DATA_ELEMS_BEGIN = 1; ///< amount of elems in data after calling constuctor of stack(stack_ctor) 
    static const int POISON = -666;  ///< Poison, it puts in elements after distroing

    typedef struct Stack
    {
        #ifdef STACK_USE_CANARY
            canary_type begin_canary;
        #endif

        #ifdef STACK_USE_HASH
            long long hash_value;
        #endif

        struct Stack* stack_pointer; ///< pointer to the object of struct Stack

        size_t capacity;
        size_t curr_size;
        elem_type* data;

        size_t pop_change;
        size_t push_change;  ///< in what value do realloc
        size_t when_pop_change;   ///< when do pop change

        int gap_after_begin_canary;
        int gap_before_end_canary;

        FILE* file_with_errors;   ///< file where will be writen errors if debag mode is on 

        #ifdef STACK_USE_CANARY
            canary_type end_canary;
        #endif

    } Stack;




#endif