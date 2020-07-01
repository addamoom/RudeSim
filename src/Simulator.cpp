/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Simulator.H"
#include "Types.H"
#include <iostream>
#include <map>
#include <time.h>

#define REDTEXT "\033[1;31m"    //Use for errors
#define YELLOWTEXT "\033[1;33m" //Use for warnings
#define GREENTEXT "\033[1;32m"  //Use for Sucesses
#define NORMTEXT "\033[0m"      //Reset text back to normal

void Simulator::visitProg(Prog *t) {}                                   //abstract class
void Simulator::visitTopDef(TopDef *t) {}                               //abstract class
void Simulator::visitPorts(Ports *t) {}                                 //abstract class
void Simulator::visitPre_begin_statements(Pre_begin_statements *t) {}   //abstract class
void Simulator::visitPost_begin_statements(Post_begin_statements *t) {} //abstract class
void Simulator::visitPmap_stm(Pmap_stm *t) {}                           //abstract class
void Simulator::visitConcurrent_statement(Concurrent_statement *t) {}   //abstract class
void Simulator::visitAss_statements(Ass_statements *t) {}               //abstract class
void Simulator::visitWhen_part(When_part *t) {}                         //abstract class
void Simulator::visitSequential_statement(Sequential_statement *t) {}   //abstract class
void Simulator::visitWhens(Whens *t) {}                                 //abstract class
void Simulator::visitExp(Exp *t) {}                                     //abstract class
void Simulator::visitCmpop(Cmpop *t) {}                                 //abstract class
void Simulator::visitLiteral(Literal *t) {}                             //abstract class
void Simulator::visitType(Type *t) {}                                   //abstract class

//extern std::vector<SignalType> rc_signals;
//extern std::vector<PortType> rc_ports;

//Simulation parameters
long int simulation_time = 5;                    //time the simulation should run, in ps.
int simulation_steps = 50000;                      //no of discrete time steps in simulation.
simulation_state init_state;                     //Hold the initial state.
simulation_state current_state;                  //Holds the currently processed state, to avoid unnecessary vector operations.
std::vector<simulation_state> simulation_states; //All the simulation states.
long int current_time;

//Symbol Information
std::map<std::string, bool> symbolDoneTable;         //is the value of a symbol accurate in this time frame? always check this table before assigning
std::map<std::string, Signal_Types> symbolTypeTable; //Holds the type of each symbol
std::vector<PortType> current_portlist;              //holds temporary information about ports, which is then written into entity information

//Expressions and visitor returns
Signal_Types visitedType;     //when a visit to a type occurs, this is used to return that type
Expr_state visitedExprState;  //what could the visited Expression be resolved into
int visitedLitInt;            //When a visit to a literal Int occurs, or when a INTEGER expression is resolved into a value, this is used to return the value.
char visitedLitChar;          //When a visit to a literal Char occurs, or when a STD_LOGIC expression is resolved into a value, this is used to return the value.
std::string visitedLitString; //When a visit to a literal String occurs, or when a STD_LOGIC_VECTOR expression is resolved into a value, this is used to return the value.

long int Simulator::convertToPs(int i, std::string u)
{
    long int l = i;
    if (u == "ps")
        return l;
    else if (u == "ns")
        return l * 1000;
    else if (u == "us")
        return l * 1000000;
    else if (u == "us")
        return l * 1000000000;
    else if (u == "us")
        return l * 1000000000000;
    else
    {
        std::cout << "error in convertToPs " << std::endl;
        returnvalue = 1;
        return l;
    }
}

