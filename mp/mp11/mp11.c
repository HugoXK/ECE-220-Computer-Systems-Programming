/*									tab:8
 *
 * mp11.c - skeleton code generation for ECE190 compiler MP (F08 MP5)
 *
 * "Copyright (c) 2008-2018 by Steven S. Lumetta."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Steve Lumetta
 * Version:	    3
 * Creation Date:   Wed Oct 15 10:16:14 2008
 * Filename:	    mp11.c
 * History:
 *	SL	3	Tue Apr 03 23:23:23 2018
 *		Changed MP5 to MP11 and 190 to 220 for ECE220.
 *	SL	2	Tue Oct 21 14:28:10 2008
 *		Extracted from gold version.
 *	SL	1	Wed Oct 15 10:16:14 2008
 *		First written.
 */



/* Introductory Paragraph:
 * This assignment requires me to use recursion to translate a description of a
 * program in a subset of the C language into LC-3 assembly code. Developing such
 * code provides me with experience with the use of recursion and pointer-based
 * data structures in C, both of which are important tools. I will also gain a
 * better understanding of the methods and challenges involved in automating
 * translations from one form of program to another. My focus will be on turning an
 * AST into a sequence of LC-3 instructions.
 */



#include <stdio.h>

#include "ece220_label.h"
#include "ece220_parse.h"
#include "ece220_symtab.h"


/*
 * PARAMETERS AND CONSTANTS
 */

typedef enum {
    BR_NEVER, BR_P, BR_Z, BR_ZP, BR_N, BR_NP, BR_NZ, BR_ALWAYS, NUM_BR
} br_type_t;



/*
 * HELPER FUNCTION PROTOTYPES--see function headers for further information
 */

static void gen_long_branch (br_type_t type, ece220_label_t* label);

static void gen_statement (ast220_t* ast);
static void gen_for_statement (ast220_t* ast);
static void gen_if_statement (ast220_t* ast);
static void gen_return_statement (ast220_t* ast);
static void gen_pop_stack (ast220_t* ast);
static void gen_debug_marker (ast220_t* ast);

static void gen_expression (ast220_t* ast);
static void gen_push_int (ast220_t* ast);
static void gen_push_str (ast220_t* ast);
static void gen_push_variable (ast220_t* ast);
static void gen_func_call (ast220_t* ast);
static void gen_get_address (ast220_t* ast);
static void gen_op_assign (ast220_t* ast);
static void gen_op_pre_incr (ast220_t* ast);
static void gen_op_pre_decr (ast220_t* ast);
static void gen_op_post_incr (ast220_t* ast);
static void gen_op_post_decr (ast220_t* ast);
static void gen_op_add (ast220_t* ast);
static void gen_op_sub (ast220_t* ast);
static void gen_op_mult (ast220_t* ast);
static void gen_op_div (ast220_t* ast);
static void gen_op_mod (ast220_t* ast);
static void gen_op_negate (ast220_t* ast);
static void gen_op_log_not (ast220_t* ast);
static void gen_op_log_or (ast220_t* ast);
static void gen_op_log_and (ast220_t* ast);
static void gen_comparison (ast220_t* ast, const char* false_cond);
static void gen_op_cmp_ne (ast220_t* ast);
static void gen_op_cmp_less (ast220_t* ast);
static void gen_op_cmp_le (ast220_t* ast);
static void gen_op_cmp_eq (ast220_t* ast);
static void gen_op_cmp_ge (ast220_t* ast);
static void gen_op_cmp_greater (ast220_t* ast);


/*
 * FILE SCOPE VARIABLES
 * 
 * N.B.  You will need to have a file scope variable to implement one
 * of the statements.  Define it here.
 *
 * static int foo;  <-- a file scope variable ("static" means only this file)
 *
 */



/* 
 * INTERFACE FUNCTIONS -- called from other files; the one function listed
 * here is the equivalent of "main" for students working on this MP
 */

/* 
 * MP11_generate_code
 *   DESCRIPTION: perform LC-3 code generation (print to stdout)
 *   INPUTS: prog -- the main subroutine of the program (a list of
 *                   statements)
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
ece220_label_t* end;
void 
MP11_generate_code (ast220_t* prog)
{
    //Declarations
    end = label_create();
    //Initialize labels
    ast220_t* Lumetta = prog;
    for(;Lumetta!= NULL;){
        gen_statement(Lumetta);
        Lumetta = Lumetta->next;
    }
    //Print all labels
    printf("%s\n",label_value(end));
}

/*
 * HELPER FUNCTIONS (used only within this file)
 */

