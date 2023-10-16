/*!\file
*/

#include "stack_func.h"


int main()
{
	errors error;
	Stack* stk = stack_ctor(&error);

	stk->file_with_errors = fopen("file_with_errors", "w+");
	//stk->file_with_errors = stderr;

    fprintf(stk->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);
	stack_dump(stk, 0);

	stack_push(stk, 10);
	stack_push(stk, 25);
	stack_push(stk, 35);
	stack_push(stk, 45);
	stack_push(stk, 55);
	stack_push(stk, 65);

	stack_pop(stk, &error);
	stack_pop(stk, &error);
	stack_pop(stk, &error);
	stack_pop(stk, &error);
	stack_pop(stk, &error);
	stack_pop(stk, &error);
	stack_pop(stk, &error);
	stack_pop(stk, &error);


	stk->capacity = -1;

	fprintf(stk->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);
	stack_dump(stk, 0);


	stack_dtor(stk);


    // fprintf(stk->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);
	//stack_dump(stk, 0);



	return 0;
}