void Simulator::assignSignalfromLit(std::string i)
{
    //This method is used to assing a value to a signal from a resolved expression. If the expression isn't resolved, or the required parameters not set, expect wierd behavior
    //Relies on that visitedLit* has been set properly in the expression that should be assigned into the value.
    switch (symbolTypeTable[i])
    {
    case STD_LOGIC:
        for (auto &s : current_state.std_logics) //the & gives a reference instead of copy, so we can change the value.
        {
            if (s.identifier == i)
            {
                s.value = visitedLitChar;
                std::cout << "Assigning signal " << i << " the value \'" << visitedLitChar << "\'" << std::endl;
            }
        }
        break;
    case STD_LOGIC_VECTOR:
        for (auto &s : current_state.std_logic_vectors) //the & gives a reference instead of copy, so we can change the value.
        {
            if (s.identifier == i)
            {
                s.value = visitedLitString;
                std::cout << "Assigning signal " << i << " the value \'" << visitedLitString << "\'" << std::endl;
            }
        }
        break;
    case INTEGER:
        for (auto &s : current_state.integers) //the & gives a reference instead of copy, so we can change the value.
        {
            if (s.identifier == i)
            {
                s.value = visitedLitInt;
                std::cout << "Assigning signal " << i << " the value \'" << visitedLitInt << "\'" << std::endl;
            }
        }
        break;
    }
}

bool Simulator::check_if_all_signals_are_updated()
{
    //THis method checks if the value of all signals has been updated in the current time frame. used in visitArch
    for (auto t : symbolDoneTable)
    {
        if (!t.second)
        {
            std::cout << "All signals weren't updated, gonna iterate the concurent statements again" << std::endl;
            return false;
        }
    }
    std::cout << "All signals were updated, time frame is complete" << std::endl;
    return true;
}

void Simulator::printState(simulation_state s)
{
    //Used to prettyprint a simulation state
    std::cout << "std_logics:" << std::endl;
    for (auto p : s.std_logics)
    {
        std::cout << p.identifier << "=" << p.value << std::endl;
    }
    std::cout << "std_logic_vectors:" << std::endl;
    for (auto p : s.std_logic_vectors)
    {
        std::cout << p.identifier << "=" << p.value << std::endl;
    }
    std::cout << "Integers:" << std::endl;
    for (auto p : s.integers)
    {
        std::cout << p.identifier << "=" << p.value << std::endl;
    }
    std::cout << "\n"
              << std::endl;
}

int Simulator::startSimulation(Visitable *t)
{
    //This method is pretty self explanatory. Call this and the simulation starts. Boom. Magic.
    //The returnvalue should be 0 on successful simulation

    t->accept(this);
    return returnvalue;
}

void Simulator::visitProgram(Program *program)
{
    /* Code For Program Goes Here */

    program->listtopdef_->accept(this);
}

void Simulator::visitLibrary_Dec(Library_Dec *library_dec)
{
    /* Code For Library_Dec Goes Here */

    visitIdent(library_dec->ident_);
}

void Simulator::visitLibrary_Use(Library_Use *library_use)
{
    /* Code For Library_Use Goes Here */

    visitIdent(library_use->ident_1);
    visitIdent(library_use->ident_2);
    visitIdent(library_use->ident_3);
}

void Simulator::visitEntity(Entity *entity)
{
    /* Code For Entity Goes Here */

    visitIdent(entity->ident_1);
    entity->listports_->accept(this);
    visitIdent(entity->ident_2);

    entities.push_back(EntityType(entity->ident_1, current_portlist));
    current_portlist.clear();
}

