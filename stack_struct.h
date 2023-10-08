#ifndef STACK_STRUCT_H_INCLUDED
#define STACK_STRUCT_H_INCLUDED

    #define CHECK 1

    #if CHECK >= 2
        #define STACK_USE_HASH
        #define STACK_USE_CANARY
        #define DATA_USE_CANARY
    #elif CHECK >= 1
        #define STACK_USE_CANARY
        #define DATA_USE_CANARY
    #endif // CHECK

    #define CHECKSTACK(reason)\
        if(!stack_ok(stk) || reason != ALL_OK){\
            if(stk && stk->file_with_errors)\
                fprintf(stk->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
            else\
                fprintf(stdout, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
        stack_dump(stk, reason);\
    }


    typedef int elem_type;
    typedef size_t canary_type;

    #define ELEM_SPECIFIER "%d"
    #define CANARY_SPECIFIER "%lx"

    static elem_type* BAD_PTR = (elem_type*)13;
    static const canary_type BEGIN_CANARY_VALUE = 0xBADC0FFEE;
    static const canary_type END_CANARY_VALUE = 0xBADDED;
    static const int N_DATA_ELEMS_BEGIN = 1; ///amount of elems in data after calling constuctor of stack(stack_ctor) 
    static const int POISON = -666;

    typedef struct Stack
    {
        #ifdef STACK_USE_CANARY
        canary_type begin_canary;
        #endif

        int capacity;
        int curr_size;
        elem_type* data;

        int pop_change;
        int push_change;

        size_t gap_after_begin_canary;
        size_t gap_before_end_canary;

        FILE* file_with_errors;  

        #ifdef STACK_USE_CANARY
        canary_type end_canary;
        #endif

    } Stack;




#endif