/* 
 * gen_long_branch
 *   DESCRIPTION: generate LC-3 assembly code for a branch without offset
 *                limitations
 *   INPUTS: type -- the type of branch to be performed
 *           label -- the branch target label
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_long_branch (br_type_t type, ece220_label_t* label)
{
    static const char* const br_names[NUM_BR] = {
        "; ", "BRp", "BRz", "BRzp", "BRn", "BRnp", "BRnz", "BRnzp"
    }; 
    br_type_t neg_type;
    ece220_label_t* target_label;
    ece220_label_t* false_label;

    neg_type = (type ^ 7);
    target_label = label_create ();
    false_label = label_create ();
    printf ("\t%s %s\n", br_names[neg_type], label_value (false_label));
    printf ("\tLD R3,%s\n\tJMP R3\n", label_value (target_label));
    printf ("%s\n", label_value (target_label));
    printf ("\t.FILL %s\n", label_value (label));
    printf ("%s\n", label_value (false_label));
}

/* 
 * gen_statement
 *   DESCRIPTION: generate LC-3 assembly code for an arbitrary statement
 *   INPUTS: ast -- the AST node corresponding to the statement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_statement (ast220_t* ast)
{
    switch (ast->type) {
        case AST220_FOR_STMT:     gen_for_statement (ast);    break;
        case AST220_IF_STMT:      gen_if_statement (ast);     break;
        case AST220_RETURN_STMT:  gen_return_statement (ast); break;
        case AST220_POP_STACK:    gen_pop_stack (ast);        break;
        case AST220_DEBUG_MARKER: gen_debug_marker (ast);     break;
	default: fputs ("BAD STATEMENT TYPE\n", stderr);      break;
    }
}

/* 
 * gen_for_statement
 *   DESCRIPTION: generate LC-3 assembly code for an for loop
 *   INPUTS: ast -- the AST node corresponding to the for loop
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_for_statement (ast220_t* ast)
{
    //Declarations and initilizations
    ast220_t*  lab_ast = ast->middle;
    ece220_label_t* lab_one=label_create();
    ece220_label_t* lab_two=label_create();
    
    //If not NULL, put on the stack
    if (ast->left!=NULL){
        gen_statement(ast->left);
    }
    
    printf("%s\n",label_value(lab_one));
    //This is the label at the beginning of the loop
    gen_expression(ast->test);
    
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    printf("\tADD R0,R0,#0\n");
    gen_long_branch(BR_Z,lab_two);
    
    //If not NULL update the expression for the next
    for ( ; lab_ast != NULL; ){
        gen_statement(lab_ast);
        lab_ast = lab_ast->next;
    }
    //If not NULL, put on the stack
    if(ast->right != NULL){
        gen_statement(ast->right);
    }
    //Go back to the beginning of the loop
    gen_long_branch(BR_ALWAYS,lab_one);
    printf("%s\n",label_value(lab_two));
    //This is the label at the end
}

/* 
 * gen_if_statement
 *   DESCRIPTION: generate LC-3 assembly code for an if statement
 *   INPUTS: ast -- the AST node corresponding to the if statement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_if_statement (ast220_t* ast)
{
    //Declarations and initilizations
    ast220_t* lab_ast_left = ast->left;
    ast220_t* lab_ast_right = ast->right;
    
    ece220_label_t* lab_one = label_create();
    ece220_label_t* lab_two = label_create();
    
    gen_expression(ast->test);
    
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    //Pop out the expression from the stack
    printf("\tADD R0,R0,#0\n");
    //Add nothing just for judgement
    
    gen_long_branch(BR_Z,lab_one);
    
    //If true, continue
    for (; lab_ast_left != NULL; lab_ast_left = lab_ast_left->next){
        gen_statement(lab_ast_left);
    }
    //Jump to the end of the expression unconditionally
    gen_long_branch(BR_ALWAYS,lab_two);
    
    printf("%s\n",label_value(lab_one));
    
    for (; lab_ast_right != NULL; lab_ast_right = lab_ast_right->next){
        gen_statement(lab_ast_right);
    }
    printf("%s\n",label_value(lab_two));
    //This is the label at the end
}

/* 
 * gen_return_statement
 *   DESCRIPTION: generate LC-3 assembly code for a return statement
 *   INPUTS: ast -- the AST node corresponding to the return statement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_return_statement (ast220_t* ast)
{
    gen_expression(ast->left);
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    //Pop out from the stack
    printf("\tSTR R0,R5,#3\n");
    gen_long_branch(BR_ALWAYS,end);
    //Always jump to the end of the expression
}

/* 
 * gen_pop_stack
 *   DESCRIPTION: generate LC-3 assembly code to pop and discard a value
 *                from the stack
 *   INPUTS: ast -- the AST node corresponding to the pop stack statement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_pop_stack (ast220_t* ast)
{
    gen_expression(ast->left);
    printf("\tADD R6,R6,#1\n");
    //Just pop the stack
}

/* 
 * gen_debug_marker
 *   DESCRIPTION: generate LC-3 assembly code for a debug marker used to
 *                help debug LC-3 code generation; the marker is simply
 *                a comment in the assembly code that can be used to
 *                identify pieces of code generated by the compiler
 *   INPUTS: ast -- the AST node corresponding to the debug marker
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an LC-3 assembly comment
 */