void Simulator::visitArch(Arch *arch)
{
    /* Code For Arch Goes Here */

    bool all_signals_are_updated = false;
    EntityType current_entity;
    int currentTime;
    simulation_state current_state_copy;
    int whilecounter = 1;
    //visitIdent(arch->ident_1);
    //visitIdent(arch->ident_2);
    //visitIdent(arch->ident_3);

    //find current entity
    for (auto e : entities)
    {
        if (e.label == arch->ident_2)
        {
            current_entity = e;
        }
    }

    //add ports to state and type table

    for (auto p : current_entity.ports)
    {
        if (p.type == STD_LOGIC)
        {
            init_state.std_logics.push_back(std_logic_state(p.identifier, 'X'));
            symbolTypeTable[p.identifier] = p.type;
        }
        else if (p.type == STD_LOGIC_VECTOR)
        {
            init_state.std_logic_vectors.push_back(std_logic_vector_state(p.identifier, "X"));
            symbolTypeTable[p.identifier] = p.type;
        }
        else //INTEGER
        {
            init_state.integers.push_back(integer_state(p.identifier, -2147483648));
            symbolTypeTable[p.identifier] = p.type;
        }
    }

    //add all Signal declarations to state
    arch->listpre_begin_statements_->accept(this);
    //done.

    std::cout << "initialization state : " << std::endl;
    printState(init_state);
    simulation_states.push_back(init_state);

    //now all signals should be initialized. lets move on to simulation

    for (int i = 1; i < simulation_steps + 1; i++)
    {
        current_state = simulation_states.at(i - 1);
        current_time = i * simulation_time / simulation_steps;
        for (auto &a : symbolDoneTable)
            a.second = false;
        all_signals_are_updated = false;

        std::cout << "--------------------Simulation state "<< i << "------------------------" << std::endl;
        whilecounter = 0;
        while (!all_signals_are_updated)
        {
            std::cout << "Step Iteration "  << whilecounter << " events:"<< std::endl;
            whilecounter++;
            //current_state_copy = current_state;
            arch->listpost_begin_statements_->accept(this);
            all_signals_are_updated = check_if_all_signals_are_updated();

            //kontrollera currstate efter förändringar
        }
        simulation_states.push_back(current_state);
        std::cout << "\nCompleted state:" << std::endl;
        printState(current_state);
    }

}

void Simulator::visitInport(Inport *inport)
{
    /* Code For Inport Goes Here */

    visitIdent(inport->ident_);
    inport->type_->accept(this);
    current_portlist.push_back(PortType(inport->ident_, visitedType, IN));
}

void Simulator::visitOutport(Outport *outport)
{
    /* Code For Outport Goes Here */

    visitIdent(outport->ident_);
    outport->type_->accept(this);
    current_portlist.push_back(PortType(outport->ident_, visitedType, OUT));
}

void Simulator::visitInoutport(Inoutport *inoutport)
{
    /* Code For Inoutport Goes Here */

    visitIdent(inoutport->ident_);
    inoutport->type_->accept(this);
    current_portlist.push_back(PortType(inoutport->ident_, visitedType, INOUT));
}

void Simulator::visitSignal_Decl(Signal_Decl *signal_decl)
{
    /* Code For Signal_Decl Goes Here */

    visitIdent(signal_decl->ident_);
    signal_decl->type_->accept(this);
    if (visitedType == STD_LOGIC)
    {
        init_state.std_logics.push_back(std_logic_state(signal_decl->ident_, 'X'));
        symbolTypeTable[signal_decl->ident_] = STD_LOGIC;
    }
    else if (visitedType == STD_LOGIC_VECTOR)
    {
        init_state.std_logic_vectors.push_back(std_logic_vector_state(signal_decl->ident_, "X"));
        symbolTypeTable[signal_decl->ident_] = STD_LOGIC_VECTOR;
    }
    else //INTEGER
    {
        init_state.integers.push_back(integer_state(signal_decl->ident_, -2147483648));
        symbolTypeTable[signal_decl->ident_] = INTEGER;
    }
}

void Simulator::visitSignal_Decl_W_Assign(Signal_Decl_W_Assign *signal_decl_w_assign)
{
    /* Code For Signal_Decl_W_Assign Goes Here */

    visitIdent(signal_decl_w_assign->ident_);
    signal_decl_w_assign->type_->accept(this);
    signal_decl_w_assign->literal_->accept(this);
    if (visitedType == STD_LOGIC)
    {
        init_state.std_logics.push_back(std_logic_state(signal_decl_w_assign->ident_, visitedLitChar));
        symbolTypeTable[signal_decl_w_assign->ident_] = STD_LOGIC;
    }
    else if (visitedType == STD_LOGIC_VECTOR)
    {
        init_state.std_logic_vectors.push_back(std_logic_vector_state(signal_decl_w_assign->ident_, visitedLitString));
        symbolTypeTable[signal_decl_w_assign->ident_] = STD_LOGIC_VECTOR;
    }
    else //INTEGER
    {
        init_state.integers.push_back(integer_state(signal_decl_w_assign->ident_, visitedLitInt));
        symbolTypeTable[signal_decl_w_assign->ident_] = INTEGER;
    }
}

