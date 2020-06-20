/*** BNFC-Generated Visitor Design Pattern RuleCheck. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "RuleCheck.H"
#include <iostream>
#include <algorithm>

#define REDTEXT "\033[1;31m"
#define NORMTEXT "\033[0m"
 
void RuleCheck::visitProg(Prog *t) {}                                   //abstract class
void RuleCheck::visitTopDef(TopDef *t) {}                               //abstract class
void RuleCheck::visitPorts(Ports *t) {}                                 //abstract class
void RuleCheck::visitPre_begin_statements(Pre_begin_statements *t) {}   //abstract class
void RuleCheck::visitPost_begin_statements(Post_begin_statements *t) {} //abstract class
void RuleCheck::visitPmap_stm(Pmap_stm *t) {}                           //abstract class
void RuleCheck::visitConcurrent_statement(Concurrent_statement *t) {}   //abstract class
void RuleCheck::visitAss_statements(Ass_statements *t) {}               //abstract class
void RuleCheck::visitWhen_part(When_part *t) {}                         //abstract class
void RuleCheck::visitSequential_statement(Sequential_statement *t) {}   //abstract class
void RuleCheck::visitWhens(Whens *t) {}                                 //abstract class
void RuleCheck::visitExp(Exp *t) {}                                     //abstract class
void RuleCheck::visitCmpop(Cmpop *t) {}                                 //abstract class
void RuleCheck::visitLiteral(Literal *t) {}                             //abstract class
void RuleCheck::visitType(Type *t) {}                                   //abstract class

int returnvalue = 0;               // set this to one if errors are detected.
std::vector<std::string> entities; //List of entities in the file.
std::vector<ArchType> archs;       //List of Architectures in file.
std::vector<PortType> entity_ports;  //List of ports in the entity scope. (scopes not implemented yet)
std::vector<std::string>  symbols; //list of all declared symbols, e.g. ports and signals, in current scope. (scope not implemented yet)
Signal_Types visitedType;

int RuleCheck::startRuleCheck(Visitable *t)
{
    t->accept(this);
    return returnvalue;
}

void RuleCheck::checkSymbolUniqueness(std::string s)
{
    for (std::string x : symbols)
    {
        if (x == s){
            std::cout << REDTEXT << "ERROR: " << NORMTEXT << "Symbol " + s + " declared more than once!" << std::endl;
        }
    }
}


void RuleCheck::visitProgram(Program *program)
{
    /* Code For Program Goes Here */

    program->listtopdef_->accept(this);
}

void RuleCheck::visitLibrary_Dec(Library_Dec *library_dec)
{
    /* Code For Library_Dec Goes Here */

    //visitIdent(library_dec->ident_);
}

void RuleCheck::visitLibrary_Use(Library_Use *library_use)
{
    /* Code For Library_Use Goes Here */

    //visitIdent(library_use->ident_1);
    //visitIdent(library_use->ident_2);
    //visitIdent(library_use->ident_3);
}

void RuleCheck::visitEntity(Entity *entity)
{
    /* Checks for lables missmatch and entity uniqeness */

    //visitIdent(entity->ident_1);
    //visitIdent(entity->ident_2);
    if (entity->ident_1 != entity->ident_2)
    {
        std::cout << REDTEXT << "ERROR: " << NORMTEXT <<+ "END label in Entity " << entity->ident_1 << " did not match entity name!" << std::endl;
        returnvalue = 1;
    }

    for (std::string s : entities)
    {
        if (s == entity->ident_1)
        {
            std::cout << "Error: Several entities have the same label" << std::endl;
            returnvalue = 1;
        }
    }
    entities.push_back(entity->ident_1);

    entity->listports_->accept(this);
}

void RuleCheck::visitArch(Arch *arch)
{
    /* Needs checks for:
        - End label need to match beginning label. DONE
        - The entity refered to must exist. DONE
        - One entity cannot have 2 architectures with the same label. DONE
    */

    if (arch->ident_1 != arch->ident_3)
    {
        std::cout << "Error: END label in Arch " << arch->ident_1 << " did not match arch!" << std::endl;
        returnvalue = 1;
    }

    if (std::find(entities.begin(), entities.end(), arch->ident_2) == entities.end())
    { //works since end() is not a element in the vector, but rather the hypoteticall nex one.
        std::cout << "Error: The Entity" << arch->ident_2 << " pointed to by architecture " << arch->ident_1 << " does not exist (yet)!" << std::endl;
    }

    for (ArchType a : archs)
    {
        if ((a.label == arch->ident_1) && (a.entity == arch->ident_2))
        {
            std::cout << "Error: Several architectures of the same entity share labels" << std::endl;
            returnvalue = 1;
        }
    }
    archs.push_back(ArchType(arch->ident_1, arch->ident_2));

    visitIdent(arch->ident_1);
    visitIdent(arch->ident_2);
    arch->listpre_begin_statements_->accept(this);
    arch->listpost_begin_statements_->accept(this);
    visitIdent(arch->ident_3);
}

