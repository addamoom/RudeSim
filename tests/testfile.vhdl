LIBRARY ieee ;
USE ieee.std_logic_1164.ALL ;

ENTITY testThing IS
    PORT(
        a:      IN  STD_LOGIC;
        b:      IN  STD_LOGIC;
        c:      OUT STD_LOGIC;
        d:      OUT STD_LOGIC
        );
END ENTITY testThing ;

ARCHITECTURE test OF testThing IS
    SIGNAL e : STD_LOGIC ;
    SIGNAL f : STD_LOGIC ;
    SIGNAL g : STD_LOGIC := '1';
BEGIN
    f <= '1' ;
    c <= f ;
    d <= NOT e;
    e <= '0', '1' AFTER 3 ps;
    
    
END ARCHITECTURE test ;