void Simulator::visitConstant_Decl(Constant_Decl *constant_decl)
{
    /* This one is never going to happen? why would you declare a constant without value...*/

    visitIdent(constant_decl->ident_);
    constant_decl->type_->accept(this);
}
void Simulator::visitConstant_Decl_W_Assign(Constant_Decl_W_Assign *constant_decl_w_assign)
{
    /* Code For Constant_Decl_W_Assign Goes Here */

    visitIdent(constant_decl_w_assign->ident_);
    constant_decl_w_assign->type_->accept(this);
    constant_decl_w_assign->literal_->accept(this);

    if (visitedType == STD_LOGIC)
    {
        init_state.std_logics.push_back(std_logic_state(constant_decl_w_assign->ident_, visitedLitChar));
    }
    else if (visitedType == STD_LOGIC_VECTOR)
    {
        init_state.std_logic_vectors.push_back(std_logic_vector_state(constant_decl_w_assign->ident_, visitedLitString));
    }
    else //INTEGER
    {
        init_state.integers.push_back(integer_state(constant_decl_w_assign->ident_, visitedLitInt));
    }
}

void Simulator::visitComponent(Component *component)
{
    /* Code For Component Goes Here */

    visitIdent(component->ident_1);
    component->listports_->accept(this);
    visitIdent(component->ident_2);
}

void Simulator::visitConcurrent_Stm(Concurrent_Stm *concurrent_stm)
{
    /* Code For Concurrent_Stm Goes Here */

    concurrent_stm->concurrent_statement_->accept(this);
}

void Simulator::visitPort_Map(Port_Map *port_map)
{
    /* Code For Port_Map Goes Here */

    visitIdent(port_map->ident_1);
    visitIdent(port_map->ident_2);
    port_map->listpmap_stm_->accept(this);
}

void Simulator::visitPort_Map_Internal_Statement(Port_Map_Internal_Statement *port_map_internal_statement)
{
    /* Code For Port_Map_Internal_Statement Goes Here */

    visitIdent(port_map_internal_statement->ident_1);
    visitIdent(port_map_internal_statement->ident_2);
}

void Simulator::visitConcurrent_Assignment(Concurrent_Assignment *concurrent_assignment)
{
    /* Code For Concurrent_Assignment Goes Here */

    if (symbolDoneTable[concurrent_assignment->ident_] == false)
    {
        //The symbol haven't been updated for this timeframe and we should therefore try to do that
        concurrent_assignment->exp_->accept(this);
        if (visitedExprState == VALUE)
        {
            //then we can do an assign and mark the symbol as done.
            assignSignalfromLit(concurrent_assignment->ident_);
            symbolDoneTable[concurrent_assignment->ident_] = true;
        }
        else if (visitedExprState == SYMBOL)
        {
            //then we need to look up if the symbol is done
            //This state isn't currently used
        }
        else
        {
            //then the visitedExprState is UNRESOLVED and we cant do anything this run
            std::cout << "Signal " << concurrent_assignment->ident_ << " couldnt be assigned yet due to depending on signals that arent done being simulated." << std::endl;
        }
    }
    else
    {
        //the symbol has already been updated and should therefore not be touched.
        std::cout << "Skipping assignment to " << concurrent_assignment->ident_ << " since it is marked as done." << std::endl;
    }
}