void RuleCheck::visitInport(Inport *inport)
{
    /* Needs checks for:
        - same port declared more than once
    everything else is handled by lexer/parser since i made putting the wrong type a syntax error
    */

    visitIdent(inport->ident_);
    inport->type_->accept(this);
    checkSymbolUniqueness(inport->ident_);
    symbols.push_back(inport->ident_);
    entity_ports.push_back(PortType(inport->ident_, visitedType, IN));
}

void RuleCheck::visitOutport(Outport *outport)
{
    /* Code For Outport Goes Here */

    visitIdent(outport->ident_);
    outport->type_->accept(this);

    checkSymbolUniqueness(outport->ident_);
    symbols.push_back(outport->ident_);
    entity_ports.push_back(PortType(outport->ident_, visitedType, OUT));
}

void RuleCheck::visitInoutport(Inoutport *inoutport)
{
    /* Code For Inoutport Goes Here */

    visitIdent(inoutport->ident_);
    inoutport->type_->accept(this);

    checkSymbolUniqueness(inoutport->ident_);
    symbols.push_back(inoutport->ident_);
    entity_ports.push_back(PortType(inoutport->ident_, visitedType, INOUT));
}

void RuleCheck::visitSignal_Decl(Signal_Decl *signal_decl)
{
    /* Code For Signal_Decl Goes Here */

    visitIdent(signal_decl->ident_);
    signal_decl->type_->accept(this);
}

void RuleCheck::visitSignal_Decl_W_Assign(Signal_Decl_W_Assign *signal_decl_w_assign)
{
    /* Code For Signal_Decl_W_Assign Goes Here */

    visitIdent(signal_decl_w_assign->ident_);
    signal_decl_w_assign->type_->accept(this);
    signal_decl_w_assign->literal_->accept(this);
}

void RuleCheck::visitConstant_Decl(Constant_Decl *constant_decl)
{
    /* Code For Constant_Decl Goes Here */

    visitIdent(constant_decl->ident_);
    constant_decl->type_->accept(this);
}

void RuleCheck::visitConstant_Decl_W_Assign(Constant_Decl_W_Assign *constant_decl_w_assign)
{
    /* Code For Constant_Decl_W_Assign Goes Here */

    visitIdent(constant_decl_w_assign->ident_);
    constant_decl_w_assign->type_->accept(this);
    constant_decl_w_assign->literal_->accept(this);
}

void RuleCheck::visitComponent(Component *component)
{
    /* Code For Component Goes Here */

    visitIdent(component->ident_1);
    component->listports_->accept(this);
    visitIdent(component->ident_2);
}

void RuleCheck::visitConcurrent_Stm(Concurrent_Stm *concurrent_stm)
{
    /* Code For Concurrent_Stm Goes Here */

    concurrent_stm->concurrent_statement_->accept(this);
}

void RuleCheck::visitPort_Map(Port_Map *port_map)
{
    /* Code For Port_Map Goes Here */

    visitIdent(port_map->ident_1);
    visitIdent(port_map->ident_2);
    port_map->listpmap_stm_->accept(this);
}

void RuleCheck::visitPort_Map_Internal_Statement(Port_Map_Internal_Statement *port_map_internal_statement)
{
    /* Code For Port_Map_Internal_Statement Goes Here */

    visitIdent(port_map_internal_statement->ident_1);
    visitIdent(port_map_internal_statement->ident_2);
}

void RuleCheck::visitConcurrent_Assignment(Concurrent_Assignment *concurrent_assignment)
{
    /* Code For Concurrent_Assignment Goes Here */

    concurrent_assignment->exp_1->accept(this);
    concurrent_assignment->exp_2->accept(this);
}