static void 
gen_debug_marker (ast220_t* ast)
{
    printf ("; --------------- DEBUG(%d) ---------------\n", ast->value);
}

/* 
 * gen_expression
 *   DESCRIPTION: generate LC-3 assembly code to calculate an arbitrary
 *                expression and push the result onto the stack
 *   INPUTS: ast -- the AST node corresponding to the expression
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_expression (ast220_t* ast)
{
    switch (ast->type) {
        case AST220_PUSH_INT:     gen_push_int (ast);       break;
        case AST220_PUSH_STR:     gen_push_str (ast);       break;
        case AST220_VARIABLE:     gen_push_variable (ast);  break;
        case AST220_FUNC_CALL:    gen_func_call (ast);      break;
	case AST220_GET_ADDRESS:  gen_get_address (ast);    break;
	case AST220_OP_ASSIGN:    gen_op_assign (ast);      break;
	case AST220_OP_PRE_INCR:  gen_op_pre_incr (ast);    break;
	case AST220_OP_PRE_DECR:  gen_op_pre_decr (ast);    break;
	case AST220_OP_POST_INCR: gen_op_post_incr (ast);   break;
	case AST220_OP_POST_DECR: gen_op_post_decr (ast);   break;
	case AST220_OP_ADD:       gen_op_add (ast);         break;
	case AST220_OP_SUB:       gen_op_sub (ast);         break;
	case AST220_OP_MULT:      gen_op_mult (ast);        break;
	case AST220_OP_DIV:       gen_op_div (ast);         break;
	case AST220_OP_MOD:       gen_op_mod (ast);         break;
	case AST220_OP_NEGATE:    gen_op_negate (ast);      break;
	case AST220_OP_LOG_NOT:   gen_op_log_not (ast);     break;
	case AST220_OP_LOG_OR:    gen_op_log_or (ast);      break;
	case AST220_OP_LOG_AND:   gen_op_log_and (ast);     break;
	case AST220_CMP_NE:       gen_op_cmp_ne (ast);      break;
	case AST220_CMP_LESS:     gen_op_cmp_less (ast);    break;
	case AST220_CMP_LE:       gen_op_cmp_le (ast);      break;
	case AST220_CMP_EQ:       gen_op_cmp_eq (ast);      break;
	case AST220_CMP_GE:       gen_op_cmp_ge (ast);      break;
	case AST220_CMP_GREATER:  gen_op_cmp_greater (ast); break;
	default: fputs ("BAD EXPRESSION TYPE\n", stderr);   break;
    }
}

/* 
 * gen_push_int
 *   DESCRIPTION: generate LC-3 assembly code to push a constant integer
 *                expression onto the stack
 *   INPUTS: ast -- the AST node corresponding to the integer
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_push_int (ast220_t* ast)
{
    ece220_label_t *lab_one;
    ece220_label_t *lab_two;
    
    lab_one = label_create();
    lab_two = label_create();
    
    printf("\tLD R0,%s\n",label_value(lab_one));
    //Push it onto the stack
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
    printf("\tBRnzp %s\n",label_value(lab_two));
    //Jump over the data stored directly to the end
    printf("%s\n",label_value(lab_one));
    printf("\t.FILL #%d\n",ast->value);
    printf("%s\n",label_value(lab_two));
    //This is the label at the end
}

/* 
 * gen_push_str
 *   DESCRIPTION: generate LC-3 assembly code to push a constant string
 *                expression onto the stack
 *   INPUTS: ast -- the AST node corresponding to the string
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_push_str (ast220_t* ast)
{
    //Declarations and initilizations
    ece220_label_t *lab_one;
    ece220_label_t *lab_two;
    
    lab_one = label_create();
    lab_two = label_create();
    
    printf("\tLEA R0,%s\n",label_value(lab_one));
    //Push onto the stack
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
    printf("\tBRnzp %s\n",label_value(lab_two));
    //Jump over the data stored directly to the end
    printf("%s\n",label_value(lab_one));
    printf("\t.STRINGZ %s\n",ast->name);
    printf("%s\n",label_value(lab_two));
    //This is the label at the end
}

/* 
 * gen_push_variable
 *   DESCRIPTION: generate LC-3 assembly code to push a variable's value
 *                onto the stack
 *   INPUTS: ast -- the AST node corresponding to the variable reference
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_push_variable (ast220_t* ast)
{
    //Declarations and initilizations
    symtab_entry_t* Christmas=symtab_lookup(ast->name);
    if(Christmas->array_len==0){
        //If it is not an array
        if(Christmas->is_global==1){
            //If global data field
            printf("LDR R0,R4,#%d\n",Christmas->offset);
            printf("ADD R6,R6,#-1\n");
            printf("STR R0,R6,#0\n");
        }else{
            //If local
            printf("LDR R0,R5,#%d\n",Christmas->offset);
            printf("ADD R6,R6,#-1\n");
            printf("STR R0,R6,#0\n");
        }
    }else{
        //If it is an array
        if(Christmas->is_global==1){
            //If global data field
            gen_expression(ast->left);
            printf("ADD R0,R4,#%d\n",Christmas->offset);
            printf("LDR R1,R6,#0\n");
            printf("ADD R0,R1,R0\n");
            printf("LDR R0,R0,#0\n");
            printf("STR R0,R6,#0\n");
        }else{
            gen_expression(ast->left);
            //If local
            printf("ADD R0,R5,#%d\n",Christmas->offset);
            printf("LDR R1,R6,#0\n");
            printf("ADD R0,R1,R0\n");
            printf("LDR R0,R0,#0\n");
            printf("STR R0,R6,#0\n");
        }
    }
}

/* 
 * gen_func_call
 *   DESCRIPTION: generate LC-3 assembly code to perform a function call,
 *                remove the arguments from the stack, and leave the
 *                return value on top of the stack
 *   INPUTS: ast -- the AST node corresponding to the function call
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_func_call (ast220_t* ast)
{
    //Declarations and initilizations
   	 ast220_t *Sam = ast->left;
    int Steven = 0;
    while (Sam != NULL)
    {
        gen_expression(Sam);
        //push ast->name on the stack
        Steven++; 
        Sam = Sam->next;
        //then the next argument
    }
    ece220_label_t *lab_one;
    ece220_label_t *lab_two;
    lab_one = label_create();
    lab_two = label_create();
    printf("\tLD R0,%s\n", label_value(lab_one));
    printf("\tJSRR R0\n");
    printf("\tBRnzp %s\n",label_value(lab_two));
  
    //Use switch to discuss all conditions
    switch (ast->fnum)
    {
    case AST220_PRINTF:
        printf("%s\n", label_value(lab_one));
        printf("\t.FILL PRINTF\n");
        break;
    case AST220_RAND:
        printf("%s\n", label_value(lab_one));
        printf("\t.FILL RAND\n");
        break;
    case AST220_SCANF:
        printf("%s\n", label_value(lab_one));
        printf("\t.FILL SCANF\n");
        break;
    case AST220_SRAND:
        printf("%s\n", label_value(lab_one));
        printf("\t.FILL SRAND\n");
        break;
    default:
        break;
    }
    printf("%s\n",label_value(lab_two));
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6, R6, #1\n");
    printf("\tADD R6,R6,#%d\n",Steven);
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
}

/* 
 * gen_get_address
 *   DESCRIPTION: generate LC-3 assembly code to push the address of
 *                a variable onto the stack
 *   INPUTS: ast -- the AST node corresponding to the variable reference
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_get_address (ast220_t* ast)
{ symtab_entry_t* Merry=symtab_lookup(ast->left->name);
    //Declarations and initilizations
    if(Merry->array_len==0){
        if(Merry->is_global){
            //If gloabl data field
            printf("\tADD R0,R4,#%d\n",Merry->offset);
            
        }
         if(Merry->is_global==0){
             //If local
            printf("\tADD R0,R5,#%d\n",Merry->offset);
        }
        printf("\tADD R6,R6,#-1\n");
        printf("\tSTR R0,R6,#0\n");
    }
    else{
        if(Merry->is_global){
            //If gloabl data field
            gen_expression(ast->left->left);
            printf("\tLDR R1,R6,#0\n");
            printf("\tADD R6,R6,#1\n");
            printf("\tADD R0,R4,#%d\n",Merry->offset);
            printf("\tADD R0,R0,R1\n");
            
        }
        if(Merry->is_global==0){
            //If local
            gen_expression(ast->left->left);
            printf("\tLDR R1,R6,#0\n");
            printf("\tADD R6,R6,#1\n");
            printf("\tADD R0,R5,#%d\n",Merry->offset);
            printf("\tADD R0,R0,R1\n");
        }
        printf("\tADD R6,R6,#-1\n");
        //Push the stack
        printf("\tSTR R0,R6,#0\n");
    }
}

/* 
 * gen_op_assign
 *   DESCRIPTION: generate LC-3 assembly code to perform an assignment,
 *                leaving the value assigned on the stack (assignments
 *                are also expressions, with value equal to that of the
 *                value assigned)
 *   INPUTS: ast -- the AST node corresponding to the assignment
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_assign (ast220_t* ast)
{ symtab_entry_t* ECE220=symtab_lookup(ast->left->name);
    //Declarations and initilizations
    gen_expression(ast->right);
    if(ECE220->array_len==0){
        //If not an array
        if(ECE220->is_global){
            //If gloabl data field
            printf("\tLDR R0,R6,#0\n");
            printf("\tSTR R0,R4,#%d\n",ECE220->offset);
        }
        
        if(ECE220->is_global==0){
            //If local
            printf("\tLDR R0,R6,#0\n");
            printf("\tSTR R0,R5,#%d\n",ECE220->offset);
        }
    }
    else{
        //If not an array
        gen_expression(ast->left->left);
        if(ECE220->is_global){
            //If gloabl data field
            printf("\tLDR R0,R6,#0\n");
            printf("\tADD R6,R6,#1\n");
            printf("\tADD R1,R4,#%d\n", ECE220->offset);
            printf("\tADD R1,R1,R0\n");
            printf("\tLDR R0,R6,#0\n");

        
        }
        if(ECE220->is_global==0){
            //If local
            printf("\tLDR R0,R6,#0\n");
            printf("\tADD R6,R6,#1\n");
            printf("\tADD R1,R5,#%d\n", ECE220->offset);
            printf("\tADD R1,R1,R0\n");
            printf("\tLDR R0,R6,#0\n");
        }
       
        printf("\tSTR R0,R1,#0\n");
    }
}

/* 
 * gen_op_pre_incr
 *   DESCRIPTION: generate LC-3 assembly code to for a pre-increment 
 *                expression, which increments a variable and pushes
 *                the new value of the variable onto the stack
 *   INPUTS: ast -- the AST node corresponding to pre-increment
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_pre_incr (ast220_t* ast)
{
    symtab_entry_t* TooHard=symtab_lookup(ast->left->name);
    //Declarations and initilizations
    if(TooHard->is_global){
        printf("\tLDR R0,R4,#%d\n",TooHard->offset);
        //use offset to find the variable
        printf("\tADD R1,R0,#1\n");
        printf("\tSTR R1,R4,#%d\n",TooHard->offset);
        //update the variable for the next
    }
    
    else{
        printf("\tLDR R0,R5,#%d\n",TooHard->offset);
        //use offset to find the variable
        printf("\tADD R1,R0,#1\n");
        printf("\tSTR R1,R5,#%d\n",TooHard->offset);
        //update the variable for the next
	
    }
    printf("\tADD R6,R6,#-1\n");
    //push the new value onto the stack
    printf("\tSTR R1,R6,#0\n");
}

/* 
 * gen_op_pre_decr
 *   DESCRIPTION: generate LC-3 assembly code to for a pre-decrement 
 *                expression, which decrements a variable and pushes
 *                the new value of the variable onto the stack
 *   INPUTS: ast -- the AST node corresponding to pre-decrement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_pre_decr (ast220_t* ast)
{
    symtab_entry_t* I_Die=symtab_lookup(ast->left->name);
    //Declarations and initilizations
    if(I_Die->is_global){
        //If global data field
        printf("\tLDR R0,R4,#%d\n",I_Die->offset);
        //use offset to find the variable
        printf("\tADD R1,R0,#-1\n");
        printf("\tSTR R1,R4,#%d\n",I_Die->offset);
        //update the variable for the next
      
       
    }
    
    else{
        //If local
        printf("\tLDR R0,R5,#%d\n",I_Die->offset);
        //use offset to find the variable
        printf("\tADD R1,R0,#-1\n");
        printf("\tSTR R1,R5,#%d\n",I_Die->offset);
        //update the variable for the next
	
    }
    printf("\tADD R6,R6,#-1\n");
    //push the new value onto the stack
    printf("\tSTR R1,R6,#0\n");
}

/* 
 * gen_op_post_incr
 *   DESCRIPTION: generate LC-3 assembly code to for a post-increment 
 *                expression, which increments a variable and pushes
 *                the original value of the variable onto the stack
 *   INPUTS: ast -- the AST node corresponding to post-increment
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_post_incr (ast220_t* ast)
{ 
    symtab_entry_t* MP=symtab_lookup(ast->left->name);
    //Declarations and initilizations
    if(MP->is_global){
        //If global data field
        printf("\tLDR R0,R4,#%d\n",MP->offset);
        //use offset to find the variable
        printf("\tADD R1,R0,#1\n");
        printf("\tSTR R1,R4,#%d\n",MP->offset);
        //update the variable for the next

       
    }
    
    else{
        //If local
        printf("\tLDR R0,R5,#%d\n",MP->offset);
        //use offset to find the variable
        printf("\tADD R1,R0,#1\n");
        printf("\tSTR R1,R5,#%d\n",MP->offset);
        //update the variable for the next
	
    }
    printf("\tADD R6,R6,#-1\n");
    //push the original value back to the stack
    printf("\tSTR R0,R6,#0\n");
   
}

/* 
 * gen_op_post_decr
 *   DESCRIPTION: generate LC-3 assembly code to for a post-decrement 
 *                expression, which decrements a variable and pushes
 *                the original value of the variable onto the stack
 *   INPUTS: ast -- the AST node corresponding to post-decrement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_post_decr (ast220_t* ast)
{
    symtab_entry_t* find=symtab_lookup(ast->left->name);
    //Declarations and initilizations
    if(find->is_global){
        //If global data field
        printf("\tLDR R0,R4,#%d\n",find->offset);
        //use offset to find the variable
        printf("\tADD R1,R0,#-1\n");
        printf("\tSTR R1,R4,#%d\n",find->offset);
        //update the variable for the next

       
    }
    
    else{//If local
        printf("\tLDR R0,R5,#%d\n",find->offset);
        //use offset to find the variable
        printf("\tADD R1,R0,#-1\n");
        printf("\tSTR R1,R5,#%d\n",find->offset);
        //update the variable for the next
	
    }
    printf("\tADD R6,R6,#-1\n");
    //push the original value back to the stack
    printf("\tSTR R0,R6,#0\n");
}

/* 
 * gen_op_add
 *   DESCRIPTION: generate LC-3 assembly code to calculate the sum of two
 *                expressions, leaving the sum on the stack
 *   INPUTS: ast -- the AST node corresponding to the add operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_add (ast220_t* ast)
{
    //Generate the expressions
    gen_expression(ast->left);
    gen_expression(ast->right);

    //Pop one operand from the stack
    printf("\tLDR R1,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    //Pop another operand from the stack
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
   
   
    printf("\tADD R0,R0,R1\n");
    //add two operands together
    printf("\tADD R6,R6,#-1\n");
    //push the result back
    printf("\tSTR R0,R6,#0\n");
}

/* 
 * gen_op_sub
 *   DESCRIPTION: generate LC-3 assembly code to calculate the difference 
 *                of two expressions, leaving the difference on the stack
 *   INPUTS: ast -- the AST node corresponding to the subtraction operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_sub (ast220_t* ast)
{
      //Generate the expressions
    gen_expression(ast->left);
    gen_expression(ast->right);

    //Pop one operand from the stack
    printf("\tLDR R1,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    //Pop another operand from the stack
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    
    //do the minus operation
    printf("\tNOT R1,R1\n");
    printf("\tADD R1,R1,#1\n");
    printf("\tADD R0,R0,R1\n");
    //Push back to the stack
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
}

/* 
 * gen_op_mult
 *   DESCRIPTION: generate LC-3 assembly code to calculate the product 
 *                of two expressions (using the MULTIPLY subroutine in
 *                the LC-3 library), leaving the product on the stack
 *   INPUTS: ast -- the AST node corresponding to the multiplication operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_mult (ast220_t* ast)
{
    //Declarations and initializations
    ece220_label_t* lab_one;
    lab_one = label_create();
    ece220_label_t* lab_two;
    lab_two = label_create();
    
    //Generate the expressions
    gen_expression(ast->left);
    gen_expression(ast->right);

    //Pop one operand from the stack
    printf("\tLDR R1,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    //Pop another operand from the stack
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    printf ("\tLD R3,%s\n\tJSRR R3\n", label_value (lab_one));
    printf ("\tBRnzp %s\n",label_value (lab_two));
    //Jump through the data stored directly to the end
    printf ("%s\n", label_value (lab_one));
    printf ("\t.FILL MULTIPLY\n");
    printf ("%s\n", label_value (lab_two));
    //Push back to the stack
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
}

/* 
 * gen_op_div
 *   DESCRIPTION: generate LC-3 assembly code to calculate the quotient 
 *                of two expressions (using the DIVIDE subroutine in
 *                the LC-3 library), leaving the quotient on the stack
 *   INPUTS: ast -- the AST node corresponding to the division operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_div (ast220_t* ast)
{
    //Declarations and initializations
    ece220_label_t *lab_div;
    ece220_label_t *lab_end;

    //Generate the expressions
    lab_div = label_create();
    lab_end = label_create();
    gen_expression(ast->left);
    gen_expression(ast->right);

    //Pop one operand from the stack
    printf("\tLDR R1,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    //Pop another operand from the stack
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    printf("\tLD R3,%s\n", label_value(lab_div));
    printf("\tJSRR R3\n");
    printf("\tBRnzp %s\n", label_value(lab_end));
    //Jump through the data stored directly to the end
    printf("%s\n", label_value(lab_div));
    printf("\t.FILL DIVIDE\n");
    printf("%s\n", label_value(lab_end));
    //Push back to the stack
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
}

/* 
 * gen_op_mod
 *   DESCRIPTION: generate LC-3 assembly code to calculate the modulus 
 *                of one expression relative to a second expression (using 
 *                the MODULUS subroutine in the LC-3 library), leaving 
 *                the result on the stack
 *   INPUTS: ast -- the AST node corresponding to the division operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_mod (ast220_t* ast)
{
    //Declarations and initializations
    ece220_label_t* lab_one;
    lab_one = label_create();
    ece220_label_t* lab_two;
    lab_two = label_create();
    
    //Generate the expressions
    gen_expression(ast->left);
    gen_expression(ast->right);
    
    //Pop one operand from the stack
    printf("\tLDR R1,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    //Pop another operand from the stack
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    printf("\tLD R3,%s\n", label_value(lab_one));
    printf("\tJSRR R3\n");
    printf("\tBRnzp %s\n", label_value(lab_two));
    //Jump through the data stored directly to the end
    printf("%s\n", label_value(lab_one));
    printf("\t.FILL MODULUS\n");
    printf("%s\n", label_value(lab_two));
    printf("\tADD R6,R6,#-1\n");
    //Push back to the stack
    printf("\tSTR R0,R6,#0\n");
}

/* 
 * gen_op_negate
 *   DESCRIPTION: generate LC-3 assembly code to negate the value of an
 *                expression, leaving the result on the stack
 *   INPUTS: ast -- the AST node corresponding to the negation operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_negate (ast220_t* ast)
{
    gen_expression(ast->left);
    printf("\tLDR R0,R6,#0\n");
    //do the operation "~x+1" to get the negative value -x
    printf("\tNOT R0,R0\n");
    printf("\tADD R0,R0,#1\n");
    printf("\tSTR R0,R6,#0\n");
}

/* 
 * gen_op_log_not
 *   DESCRIPTION: generate LC-3 assembly code to perform a logical NOT
 *                operation on an expression, leaving the result (0 or 1)
 *                on the stack
 *   INPUTS: ast -- the AST node corresponding to the logical NOT operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_log_not (ast220_t* ast)
{
    //Declarations and initializations
    ece220_label_t* lab_one = label_create();
    ece220_label_t* lab_two = label_create();
    
    //Generalize and pop from the stack
    gen_expression(ast->left);
    
    printf("\tLDR R0,R6,#0\n");
    printf("\tBRz %s\n",label_value(lab_one));
    //If the value is nonzero, its not value should be zero
    printf("\tAND R0,R0,#0\n");
    printf("\tSTR R0,R6,#0\n");
    printf("\tBRnzp %s\n",label_value(lab_two));
    //Jump through the condition for zerovalue to the end directly
    printf("%s\n",label_value(lab_one));
    //If the value is 0, its not value should be one
    printf("\tAND R0,R0,#0\n");
    printf("\tADD R0,R0,#1\n");
    printf("\tSTR R0,R6,#0\n");
    printf("%s\n",label_value(lab_two));
    //This is the label at the end
}

/* 
 * gen_op_log_or
 *   DESCRIPTION: generate LC-3 assembly code to perform a logical OR
 *                operation on two expressions, leaving the result (0 or 1)
 *                on the stack; the code generated must perform proper
 *                shortcutting, i.e., if the first expression is true
 *                the second is not evaluated (the code is not executed)
 *   INPUTS: ast -- the AST node corresponding to the logical OR operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_log_or (ast220_t* ast)
{
    //Declarations and initializations
    ece220_label_t* lab_end = label_create();
    ece220_label_t* lab_true = label_create();
    
    //Generalize and pop from the stack
    gen_expression(ast->left);
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    printf("\tADD R0,R0,#0\n");
    gen_long_branch(BR_NP,lab_true);
    //If the first operand is not zero, the OR value must be 1
    
    //Generalize and pop from the stack
    gen_expression(ast->right);
    printf("\tLDR R1,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    printf("\tADD R1,R1,#0\n");
    printf("\tBRnp %s\n",label_value(lab_true));
    //If the second operand is not zero, the OR value must be 1
    printf("\tAND R0,R0,#0\n");
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
    //If both the first or the second is zero, the OR value is one
    printf("\tBRnzp %s\n",label_value(lab_end));
    //Jump to the end of the expression directly

    printf("%s\n",label_value(lab_true));
    //This is the place set OR value to 1 and push
    printf("\tAND R0,R0,#0\n");
    printf("\tADD R0,R0,#1\n");
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
    printf("%s\n",label_value(lab_end));
    //This is the label at the end
    
}

/* 
 * gen_op_log_and
 *   DESCRIPTION: generate LC-3 assembly code to perform a logical AND
 *                operation on two expressions, leaving the result (0 or 1)
 *                on the stack; the code generated must perform proper
 *                shortcutting, i.e., if the first expression is false
 *                the second is not evaluated (the code is not executed)
 *   INPUTS: ast -- the AST node corresponding to the logical AND operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_log_and (ast220_t* ast)
{
    //Declarations and initializations
    ece220_label_t* lab_one = label_create();
    ece220_label_t* lab_two = label_create();
    
    //Generalize and pop from the stack
    gen_expression(ast->left);
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    printf("\tADD R0,R0,#0\n");
    gen_long_branch(BR_Z,lab_two);
    //If the first operand is zero, the AND value must be zero
    
    //Generalize and pop from the stack
    gen_expression(ast->right);
    printf("\tLDR R1,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    printf("\tADD R1,R1,#0\n");
    printf("\tBRz %s\n",label_value(lab_two));
    //If the second operand is zero, the AND value must be zero
    printf("\tAND R0,R0,#0\n");
    printf("\tADD R0,R0,#1\n");
    //If neither the first or the second is zero, the AND value is one
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
    //Push 1 back to the stack
    printf("\tBRnzp %s\n",label_value(lab_one));
    //Jump to the end of the expression directly

    printf("%s\n",label_value(lab_two));
    //This is the place set AND value to 0 and push
    printf("\tAND R0,R0,#0\n");
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
    printf("%s\n",label_value(lab_one));
    //This is the label at the end
}

/* 
 * gen_comparison
 *   DESCRIPTION: generate LC-3 assembly code for a comparison operator,
 *                leaving the result (0 or 1) on the stack
 *   INPUTS: ast -- the AST node corresponding to the comparison
 *           false_cond -- a string representation of the condition codes 
 *                         for which the comparison is false (as calculated
 *                         by subtracting the second expression from the 
 *                         first)
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 *   KNOWN BUGS: ignores 2's complement overflow in making the comparison
 */