void Simulator::visitConcurrent_Assignment_W_AFTER(Concurrent_Assignment_W_AFTER *concurrent_assignment_w_after)
{
    /* Code For Concurrent_Assignment_W_AFTER Goes Here */
    //visitIdent(concurrent_assignment_w_after->ident_);

    if (symbolDoneTable[concurrent_assignment_w_after->ident_] == false)
    {
        //The symbol haven't been updated for this timeframe and we should therefore try to do that
        concurrent_assignment_w_after->exp_->accept(this);
        concurrent_assignment_w_after->listass_statements_->accept(this);

        if (visitedExprState == VALUE)
        {
            //then we can do an assign and mark the symbol as done.
            assignSignalfromLit(concurrent_assignment_w_after->ident_);
            symbolDoneTable[concurrent_assignment_w_after->ident_] = true;
        }
        else if (visitedExprState == SYMBOL)
        {
            //then we need to look up if the symbol is done
            //This state isn't currently used
        }
        else
        {
            //then the visitedExprState is UNRESOLVED and we cant do anything this run
            std::cout << "Signal " << concurrent_assignment_w_after->ident_ << " couldnt be assigned yet due to depending on signals that arent done being simulated." << std::endl;
        }
    }
    else
    {
        //the symbol has already been updated and should therefore not be touched.
        std::cout << "Skipping assignment to " << concurrent_assignment_w_after->ident_ << " since it is marked as done." << std::endl;
    }
}

void Simulator::visitWhen_Statement(When_Statement *when_statement)
{
    /* Code For When_Statement Goes Here */

    when_statement->exp_->accept(this);
    when_statement->listwhen_part_->accept(this);
}

void Simulator::visitProcess_Statement(Process_Statement *process_statement)
{
    /* Code For Process_Statement Goes Here */

    visitIdent(process_statement->ident_);
    process_statement->listident_->accept(this);
    process_statement->listsequential_statement_->accept(this);
}

void Simulator::visitAfter_Component(After_Component *after_component)
{
    /* Code For After_Component Goes Here */

    if (convertToPs(after_component->integer_, after_component->ident_) <= current_time)
    {
        after_component->exp_->accept(this);
    }
}

void Simulator::visitWhen_Component(When_Component *when_component)
{
    /* Code For When_Component Goes Here */

    when_component->exp_1->accept(this);
    visitIdent(when_component->ident_);
    when_component->exp_2->accept(this);
}

void Simulator::visitWhen_Finisher(When_Finisher *when_finisher)
{
    /* Code For When_Finisher Goes Here */

    when_finisher->exp_1->accept(this);
    visitIdent(when_finisher->ident_);
    when_finisher->exp_2->accept(this);
}

void Simulator::visitCase_Statement(Case_Statement *case_statement)
{
    /* Code For Case_Statement Goes Here */

    case_statement->exp_->accept(this);
    case_statement->listwhens_->accept(this);
}

void Simulator::visitIf_Statement(If_Statement *if_statement)
{
    /* Code For If_Statement Goes Here */

    if_statement->exp_1->accept(this);
    if_statement->exp_2->accept(this);
    if_statement->listsequential_statement_->accept(this);
}

void Simulator::visitIf_Else_Statement(If_Else_Statement *if_else_statement)
{
    /* Code For If_Else_Statement Goes Here */

    if_else_statement->exp_1->accept(this);
    if_else_statement->exp_2->accept(this);
    if_else_statement->listsequential_statement_1->accept(this);
    if_else_statement->listsequential_statement_2->accept(this);
}

void Simulator::visitWait_For_Statement(Wait_For_Statement *wait_for_statement)
{
    /* Code For Wait_For_Statement Goes Here */

    visitInteger(wait_for_statement->integer_);
    visitIdent(wait_for_statement->ident_);
}

void Simulator::visitSeq_Assignment(Seq_Assignment *seq_assignment)
{
    /* Code For Seq_Assignment Goes Here */

    visitIdent(seq_assignment->ident_);
    seq_assignment->exp_->accept(this);
}

