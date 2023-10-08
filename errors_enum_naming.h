#ifndef ERRORS_ENUM_H_INCLUDED
#define ERRORS_ENUM_H_INCLUDED

    typedef enum errors{ALL_OK             = 0,
                        NOT_MEMORY         = -1,
                        BAD_POINTER        = -2,
                        NOT_POINT_ERROR    = -3,
                        BAD_STACK_POINTER  = -4,
                        NEGATIVE_CAPASITY  = -5,
                        NEGATIVE_SIZE      = -6,
                        WRONG_BEGIN_CANARY = -7,
                        WRONG_END_CANARY   = -8,
                        DATA_BEGIN_CANARY  = -9,
                        DATA_END_CANARY    = -10,
                        BAD_DATA_POINTER   = -11,
                        CAP_SMALLER_SIZE   = -12,
                        WRONG_HASH         = -13, 
                        BAD_FILE_POINTER   = -14,
                        POP_EMPTY_STK      = -15}
    errors;

	static char *error_names[] = {
        "All is ok",
        "Not enough memory",
        "Bad pointer",
        "Not pointer to error",
        "Bad stack pointer",
        "Negative capacity",
        "Negative size",
        "Wrong begin stack canary",
        "Wrong end stack canary",
        "Wrong begin data canary",
        "Wrong end data canary",
        "Bad data pointer",
        "Capacity smaller then size",    //12
        "Wrong hash",
        "Bad file pointer",
        "Pop from empty stack"
    };

#endif
