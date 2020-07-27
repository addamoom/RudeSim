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
#include <sstream>

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

    std::string cfile;
    std::vector<std::string> filenames;
    std::stringstream allfiles;
    std::string simEntity;

    bool printDebugInfo = false;
    bool parse = false;
    bool rulecheck = false;
    bool simulate = false;

    std::vector<Prog *> parsedfiles;

    while ((c = getopt(argc, argv, "f:t:u:r:e:dspc")) != -1)
    {
        
        switch (c)
        {
        case 'f': //sets the input files, comma separated list with relative paths.
            allfiles << optarg;
            while (allfiles.good())
            {
                std::getline(allfiles, cfile, ',');
                filenames.push_back(cfile);
            }
            break;
        case 'e': //sets simulation entity
            simEntity = optarg;
            break;
        case 't': //sets simulation time.
            simulationTime = std::stoi(optarg);
            break;
        case 's': //If this flag is present, simulate
            simulate = true;
            break;
        case 'p': //If this flag is present, parse
            parse = true;
            break;
        case 'c': //If this flag is present, rulecheck
            rulecheck = true;
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
    //print all files
    std::cout << "Files: ";
    for (auto i : filenames)
        std::cout << i << ' ';
    std::cout << "\n";

    if (parse)
    {
        for (auto i : filenames)
        {
            input = fopen(i.c_str(), "r");
            if (!input)
            {
                fprintf(stderr, "Error opening input file.\n");
                exit(1);
            }
            std::cout << "Parsing " << i << '\n';
            Prog *parse_tree = pProg(input);
            if (parse_tree)
            {
                printf("\nParse Successful!\n");
                parsedfiles.push_back(parse_tree);
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
            fclose(input);
        }
    }

    if (rulecheck)
    {
        RuleCheck ruleChecker = RuleCheck();
        ruleChecker.printDebugInfo = printDebugInfo;
        int n = 0;
        for (auto i : parsedfiles)
        {
            std::cout << "Ruleckecking " << filenames.at(n) << std::endl;
            n++;
            if (ruleChecker.startRuleCheck(i) == 0)
            {
                std::cout << "File is Correct!" << std::endl;
            }
            else
            {
                std::cerr << "Rulechecker faced an error checking the file, see output above" << std::endl;
            }
        }
    }

    if (simulate)
    {
        Simulator sim = Simulator();
        sim.simulation_time = sim.convertToPs(simulationTime, simulationTimeUnit);
        sim.simulation_steps = simulationResolution;
        sim.printDebugInfo = printDebugInfo;
        sim.topLevelEntity = simEntity;
        if (sim.startSimulation(parsedfiles) == 0)
        {
            std::cout << "Simulation finished successfully" << std::endl;
        }
        else
        {
            std::cerr << "Simulator was unsuccessfull" << std::endl;
            return 3;
        }
    }

    std::cout << "Time elapsed: " << (double)(clock() - start) / (double)CLOCKS_PER_SEC << std::endl;
    return 0;
}