void Simulator::visitCase_Case(Case_Case *case_case)
{
    /* Code For Case_Case Goes Here */

    case_case->exp_->accept(this);
    case_case->listsequential_statement_->accept(this);
}

void Simulator::visitE_Identifier(E_Identifier *e_identifier)
{
    //If the symbol has a value, set the expression status to VALUE and return symbol value. Otherwise set as UNRESOLVED
    std::string i = e_identifier->ident_;
    if (symbolDoneTable[i])
    {
        switch (symbolTypeTable[i])
        {
        case STD_LOGIC:
            visitedType = STD_LOGIC;
            for (auto s : current_state.std_logics)
            {
                if (s.identifier == i)
                {
                    visitedLitChar = s.value;
                }
            }
            break;
        case STD_LOGIC_VECTOR:
            visitedType = STD_LOGIC_VECTOR;
            for (auto s : current_state.std_logic_vectors)
            {
                if (s.identifier == i)
                {
                    visitedLitString = s.value;
                }
            }
            break;
        case INTEGER:
            visitedType = INTEGER;
            for (auto s : current_state.integers)
            {
                if (s.identifier == i)
                {
                    visitedLitInt = s.value;
                }
            }
            break;
        }
        visitedExprState = VALUE;
    }
    else
        visitedExprState = UNRESOLVED;
}

void Simulator::visitE_String(E_String *e_string)
{
    /* Code For E_String Goes Here */

    visitString(e_string->string_);
}

void Simulator::visitE_Integer(E_Integer *e_integer)
{
    /* Code For E_Integer Goes Here */

    visitInteger(e_integer->integer_);
}

void Simulator::visitE_Std_Logic(E_Std_Logic *e_std_logic)
{
    /* Code For E_Std_Logic Goes Here */

    visitChar(e_std_logic->char_);
}

void Simulator::visitE_Vector_Ref(E_Vector_Ref *e_vector_ref)
{
    /* Code For E_Vector_Ref Goes Here */

    visitIdent(e_vector_ref->ident_);
    visitInteger(e_vector_ref->integer_);
}

void Simulator::visitE_Add(E_Add *e_add)
{
    /* Code For E_Add Goes Here */

    e_add->exp_1->accept(this);
    e_add->exp_2->accept(this);
}

void Simulator::visitE_Sub(E_Sub *e_sub)
{
    /* Code For E_Sub Goes Here */

    e_sub->exp_1->accept(this);
    e_sub->exp_2->accept(this);
}
std::string Simulator::invertString(std::string s)
{
    for(char& c : s) {
        if(c == '0')
            c = '1';
        else if(c == '1')
            c = '0';
    }
    return s;
}

void Simulator::visitE_Not(E_Not *e_not)
{
    /* Check if incoming expression is value, if so, invert value and keep VALUE status. if not, do nothing */

    e_not->exp_->accept(this);
    if (visitedExprState == VALUE)
    {
        switch (visitedType)
        {
        case STD_LOGIC:
            if (visitedLitChar == '1')
                visitedLitChar = '0';
            else if (visitedLitChar == '0')
                visitedLitChar = '1';
            break;
        case STD_LOGIC_VECTOR:
            visitedLitString = invertString(visitedLitString);
            break;
        case INTEGER:
            std::cout << YELLOWTEXT << "WARNING: " << NORMTEXT << "NOT operator used on integer, Ignoring it" << std::endl;
            break;
        }
    }
}

void Simulator::visitE_Cmp(E_Cmp *e_cmp)
{
    /* Code For E_Cmp Goes Here */

    e_cmp->exp_1->accept(this);
    e_cmp->cmpop_->accept(this);
    e_cmp->exp_2->accept(this);
}

void Simulator::visitE_Equal(E_Equal *e_equal)
{
    /* Code For E_Equal Goes Here */
}

void Simulator::visitE_Less(E_Less *e_less)
{
    /* Code For E_Less Goes Here */
}

