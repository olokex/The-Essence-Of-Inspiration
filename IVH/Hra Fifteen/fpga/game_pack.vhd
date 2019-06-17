library IEEE;
use IEEE.STD_LOGIC_1164.all;

package game_pack is

type mask_t is record
		top    : std_logic;
		left   : std_logic;
		right  : std_logic;
		bottom : std_logic;
end record;

function getmask(x, y : natural; COLUMNS, ROWS : natural) return mask_t;
function vypocet(x, y : integer) return integer;

end game_pack;

package body game_pack is

function vypocet(x, y : integer) return integer is
begin
    return (63 - 4 * (x mod 4) - 16 * (y mod 4) - 3);
end vypocet;

function getmask(x,y : natural; COLUMNS, ROWS : natural) return mask_t is

variable mask : mask_t;
	
begin
	mask := ('1', '1', '1', '1');
	
	if X = 0 then
		mask.left := '0';
	end if;
	
	if Y = ROWS - 1 then
		mask.bottom := '0';
	end if;
	
	if Y = 0 then
		mask.top := '0';
	end if;
	
	if X = COLUMNS - 1 then
		mask.right := '0';
	end if;
	
	return mask;
end getmask;
end game_pack;