void RuleCheck::visitConcurrent_Assignment_W_AFTER(Concurrent_Assignment_W_AFTER *concurrent_assignment_w_after)
{
    /* Code For Concurrent_Assignment_W_AFTER Goes Here */

    concurrent_assignment_w_after->exp_1->accept(this);
    concurrent_assignment_w_after->exp_2->accept(this);
    concurrent_assignment_w_after->listass_statements_->accept(this);
}

void RuleCheck::visitWhen_Statement(When_Statement *when_statement)
{
    /* Code For When_Statement Goes Here */

    when_statement->exp_->accept(this);
    when_statement->listwhen_part_->accept(this);
}

void RuleCheck::visitProcess_Statement(Process_Statement *process_statement)
{
    /* Code For Process_Statement Goes Here */

    visitIdent(process_statement->ident_);
    process_statement->listident_->accept(this);
    process_statement->listsequential_statement_->accept(this);
}

void RuleCheck::visitAfter_Component(After_Component *after_component)
{
    /* Code For After_Component Goes Here */

    after_component->exp_->accept(this);
    visitInteger(after_component->integer_);
    visitIdent(after_component->ident_);
}

void RuleCheck::visitWhen_Component(When_Component *when_component)
{
    /* Code For When_Component Goes Here */

    when_component->exp_1->accept(this);
    visitIdent(when_component->ident_);
    when_component->exp_2->accept(this);
}

void RuleCheck::visitWhen_Finisher(When_Finisher *when_finisher)
{
    /* Code For When_Finisher Goes Here */

    when_finisher->exp_1->accept(this);
    visitIdent(when_finisher->ident_);
    when_finisher->exp_2->accept(this);
}

void RuleCheck::visitCase_Statement(Case_Statement *case_statement)
{
    /* Code For Case_Statement Goes Here */

    case_statement->exp_->accept(this);
    case_statement->listwhens_->accept(this);
}

void RuleCheck::visitIf_Statement(If_Statement *if_statement)
{
    /* Code For If_Statement Goes Here */

    if_statement->exp_1->accept(this);
    if_statement->exp_2->accept(this);
    if_statement->listsequential_statement_->accept(this);
}

void RuleCheck::visitIf_Else_Statement(If_Else_Statement *if_else_statement)
{
    /* Code For If_Else_Statement Goes Here */

    if_else_statement->exp_1->accept(this);
    if_else_statement->exp_2->accept(this);
    if_else_statement->listsequential_statement_1->accept(this);
    if_else_statement->listsequential_statement_2->accept(this);
}

void RuleCheck::visitWait_For_Statement(Wait_For_Statement *wait_for_statement)
{
    /* Code For Wait_For_Statement Goes Here */

    visitInteger(wait_for_statement->integer_);
    visitIdent(wait_for_statement->ident_);
}

void RuleCheck::visitSeq_Assignment(Seq_Assignment *seq_assignment)
{
    /* Code For Seq_Assignment Goes Here */

    seq_assignment->exp_1->accept(this);
    seq_assignment->exp_2->accept(this);
}

void RuleCheck::visitCase_Case(Case_Case *case_case)
{
    /* Code For Case_Case Goes Here */

    case_case->exp_->accept(this);
    case_case->listsequential_statement_->accept(this);
}

void RuleCheck::visitE_Identifier(E_Identifier *e_identifier)
{
    /* Code For E_Identifier Goes Here */

    visitIdent(e_identifier->ident_);
    //std::cout << "ident:" << e_identifier->ident_ << std::endl;
}

void RuleCheck::visitE_String(E_String *e_string)
{
    /* Code For E_String Goes Here */

    visitString(e_string->string_);
}

void RuleCheck::visitE_Integer(E_Integer *e_integer)
{
    /* Code For E_Integer Goes Here */

    visitInteger(e_integer->integer_);
}

void RuleCheck::visitE_Std_Logic(E_Std_Logic *e_std_logic)
{
    /* Code For E_Std_Logic Goes Here */

    visitChar(e_std_logic->char_);
}

void RuleCheck::visitE_Vector_Ref(E_Vector_Ref *e_vector_ref)
{
    /* Code For E_Vector_Ref Goes Here */

    visitIdent(e_vector_ref->ident_);
    visitInteger(e_vector_ref->integer_);
}

void RuleCheck::visitE_Add(E_Add *e_add)
{
    /* Code For E_Add Goes Here */

    e_add->exp_1->accept(this);
    e_add->exp_2->accept(this);
}