void Simulator::visitE_More(E_More *e_more)
{
    /* Code For E_More Goes Here */
}

void Simulator::visitLit_string(Lit_string *lit_string)
{
    /* Code For Lit_string Goes Here */
    std::cout << "visited lit string" << std::endl;
    visitString(lit_string->string_);
    visitedLitString = lit_string->string_;
    visitedExprState = VALUE;
}

void Simulator::visitLit_int(Lit_int *lit_int)
{
    /* Code For Lit_int Goes Here */
    visitInteger(lit_int->integer_);
    visitedLitInt = lit_int->integer_;
    visitedExprState = VALUE;
}

void Simulator::visitLit_char(Lit_char *lit_char)
{
    /* Code For Lit_char Goes Here */
    std::cout << "visited lit char" << std::endl;
    visitChar(lit_char->char_);
    visitedLitChar = lit_char->char_;
    visitedExprState = VALUE;
}

void Simulator::visitT_std_logic(T_std_logic *t_std_logic)
{
    visitedType = STD_LOGIC;
}

void Simulator::visitT_std_logic_vector(T_std_logic_vector *t_std_logic_vector)
{
    /* Code For T_std_logic_vector Goes Here */

    //visitInteger(t_std_logic_vector->integer_1);
    //visitInteger(t_std_logic_vector->integer_2);
    visitedType = STD_LOGIC_VECTOR;
}

void Simulator::visitT_integer(T_integer *t_integer)
{
    visitedType = INTEGER;
}

void Simulator::visitListTopDef(ListTopDef *listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin(); i != listtopdef->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Simulator::visitListPorts(ListPorts *listports)
{
    for (ListPorts::iterator i = listports->begin(); i != listports->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Simulator::visitListPre_begin_statements(ListPre_begin_statements *listpre_begin_statements)
{
    for (ListPre_begin_statements::iterator i = listpre_begin_statements->begin(); i != listpre_begin_statements->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Simulator::visitListPost_begin_statements(ListPost_begin_statements *listpost_begin_statements)
{
    for (ListPost_begin_statements::iterator i = listpost_begin_statements->begin(); i != listpost_begin_statements->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Simulator::visitListPmap_stm(ListPmap_stm *listpmap_stm)
{
    for (ListPmap_stm::iterator i = listpmap_stm->begin(); i != listpmap_stm->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Simulator::visitListAss_statements(ListAss_statements *listass_statements)
{
    for (ListAss_statements::iterator i = listass_statements->begin(); i != listass_statements->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Simulator::visitListWhen_part(ListWhen_part *listwhen_part)
{
    for (ListWhen_part::iterator i = listwhen_part->begin(); i != listwhen_part->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Simulator::visitListIdent(ListIdent *listident)
{
    for (ListIdent::iterator i = listident->begin(); i != listident->end(); ++i)
    {
        visitIdent(*i);
    }
}

void Simulator::visitListWhens(ListWhens *listwhens)
{
    for (ListWhens::iterator i = listwhens->begin(); i != listwhens->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Simulator::visitListSequential_statement(ListSequential_statement *listsequential_statement)
{
    for (ListSequential_statement::iterator i = listsequential_statement->begin(); i != listsequential_statement->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Simulator::visitInteger(Integer x)
{
    std::cout << "visited integer" << x << std::endl;
    visitedLitInt = x;
    visitedExprState = VALUE;
    visitedType = INTEGER;
}

void Simulator::visitChar(Char x)
{
    visitedLitChar = x;
    visitedExprState = VALUE;
    visitedType = STD_LOGIC;
}

void Simulator::visitDouble(Double x)
{
    /* Code for Double Goes Here */
}

void Simulator::visitString(String x)
{
    visitedLitString = x;
    visitedExprState = VALUE;
    visitedType = STD_LOGIC_VECTOR;
}

void Simulator::visitIdent(Ident x)
{
    /* Code for Ident Goes Here */
}
