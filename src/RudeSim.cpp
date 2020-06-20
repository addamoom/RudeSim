// DEBUG parameters, remove to disable
#define PRINT_DEBUG_INFO 

#include <stdio.h>
#include "../Parser.H"
#include "../Printer.H"
#include "../Absyn.H"
#include "RuleCheck.H"
#include <iostream>
int main(int argc, char **argv)
{
    FILE *input;
    if (argc > 1)
    {
        input = fopen(argv[1], "r");
        if (!input)
        {
            fprintf(stderr, "Error opening input file.\n");
            exit(1);
        }
    }
    else
        input = stdin;
    Prog *parse_tree = pProg(input);

    if (parse_tree)
    {
        printf("\nParse Succesful!\n");

        #ifdef PRINT_DEBUG_INFO
            printf("\n[Abstract Syntax]\n");
            ShowAbsyn *s = new ShowAbsyn();
            printf("%s\n\n", s->show(parse_tree));
            printf("[Linearized Tree]\n");
            PrintAbsyn *p = new PrintAbsyn();
            printf("%s\n\n", p->print(parse_tree));
        #endif
    }
    else
    {
        std::cout << "ERROR";
        return 1;
    }

    RuleCheck t = RuleCheck();

    if (t.startRuleCheck(parse_tree) == 0)
    {
        std::cout << "Input is up to spec. Moving on to simulate!" << std::endl;
    }
    else
    {
        std::cerr << "Rulechecker faced an error parsing the file, stopping the simulator" << std::endl;
        return 2;
    }

    //place simualator here!

    return 0;
}
