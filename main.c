/*!\file
*/

#include "stack_func.h"


int main()
{
	errors error;
	Stack* stk = stack_ctor(&error);
	Stack copy = *stk;



	//stk->file_with_stack_errors = fopen("file_with_stack_errors", "w+");
	stk->file_with_stack_errors = stderr;

    fprintf(stk->file_with_stack_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);
	stack_dump(stk, ALL_OK);

	stack_push(stk, 25);
	printf("stk = %p copy = %p\n", stk, &copy);
	stack_push(&copy, 10);
	// stack_push(stk, 35);
	// stack_push(stk, 45);
	// stack_push(stk, 55);
	// stack_push(stk, 65);

	printf("%d\n", stack_pop(stk, &error));
	// stack_pop(stk, &error);
	// stack_pop(stk, &error);
	// stack_pop(stk, &error);
	// stack_pop(stk, &error);
	// stack_pop(stk, &error);
	// stack_pop(stk, &error);
	// stack_pop(stk, &error);


	// stk->capacity = -1;

	// fprintf(stk->file_with_stack_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);
	// stack_dump(stk, ALL_OK);


	stack_dtor(stk);


    // fprintf(stk->file_with_stack_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);
	//stack_dump(stk, 0);



	return 0;
}
