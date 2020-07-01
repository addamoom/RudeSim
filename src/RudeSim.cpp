// DEBUG parameters, remove to disable
#define PRINT_DEBUG_INFO 

#include <stdio.h>
#include "../Parser.H"
#include "../Printer.H"
#include "../Absyn.H"
#include "RuleCheck.H"
#include "Simulator.H"
#include "Types.H"
#include <iostream>
#include <time.h>

std::vector<SignalType> rc_signals;
std::vector<PortType> rc_ports;

int main(int argc, char **argv)
{
    clock_t start = clock();
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

    RuleCheck ruleChecker = RuleCheck();

    if (ruleChecker.startRuleCheck(parse_tree) == 0)
    {
        std::cout << "Input is up to spec. Moving on to simulate!" << std::endl;
    }
    else
    {
        std::cerr << "Rulechecker faced an error parsing the file, stopping the simulator" << std::endl;
        return 2;
    }
    rc_signals = ruleChecker.signals;
    rc_ports = ruleChecker.entity_ports;

    Simulator sim = Simulator();

    if (sim.startSimulation(parse_tree) == 0)
    {
        std::cout << "Simulation finished sucsessfully" << std::endl;
    }
    else
    {
        std::cerr << "Simulator was unsuccessfull" << std::endl;
        return 2;
    }
    std::cout << "Simulation finished in: " << (double)(clock() - start) / (double)CLOCKS_PER_SEC << std::endl;

    return 0;
}
