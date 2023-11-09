/*!\file
*/

#ifndef ERRORS_ENUM_H_INCLUDED
#define ERRORS_ENUM_H_INCLUDED


    #define RED "\033[0;31m"
    #define RST "\x1b[0m" 


    typedef enum stack_errors{ALL_OK             =  0, 
                        BAD_STACK_POINTER  = -1,    //
                        WRONG_BEGIN_CANARY = -2,    //
                        WRONG_END_CANARY   = -3,    //
                        DATA_BEGIN_CANARY  = -4,    //
                        DATA_END_CANARY    = -5,   //
                        BAD_DATA_POINTER   = -6,   //
                        CAP_SMALLER_SIZE   = -7,   //
                        WRONG_HASH         = -8,   //
                        WORK_FROM_COPY     = -9,   //
                        NOT_MEMORY         = -10,
                        BAD_POINTER        = -11,
                        NOT_POINT_ERROR    = -12,
                        BAD_FILE_POINTER   = -13,
                        POP_EMPTY_STK      = -14}
    stack_errors;

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
        "Not enough memory",  //10
        "Bad pointer",
        "Not pointer to error",
        "Bad file pointer",
        "Pop from empty stack"
    };

#endif
