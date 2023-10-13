/*!\file
*/

#ifndef ERRORS_ENUM_H_INCLUDED
#define ERRORS_ENUM_H_INCLUDED


    #define RED "\033[0;31m"
    #define RST "\x1b[0m" 


    typedef enum errors{ALL_OK             =  0, 
                        BAD_STACK_POINTER  = -1,    //
                        NEGATIVE_CAPASITY  = -2,    //
                        NEGATIVE_SIZE      = -3,    //
                        WRONG_BEGIN_CANARY = -4,    //
                        WRONG_END_CANARY   = -5,    //
                        DATA_BEGIN_CANARY  = -6,    //
                        DATA_END_CANARY    = -7,   //
                        BAD_DATA_POINTER   = -8,   //
                        CAP_SMALLER_SIZE   = -9,   //
                        WRONG_HASH         = -10,   //
                        NOT_MEMORY         = -11,
                        BAD_POINTER        = -12,
                        NOT_POINT_ERROR    = -13,
                        BAD_FILE_POINTER   = -14,
                        POP_EMPTY_STK      = -15}
    errors;

	static char *error_names[] = {
        "All is ok",
        "Bad stack pointer",
        "Negative capacity",
        "Negative size",
        "Wrong begin stack canary",
        "Wrong end stack canary",
        "Wrong begin data canary",
        "Wrong end data canary",
        "Bad data pointer",
        "Capacity smaller then size",    //9
        "Wrong hash",
        "Not enough memory",
        "Bad pointer",
        "Not pointer to error",
        "Bad file pointer",
        "Pop from empty stack"
    };

#endif
