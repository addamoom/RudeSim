LIBRARY ieee ;
USE ieee.std_logic_1164.ALL ;

ENTITY testThing IS
    PORT(
        p1: IN STD_LOGIC;
        );
END ENTITY testThing ;

ARCHITECTURE test OF testThing IS
    SIGNAL a : STD_LOGIC ;
    SIGNAL b : STD_LOGIC ;
    SIGNAL c : STD_LOGIC ;
    SIGNAL d : STD_LOGIC_VECTOR(3 DOWNTO 0) ;
BEGIN
    a <= '0', '1' AFTER 2 ps;
    lab: PROCESS(a)
    BEGIN
        b <= '1';
        c <= b;
    END PROCESS;
    d <= "0001", "0010" AFTER 1 ps, "0011" AFTER 2 ps, "0100" AFTER 3 ps, "0101" AFTER 4 ps;
    
END ARCHITECTURE test ;


