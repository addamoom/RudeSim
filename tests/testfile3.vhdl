LIBRARY ieee ;
USE ieee.std_logic_1164.ALL ;

ENTITY testThing3 IS
    PORT(
        p1: IN STD_LOGIC;
        );
END ENTITY testThing3 ;

ARCHITECTURE test OF testThing3 IS
    SIGNAL a : STD_LOGIC ;
    SIGNAL b : STD_LOGIC ;
    SIGNAL c : STD_LOGIC ;
BEGIN
    a <= '0', '1' AFTER 2 ps;
    lab: PROCESS(a)
    BEGIN
        b <= '1';
        c <= a;
    END PROCESS;
    
END ARCHITECTURE test ;


