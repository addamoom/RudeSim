LIBRARY ieee ;
USE ieee.std_logic_1164.ALL ;

ENTITY testThing IS
    PORT(
        a:      IN  STD_LOGIC;
        b:      IN  STD_LOGIC;
        c:      OUT STD_LOGIC;
        d:      OUT STD_LOGIC;
        i:      OUT STD_LOGIC_VECTOR(5 DOWNTO 0)
        );
END ENTITY testThing ;

ARCHITECTURE test OF testThing IS
    SIGNAL e : STD_LOGIC ;
    SIGNAL f : STD_LOGIC ;
    SIGNAL g : STD_LOGIC := '1';
    --SIGNAL h : STD_LOGIC_VECTOR(5 DOWNTO 0) := "000000";
    --SIGNAL l : STD_LOGIC_VECTOR(5 DOWNTO 0);
    --SIGNAL m : STD_LOGIC_VECTOR(5 DOWNTO 0);
    SIGNAL j : INTEGER := 4;
    SIGNAL k : INTEGER;

BEGIN
    f <= '1', '0' AFTER 2 ps;
    c <= '1' ;
    d <= f XOR c;
    --e <= f OR e;
    h <= "111100", "100111" AFTER 2 ps;
    --l <=  "111111";
    --m <=h XOR l;
    --k <= 10, 344 AFTER 2 ps, 26 AFTER 4 ps;
    
END ARCHITECTURE test ;


