LIBRARY ieee ;
USE ieee.std_logic_1164.ALL ;

ENTITY testThing2 IS
    PORT(
        p1: IN STD_LOGIC;
        );
END ENTITY testThing2 ;

ARCHITECTURE test OF testThing2 IS
    SIGNAL a : STD_LOGIC ;
    SIGNAL b : STD_LOGIC ;
    SIGNAL c : STD_LOGIC ;
BEGIN
    a <= '0', '1' AFTER 2 ps;
    lab: PROCESS(a)
    BEGIN
        b <= '0';
        c <= b;
    END PROCESS;
    
END ARCHITECTURE test ;


