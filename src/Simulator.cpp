/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Simulator.H"
#include "Types.H"
#include <iostream>
#include <map>

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
std::vector<PortType> current_portlist; //holds temporary information about ports, which is then written into entity information
Signal_Types visitedType;               //when a visit to a type occurs, this
int visitedLitInt;
char visitedLitChar;
std::string visitedLitString;

int simulation_time;         //time the simulation should run, in ps
int simulation_steps = 1000; //no of discrete time steps in simulation.

std::map<std::string, bool> symbolDoneTable;        //is the value of a symbol accurate in this time frame? always check this table before assigning
std::vector<simulation_state> simulation_states;    //All the simulation states.
simulation_state current_state;                     //
int current_time;

Expr_state visitedExprState;

simulation_state init_state;
bool Simulator::check_if_all_signals_are_updated(std::map<std::string, bool> t)
{
    for (auto t : symbolDoneTable)
    {
        if (!t.second)
            return false;
    }
    return true;
}

void Simulator::printState(simulation_state s)
{
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
}

int Simulator::startSimulation(Visitable *t)
{

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

    bool all_signals_are_updated;
    EntityType current_entity;
    int currentTime;
    simulation_state current_state_copy;
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

    //add ports to state

    for (auto p : current_entity.ports)
    {
        if (p.type == STD_LOGIC)
        {
            init_state.std_logics.push_back(std_logic_state(p.identifier, 'X'));
        }
        else if (p.type == STD_LOGIC_VECTOR)
        {
            init_state.std_logic_vectors.push_back(std_logic_vector_state(p.identifier, "X"));
        }
        else //INTEGER
        {
            init_state.integers.push_back(integer_state(p.identifier, -2147483648));
        }
    }

    //add all Signal declarations to state
    arch->listpre_begin_statements_->accept(this);
    //done.

    std::cout << "initialization state : " << std::endl;
    printState(init_state);

    //now all signals should be initialized. lets move on to simulation

    //for (int i = 1; i < simulation_steps + 1; i++)
    //{
    //    if(i == 1)
    //        current_state = init_state;
    //    else
    //        current_state = simulation_states.at(i - 1);
//
    //    current_time = i * simulation_time / simulation_steps;
//
    //    while (!all_signals_are_updated)
    //    {
    //        current_state_copy = current_state;
//
    //        arch->listpost_begin_statements_->accept(this);
    //        //all_signals_are_updated = check_if_all_signals_are_updated();
//
    //        //kontrollera currstate efter förändringar
    //    
    //    }
    //}
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
    }
    else if (visitedType == STD_LOGIC_VECTOR)
    {
        init_state.std_logic_vectors.push_back(std_logic_vector_state(signal_decl->ident_, "X"));
    }
    else //INTEGER
    {
        init_state.integers.push_back(integer_state(signal_decl->ident_, -2147483648));
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
    }
    else if (visitedType == STD_LOGIC_VECTOR)
    {
        init_state.std_logic_vectors.push_back(std_logic_vector_state(signal_decl_w_assign->ident_, visitedLitString));
    }
    else //INTEGER
    {
        init_state.integers.push_back(integer_state(signal_decl_w_assign->ident_, visitedLitInt));
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

    concurrent_assignment->exp_1->accept(this); 
    //if(visitedExprState == UNRESOLVED){
    //};
    concurrent_assignment->exp_2->accept(this);
}

void Simulator::visitConcurrent_Assignment_W_AFTER(Concurrent_Assignment_W_AFTER *concurrent_assignment_w_after)
{
    /* Code For Concurrent_Assignment_W_AFTER Goes Here */

    concurrent_assignment_w_after->exp_1->accept(this);
    concurrent_assignment_w_after->exp_2->accept(this);
    concurrent_assignment_w_after->listass_statements_->accept(this);
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

    after_component->exp_->accept(this);
    visitInteger(after_component->integer_);
    visitIdent(after_component->ident_);
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

    seq_assignment->exp_1->accept(this);
    seq_assignment->exp_2->accept(this);
}

void Simulator::visitCase_Case(Case_Case *case_case)
{
    /* Code For Case_Case Goes Here */

    case_case->exp_->accept(this);
    case_case->listsequential_statement_->accept(this);
}

void Simulator::visitE_Identifier(E_Identifier *e_identifier)
{
    /* Code For E_Identifier Goes Here */

    visitIdent(e_identifier->ident_);
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

void Simulator::visitE_Not(E_Not *e_not)
{
    /* Code For E_Not Goes Here */

    e_not->exp_->accept(this);
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

    visitString(lit_string->string_);
    visitedLitString = lit_string->string_;
}

void Simulator::visitLit_int(Lit_int *lit_int)
{
    /* Code For Lit_int Goes Here */
    visitInteger(lit_int->integer_);
    visitedLitInt = lit_int->integer_;
}

void Simulator::visitLit_char(Lit_char *lit_char)
{
    /* Code For Lit_char Goes Here */

    visitChar(lit_char->char_);
    visitedLitChar = lit_char->char_;
}

void Simulator::visitT_std_logic(T_std_logic *t_std_logic)
{
    visitedType = STD_LOGIC;
}

void Simulator::visitT_std_logic_vector(T_std_logic_vector *t_std_logic_vector)
{
    /* Code For T_std_logic_vector Goes Here */

    visitInteger(t_std_logic_vector->integer_1);
    visitInteger(t_std_logic_vector->integer_2);
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
    /* Code for Integer Goes Here */
}

void Simulator::visitChar(Char x)
{
    /* Code for Char Goes Here */
}

void Simulator::visitDouble(Double x)
{
    /* Code for Double Goes Here */
}

void Simulator::visitString(String x)
{
    /* Code for String Goes Here */
}

void Simulator::visitIdent(Ident x)
{
    /* Code for Ident Goes Here */
}