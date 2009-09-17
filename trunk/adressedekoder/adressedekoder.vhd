library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity Adresse is
	Port(	sel : In	std_logic;
	     cs_ram : Out	std_logic;
	     cs_adc : Out	std_logic );

end;

architecture behavioral of Adresse is
begin
	cs_ram <= not sel;
	cs_adc <= sel;

end behavioral;

