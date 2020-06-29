#ifndef Types_HEADER
#define Types_HEADER

//Typechecker datasructures
enum Signal_Types
{
    STD_LOGIC,
    STD_LOGIC_VECTOR,
    INTEGER
};
enum Port_direction
{
    IN,
    OUT,
    INOUT
};
enum Expr_state
{
    VALUE,              //When an expression is resolved into a value, the coresponding visitedLit* var must be set with the value.
    SYMBOL,             //Gonna use UNRESOLVED instead of this one to begin with 
    UNRESOLVED
};

struct PortType
{
    std::string identifier;
    Signal_Types type;
    Port_direction direction;

    PortType(std::string i, Signal_Types t, Port_direction d)
    {
        identifier = i;
        type = t;
        direction = d;
    }
};
struct SignalType
{
    std::string identifier;
    std::string type;

    SignalType(std::string i, std::string t)
    {
        identifier = i;
        type = t;
    }
};

struct ArchType
{
    std::string label;
    std::string entity;

    ArchType(std::string l, std::string e)
    {
        label = l;
        entity = e;
    }
};
struct EntityType
{
    std::string label;
    std::vector<PortType> ports;

    EntityType(std::string l, std::vector<PortType> e)
    {
        label = l;
        ports = e;
    }
    EntityType(){}
};

//Simulator datastructure
struct std_logic_state
{
    std::string identifier;
    char value;

    std_logic_state(std::string i, char v)
    {
        identifier = i;
        value = v;
    }
};
struct std_logic_vector_state
{
    std::string identifier;
    std::string value;

    std_logic_vector_state(std::string i, std::string v)
    {
        identifier = i;
        value = v;
    }
};
struct integer_state
{
    std::string identifier;
    int value;

    integer_state(std::string i, int v)
    {
        identifier = i;
        value = v;
    }
};
struct simulation_state
{
    std::vector<std_logic_state> std_logics;
    std::vector<integer_state> integers;
    std::vector<std_logic_vector_state> std_logic_vectors;

    simulation_state(std::vector<std_logic_state> sl, std::vector<integer_state> i, std::vector<std_logic_vector_state> slv)
    {
        std_logics = sl;
        integers = i;
        std_logic_vectors = slv;
    };
    simulation_state(){
        
    }
};


#endif