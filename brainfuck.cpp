#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

class Compiler{
    int loopcount;
    const char *prolog;
    const char *epilog;

    public:
        Compiler(){
            loopcount = 4;
            prolog = "	.comm	tape,28,16\n"
"	.globl	pos\n"
"	.bss\n"
"	.align 4\n"
"	.type	pos, @object\n"
"	.size	pos, 4\n"
"pos:\n"
"	.zero	4\n"
"    .comm   c,100,32\n"
"    .comm   i,4,4\n"
"	.text\n"
"	.globl	getVal\n"
"	.type	getVal, @function\n"
"getVal:\n"
".LFB0:\n"
"	.cfi_startproc\n"
"	pushq	%rbp\n"
"	.cfi_def_cfa_offset 16\n"
"	.cfi_offset 6, -16\n"
"	movq	%rsp, %rbp\n"
"	.cfi_def_cfa_register 6\n"
"	movl	pos(%rip), %eax\n"
"	cltq\n"
"	movl	tape(,%rax,4), %eax\n"
"	popq	%rbp\n"
"	.cfi_def_cfa 7, 8\n"
"	ret\n"
"	.cfi_endproc\n"
".LFE0:\n"
"	.size	getVal, .-getVal\n"
"	.globl	setVal\n"
"	.type	setVal, @function\n"
"setVal:\n"
".LFB1:\n"
"	.cfi_startproc\n"
"	pushq	%rbp\n"
"	.cfi_def_cfa_offset 16\n"
"	.cfi_offset 6, -16\n"
"	movq	%rsp, %rbp\n"
"	.cfi_def_cfa_register 6\n"
"	movl	%edi, -4(%rbp)\n"
"	movl	pos(%rip), %eax\n"
"	cltq\n"
"	movl	-4(%rbp), %edx\n"
"	movl	%edx, tape(,%rax,4)\n"
"	popq	%rbp\n"
"	.cfi_def_cfa 7, 8\n"
"	ret\n"
"	.cfi_endproc\n"
".LFE1:\n"
"	.size	setVal, .-setVal\n"
"	.globl	moveRight\n"
"	.type	moveRight, @function\n"
"moveRight:\n"
".LFB2:\n"
"	.cfi_startproc\n"
"	pushq	%rbp\n"
"	.cfi_def_cfa_offset 16\n"
"	.cfi_offset 6, -16\n"
"	movq	%rsp, %rbp\n"
"	.cfi_def_cfa_register 6\n"
"	movl	pos(%rip), %eax\n"
"	addl	$1, %eax\n"
"	movl	%eax, pos(%rip)\n"
"	popq	%rbp\n"
"	.cfi_def_cfa 7, 8\n"
"	ret\n"
"	.cfi_endproc\n"
".LFE2:\n"
"	.size	moveRight, .-moveRight\n"
"	.globl	moveLeft\n"
"	.type	moveLeft, @function\n"
"moveLeft:\n"
".LFB3:\n"
"	.cfi_startproc\n"
"	pushq	%rbp\n"
"	.cfi_def_cfa_offset 16\n"
"	.cfi_offset 6, -16\n"
"	movq	%rsp, %rbp\n"
"	.cfi_def_cfa_register 6\n"
"	movl	pos(%rip), %eax\n"
"	subl	$1, %eax\n"
"	movl	%eax, pos(%rip)\n"
"	popq	%rbp\n"
"	.cfi_def_cfa 7, 8\n"
"	ret\n"
"	.cfi_endproc\n"
".LFE3:\n"
"	.size	moveLeft, .-moveLeft\n"
"	.globl	main\n"
"	.type	main, @function\n"
".LC0:\n"
"    .string \"%s\"\n"
"    .text\n"
"    .globl  main\n"
"    .type   main, @function\n"
"main:\n"
".LFB4:\n"
"	.cfi_startproc\n"
"	pushq	%rbp\n"
"	.cfi_def_cfa_offset 16\n"
"	.cfi_offset 6, -16\n"
"	movq	%rsp, %rbp\n"
"	.cfi_def_cfa_register 6\n";

    	    epilog = "	popq	%rbp\n"
"	.cfi_def_cfa 7, 8\n"
"	ret\n"
"	.cfi_endproc\n"
".LFE4:\n"
"	.size	main, .-main\n"
"	.ident	\"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3\"\n"
"	.section	.note.GNU-stack,\"\",@progbits";
        }
        void compile_exp(vector<char> exp){
            int i;
            for(i=0;i<exp.size();i++){
                if(exp[i] == '['){
                    loopcount += 2;
                    cout << "\t\tjmp\t.L" << loopcount << endl;
                    cout << "\t.L" << (loopcount + 1) << ":" << endl;
                }else if(exp[i] == ']'){
                    cout << "\t.L" << loopcount << ":" << endl;
                    cout << "\t\tmovl\t$0, \%eax" << endl;
                    cout << "\t\tcall\tgetVal" << endl;
                    cout << "\t\ttestl\t\%eax, \%eax" << endl;
                    cout << "\t\tjg\t.L" << (loopcount + 1) << endl;
                }else if(exp[i] == '<'){
                    cout << "\t\tmovl\t$0, \%eax" << endl;
                    cout << "\t\tcall\tmoveLeft" << endl;
                }else if(exp[i] == '>'){
                    cout << "\t\tmovl\t$0, \%eax" << endl;
                    cout << "\t\tcall\tmoveRight" << endl;
                }else if(exp[i] == '+'){
                    cout << "\t\tmovl\t$0, \%eax" << endl;
                    cout << "\t\tcall\tgetVal" << endl;
                    cout << "\t\taddl\t$1, \%eax" << endl;
                    cout << "\t\tmovl\t\%eax, \%edi" << endl;
                    cout << "\t\tcall\tsetVal" << endl;
                }else if(exp[i] == '-'){
                    cout << "\t\tmovl\t$0, \%eax" << endl;
                    cout << "\t\tcall\tgetVal" << endl;
                    cout << "\t\tsubl\t$1, \%eax" << endl;
                    cout << "\t\tmovl\t\%eax, \%edi" << endl;
                    cout << "\t\tcall\tsetVal" << endl;
                }else if(exp[i] == '.'){
                    cout << "\t\tmovl\t$0, \%eax" << endl;
                    cout << "\t\tcall\tgetVal" << endl;
                    cout << "\t\tmovl\t\%eax, \%edi" << endl;
                    cout << "\t\tcall\tputchar" << endl;

                }else if(exp[i] == ','){
                    cout << "\t\tmovl\t$.LC0, \%eax" << endl;
                    cout << "\t\tmovl\t$c, \%esi" << endl;
                    cout << "\t\tmovq\t%rax, %rdi" << endl;
                    cout << "\t\tmovl\t$0, \%eax" << endl;
                    cout << "\t\tcall\t__isoc99_scanf" << endl;
                    cout << "\t\tmovzbl\tc(%rip), \%eax" << endl;
                    cout << "\t\tmovsbl\t\%al, \%eax" << endl;
                    cout << "\t\tmovl\t\%eax, i(%rip)" << endl;
                    cout << "\t\tmovl\ti(%rip), \%eax" << endl;
                    cout << "\t\tmovl\t\%eax, \%edi" << endl;
                    cout << "\t\tcall\tsetVal" << endl;
                }
            }
        }

        void compile(vector <char> exp){
            cout << prolog << endl;
            compile_exp(exp);
            cout << epilog << endl;
        }
};

int main(int argc, char *argv[]){
    Compiler myComp;
    ifstream myfile(argv[1]);
    vector<char> prog;
    string line;
    int i;
    while(getline(myfile,line)){
        for(i=0;i<line.size();i++)
            prog.push_back(line[i]);
    }
    myComp.compile(prog);
}
