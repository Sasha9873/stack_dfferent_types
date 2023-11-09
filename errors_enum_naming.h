/*!\file
*/

#ifndef ERRORS_ENUM_H_INCLUDED
#define ERRORS_ENUM_H_INCLUDED


    #define RED "\033[0;31m"
    #define RST "\x1b[0m" 


    typedef enum errors{        ALL_OK             =  0, 
                                NOT_MEMORY         = -1,
                                BAD_POINTER        = -2,
                                NOT_POINT_ERROR    = -3,
                                BAD_FILE_POINTER   = -4,
    } errors;

	static const char *error_names[] = {
        "All is ok",
        "Not enough memory",  //1
        "Bad pointer",
        "Not pointer to error",
        "Bad file pointer",
    };

#endif