void RuleCheck::visitE_Sub(E_Sub *e_sub)
{
    /* Code For E_Sub Goes Here */

    e_sub->exp_1->accept(this);
    e_sub->exp_2->accept(this);
}

void RuleCheck::visitE_Not(E_Not *e_not)
{
    /* Code For E_Not Goes Here */

    e_not->exp_->accept(this);
}

void RuleCheck::visitE_Cmp(E_Cmp *e_cmp)
{
    /* Code For E_Cmp Goes Here */

    e_cmp->exp_1->accept(this);
    e_cmp->cmpop_->accept(this);
    e_cmp->exp_2->accept(this);
}

void RuleCheck::visitE_Equal(E_Equal *e_equal)
{
    /* Code For E_Equal Goes Here */
}

void RuleCheck::visitE_Less(E_Less *e_less)
{
    /* Code For E_Less Goes Here */
}

void RuleCheck::visitE_More(E_More *e_more)
{
    /* Code For E_More Goes Here */
}

void RuleCheck::visitLit_string(Lit_string *lit_string)
{
    /* Code For Lit_string Goes Here */

    visitString(lit_string->string_);
}

void RuleCheck::visitLit_int(Lit_int *lit_int)
{
    /* Code For Lit_int Goes Here */

    visitInteger(lit_int->integer_);
}

void RuleCheck::visitT_std_logic(T_std_logic *t_std_logic)
{
    /* Code For T_std_logic Goes Here */
    visitedType = STD_LOGIC;
}

void RuleCheck::visitT_std_logic_vector(T_std_logic_vector *t_std_logic_vector)
{
    /* Code For T_std_logic_vector Goes Here */
    visitedType = STD_LOGIC_VECTOR;
}

void RuleCheck::visitT_integer(T_integer *t_integer)
{
    /* Code For T_integer Goes Here */
    visitedType = INTEGER;
}

void RuleCheck::visitListTopDef(ListTopDef *listtopdef)
{
    for (ListTopDef::iterator i = listtopdef->begin(); i != listtopdef->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void RuleCheck::visitListPorts(ListPorts *listports)
{

    for (ListPorts::iterator i = listports->begin(); i != listports->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void RuleCheck::visitListPre_begin_statements(ListPre_begin_statements *listpre_begin_statements)
{
    for (ListPre_begin_statements::iterator i = listpre_begin_statements->begin(); i != listpre_begin_statements->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void RuleCheck::visitListPost_begin_statements(ListPost_begin_statements *listpost_begin_statements)
{
    for (ListPost_begin_statements::iterator i = listpost_begin_statements->begin(); i != listpost_begin_statements->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void RuleCheck::visitListPmap_stm(ListPmap_stm *listpmap_stm)
{
    for (ListPmap_stm::iterator i = listpmap_stm->begin(); i != listpmap_stm->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void RuleCheck::visitListAss_statements(ListAss_statements *listass_statements)
{
    for (ListAss_statements::iterator i = listass_statements->begin(); i != listass_statements->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void RuleCheck::visitListWhen_part(ListWhen_part *listwhen_part)
{
    for (ListWhen_part::iterator i = listwhen_part->begin(); i != listwhen_part->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void RuleCheck::visitListIdent(ListIdent *listident)
{
    for (ListIdent::iterator i = listident->begin(); i != listident->end(); ++i)
    {
        visitIdent(*i);
    }
}

void RuleCheck::visitListWhens(ListWhens *listwhens)
{
    for (ListWhens::iterator i = listwhens->begin(); i != listwhens->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void RuleCheck::visitListSequential_statement(ListSequential_statement *listsequential_statement)
{
    for (ListSequential_statement::iterator i = listsequential_statement->begin(); i != listsequential_statement->end(); ++i)
    {
        (*i)->accept(this);
    }
}

void RuleCheck::visitInteger(Integer x)
{
    /* Code for Integer Goes Here */
}

void RuleCheck::visitChar(Char x)
{
    /* Code for Char Goes Here */
}

void RuleCheck::visitDouble(Double x)
{
    /* Code for Double Goes Here */
}

void RuleCheck::visitString(String x)
{
    /* Code for String Goes Here */
}

void RuleCheck::visitIdent(Ident x)
{
    /* Code for Ident Goes Here */
    //std::cout << "ident:" << x << std::endl;
}
