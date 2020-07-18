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
#include <unistd.h>
#include <bits/stdc++.h> 

std::vector<SignalType> rc_signals;
std::vector<PortType> rc_ports;

int main(int argc, char **argv)
{
    clock_t start = clock();

    int simulationTime = 5;
    std::string simulationTimeUnit = "ps";
    int simulationResolution = 5;
    FILE *input;
    std::string filename = "";
    int c;
    bool printDebugInfo = false;

    while ((c = getopt(argc, argv, "f:t:u:r:d")) != -1)
    {
        switch (c)
        {
        case 'f': //sets a input file instead of taking it from stdin.
            filename = optarg;
            break;
        case 't': //sets simulation time.
            simulationTime = std::stoi(optarg);
            break;
        case 'u': //sets unit for given simulation time.
            simulationTimeUnit = optarg;
            break;
        case 'r': //sets resolution/no of steps for the simulator
            simulationResolution = std::stoi(optarg);
            break;
        case 'd':
            printDebugInfo = true;
            break;
        case '?':
            std::cout << "Wierd parameter given to RudeSim" << std::endl;
        default:
            abort();
        };
    };

    if (filename != "")
    {
        input = fopen(filename.c_str(), "r");
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
        printf("\nParse Successful!\n");

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
    ruleChecker.printDebugInfo = printDebugInfo;

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
    sim.simulation_time = sim.convertToPs(simulationTime, simulationTimeUnit);
    sim.simulation_steps = simulationResolution;
    sim.printDebugInfo = printDebugInfo;
    if (sim.startSimulation(parse_tree) == 0)
    {
        std::cout << "Simulation finished successfully" << std::endl;
    }
    else
    {
        std::cerr << "Simulator was unsuccessfull" << std::endl;
        return 2;
    }
    std::cout << "Time elapsed: " << (double)(clock() - start) / (double)CLOCKS_PER_SEC << std::endl;

    return 0;
}
