-- INP 1. projekt, VUT FIT 2018/19
-- Adam Sedláèek, xsedla1e

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port (
    RESET : IN std_logic;
    SMCLK : IN std_logic;
    ROW   : OUT std_logic_vector(0 to 7);
    LED   : OUT std_logic_vector(7 downto 0)
);
end ledc8x8;

architecture main of ledc8x8 is
    signal my_row : std_logic_vector(7 downto 0);
    signal my_col : std_logic_vector(7 downto 0);
    signal cnt : std_logic_vector(11 downto 0) := (others => '0');
    signal state_cnt : std_logic_vector(20 downto 0) := (others => '0');
    signal clock_enable : std_logic := '0';
    signal state : std_logic_vector(1 downto 0) := "00";
        -- 00    ->   A
        -- 01 a 11 -> nic
        -- 10    ->   S

begin
    smclk_div: process(RESET, SMCLK)
    begin
        if (RESET = '1') then
            cnt <= (others => '0');
        elsif rising_edge(SMCLK) then
            cnt <= cnt + 1;
            if (cnt = "111000010000") then
                clock_enable <= '1';
                cnt <= (others => '0');
            else
                clock_enable <= '0';
            end if;
        end if;
    end process;

    row_rotation: process(RESET, SMCLK, clock_enable)
    begin   
        if (RESET = '1') then
            my_row <= "10000000"; 
        elsif (rising_edge(SMCLK) and clock_enable = '1') then
            my_row <= my_row(0) & my_row(7 downto 1);
        end if;
    end process;

    state_changer: process(RESET, SMCLK, clock_enable)
    begin
        if (RESET = '1') then
            state <= "00";
            state_cnt <= (others => '0');
        elsif (rising_edge(SMCLK)) then
            if (state_cnt = "111000010000000000000") then
                state <= state + 1;
                state_cnt <= (others => '0');
            else
                state_cnt <= state_cnt + 1;
            end if;
        end if;
    end process;

    decode: process(my_row)
    begin
        if state = "00" then
            case my_row is 
                when "10000000" => my_col <= "11100111";
                when "01000000" => my_col <= "11000011";
                when "00100000" => my_col <= "10011001";
                when "00010000" => my_col <= "00111100";
                when "00001000" => my_col <= "00000000";
                when "00000100" => my_col <= "00000000";
                when "00000010" => my_col <= "00111100";
                when "00000001" => my_col <= "00111100";
                when others =>     my_col <= "11111111";
            end case;
        elsif state = "01" or state = "11" then
                my_col <= (others => '1');
        elsif state = "10" then
            case my_row is
                when "10000000" => my_col <= "01111111";
                when "01000000" => my_col <= "00000111";
                when "00100000" => my_col <= "00000001";
                when "00010000" => my_col <= "11111001";
                when "00001000" => my_col <= "11000001";
                when "00000100" => my_col <= "10000011";
                when "00000010" => my_col <= "10011111";
                when "00000001" => my_col <= "10000000";
                when others =>     my_col <= "11111111";
            end case;
        end if;
    end process;

    ROW <= my_row;
    LED <= my_col;

end main;