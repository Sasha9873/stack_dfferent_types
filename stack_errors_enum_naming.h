/*!\file
*/

#ifndef STACK_ERRORS_ENUM_H_INCLUDED
#define STACK_ERRORS_ENUM_H_INCLUDED


    #define RED "\033[0;31m"
    #define RST "\x1b[0m" 


    typedef enum stack_errors{  BAD_STACK_POINTER  = -1,    //
                                WRONG_BEGIN_CANARY = -2,    //
                                WRONG_END_CANARY   = -3,    //
                                DATA_BEGIN_CANARY  = -4,    //
                                DATA_END_CANARY    = -5,   //
                                BAD_DATA_POINTER   = -6,   //
                                CAP_SMALLER_SIZE   = -7,   //
                                WRONG_HASH         = -8,   //
                                WORK_FROM_COPY     = -9,   //
                                POP_EMPTY_STK      = -10,
                                EMPTY_STK          = -11,
    } stack_errors;

	static const char *stack_error_names[] = {
        "All is ok",
        "Bad stack pointer",
        "Wrong begin stack canary",
        "Wrong end stack canary",
        "Wrong begin data canary",
        "Wrong end data canary",
        "Bad data pointer",
        "Capacity smaller then size",    //7
        "Wrong hash",
        "Work with stack from copy",
        "Pop from empty stack",
        "Empty stack",
    };

#endif