static void
gen_comparison (ast220_t* ast, const char* false_cond)
{
    ece220_label_t* false_label;

    false_label = label_create ();
    gen_expression (ast->left);
    gen_expression (ast->right);
    printf ("\tLDR R1,R6,#0\n\tLDR R0,R6,#1\n\tADD R6,R6,#2\n");
    printf ("\tAND R2,R2,#0\n\tNOT R1,R1\n\tADD R1,R1,#1\n\tADD R0,R0,R1\n");
    printf ("\tBR%s %s\n\tADD R2,R2,#1\n", false_cond,
	    label_value (false_label));
    printf ("%s\n", label_value (false_label));
    printf ("\tADD R6,R6,#-1\n\tSTR R2,R6,#0\n");
}

/* 
 * gen_op_cmp_ne
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is not equal to the value of a second 
 *                expression, leaving the result (0 or 1) on the stack
 *   INPUTS: ast -- the AST node corresponding to the inequality operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_ne (ast220_t* ast)
{
    gen_comparison (ast, "z");
}

/* 
 * gen_op_cmp_less
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is less than the value of a second
 *                expression, leaving the result (0 or 1) on the stack
 *   INPUTS: ast -- the AST node corresponding to the less-than operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_less (ast220_t* ast)
{
    gen_comparison (ast, "zp");
}

/* 
 * gen_op_cmp_le
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is less than or equal to the value 
 *                of a second expression, leaving the result (0 or 1) on 
 *                the stack
 *   INPUTS: ast -- the AST node corresponding to the less-or-equal-to 
 *                  operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_le (ast220_t* ast)
{
    gen_comparison (ast, "p");
}

/* 
 * gen_op_cmp_eq
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is equal to the value of a second 
 *                expression, leaving the result (0 or 1) on the stack
 *   INPUTS: ast -- the AST node corresponding to the equality operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_eq (ast220_t* ast)
{
    gen_comparison (ast, "np");
}

/* 
 * gen_op_cmp_ge
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is greater than or equal to the value 
 *                of a second expression, leaving the result (0 or 1) on 
 *                the stack
 *   INPUTS: ast -- the AST node corresponding to the greater-or-equal-to 
 *                  operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_ge (ast220_t* ast)
{
    gen_comparison (ast, "n");
}

/* 
 * gen_op_cmp_greater
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is greater than the value of a second
 *                expression, leaving the result (0 or 1) on the stack
 *   INPUTS: ast -- the AST node corresponding to the greater-than operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_greater (ast220_t* ast)
{
    gen_comparison (ast, "nz");
}
