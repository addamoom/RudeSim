#ifndef Simulator_HEADER
#define Simulator_HEADER
/* You might want to change the above name. */

#include "../Absyn.H"
#include "Types.H"
#include <map>

class Simulator : public Visitor
{
public:
    void visitProg(Prog *p);
    void visitTopDef(TopDef *p);
    void visitPorts(Ports *p);
    void visitPre_begin_statements(Pre_begin_statements *p);
    void visitPost_begin_statements(Post_begin_statements *p);
    void visitPmap_stm(Pmap_stm *p);
    void visitConcurrent_statement(Concurrent_statement *p);
    void visitAss_statements(Ass_statements *p);
    void visitWhen_part(When_part *p);
    void visitSequential_statement(Sequential_statement *p);
    void visitWhens(Whens *p);
    void visitExp(Exp *p);
    void visitCmpop(Cmpop *p);
    void visitLiteral(Literal *p);
    void visitType(Type *p);
    void visitProgram(Program *p);
    void visitLibrary_Dec(Library_Dec *p);
    void visitLibrary_Use(Library_Use *p);
    void visitEntity(Entity *p);
    void visitInport(Inport *p);
    void visitOutport(Outport *p);
    void visitInoutport(Inoutport *p);
    void visitArch(Arch *p);
    void visitSignal_Decl(Signal_Decl *p);
    void visitSignal_Decl_W_Assign(Signal_Decl_W_Assign *p);
    void visitConstant_Decl(Constant_Decl *p);
    void visitConstant_Decl_W_Assign(Constant_Decl_W_Assign *p);
    void visitComponent(Component *p);
    void visitConcurrent_Stm(Concurrent_Stm *p);
    void visitPort_Map(Port_Map *p);
    void visitPort_Map_Internal_Statement(Port_Map_Internal_Statement *p);
    void visitConcurrent_Assignment(Concurrent_Assignment *p);
    void visitConcurrent_Assignment_W_AFTER(Concurrent_Assignment_W_AFTER *p);
    void visitAfter_Component(After_Component *p);
    void visitWhen_Statement(When_Statement *p);
    void visitWhen_Component(When_Component *p);
    void visitWhen_Finisher(When_Finisher *p);
    void visitProcess_Statement(Process_Statement *p);
    void visitCase_Statement(Case_Statement *p);
    void visitCase_Case(Case_Case *p);
    void visitIf_Statement(If_Statement *p);
    void visitIf_Else_Statement(If_Else_Statement *p);
    void visitWait_For_Statement(Wait_For_Statement *p);
    void visitSeq_Assignment(Seq_Assignment *p);
    void visitE_Identifier(E_Identifier *p);
    void visitE_String(E_String *p);
    void visitE_Integer(E_Integer *p);
    void visitE_Std_Logic(E_Std_Logic *p);
    void visitE_Vector_Ref(E_Vector_Ref *p);
    void visitE_Add(E_Add *p);
    void visitE_Sub(E_Sub *p);
    void visitE_Not(E_Not *p);
    void visitE_Cmp(E_Cmp *p);
    void visitE_Equal(E_Equal *p);
    void visitE_Less(E_Less *p);
    void visitE_More(E_More *p);
    void visitE_AND(E_AND *p);
    void visitE_OR(E_OR *p);
    void visitE_XOR(E_XOR *p);
    void visitE_NAND(E_NAND *p);
    void visitE_NOR(E_NOR *p);
    void visitE_XNOR(E_XNOR *p);
    void visitLit_string(Lit_string *p);
    void visitLit_int(Lit_int *p);
    void visitLit_char(Lit_char *lit_char);
    void visitT_std_logic(T_std_logic *p);
    void visitT_std_logic_vector(T_std_logic_vector *p);
    void visitT_integer(T_integer *p);
    void visitListTopDef(ListTopDef *p);
    void visitListPorts(ListPorts *p);
    void visitListPre_begin_statements(ListPre_begin_statements *p);
    void visitListPost_begin_statements(ListPost_begin_statements *p);
    void visitListPmap_stm(ListPmap_stm *p);
    void visitListAss_statements(ListAss_statements *p);
    void visitListWhen_part(ListWhen_part *p);
    void visitListIdent(ListIdent *p);
    void visitListWhens(ListWhens *p);
    void visitListSequential_statement(ListSequential_statement *p);

    void visitInteger(Integer x);
    void visitChar(Char x);
    void visitDouble(Double x);
    void visitString(String x);
    void visitIdent(Ident x);

    int startSimulation(Visitable *t);
    long int convertToPs(int i, std::string u);
    long int simulation_time = 5; //time the simulation should run, in ps.
    int simulation_steps = 50000; //no of discrete time steps in simulation.
    bool printDebugInfo;          //Determines wheter debug info should be printed or not

private:
    int returnvalue = 0; // set this to one if errors are detected.

    std::vector<EntityType> entities; //List of entities in the file.
    void printState(simulation_state s);
    bool check_if_all_signals_are_updated();
    void assignSignalfromLit(std::string i);
    std::string invertString(std::string s);
    void print(PrintType p, std::string s);
};

#endif
