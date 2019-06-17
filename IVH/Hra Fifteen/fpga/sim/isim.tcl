# sim.tcl : ISIM simulation script
# Copyright (C) 2011 Brno University of Technology,
#                    Faculty of Information Technology
# Author(s): Zdenek Vasicek
#
# LICENSE TERMS
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. All advertising materials mentioning features or use of this software
#    or firmware must display the following acknowledgement:
#
#      This product includes software developed by the University of
#      Technology, Faculty of Information Technology, Brno and its
#      contributors.
#
# 4. Neither the name of the Company nor the names of its contributors
#    may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# This software or firmware is provided ``as is'', and any express or implied
# warranties, including, but not limited to, the implied warranties of
# merchantability and fitness for a particular purpose are disclaimed.
# In no event shall the company or contributors be liable for any
# direct, indirect, incidental, special, exemplary, or consequential
# damages (including, but not limited to, procurement of substitute
# goods or services; loss of use, data, or profits; or business
# interruption) however caused and on any theory of liability, whether
# in contract, strict liability, or tort (including negligence or
# otherwise) arising in any way out of the use of this software, even
# if advised of the possibility of such damage.
#

#Project setup
#========================================
#set TESTBENCH_ENTITY "testbench"
#set ISIM_PRECISION "1 ps"

#Run simulation
#========================================
proc isim_script {} {

   add_divider "Top level FPGA"
   add_wave_label "-color #FFFF00" "reset" /testbench/uut/p3m(0)
   add_wave_label "-color #ff8000" "smclk" /testbench/uut/smclk
   add_wave_label "" "irq" /testbench/uut/x(0)
   
   add_divider "SPI"
   add_wave_label "" "CLK" /testbench/uut/spi_clk
   add_wave_label "" "CS" /testbench/uut/spi_cs
   add_wave_label "" "DI" /testbench/uut/spi_di
   add_wave_label "" "DO" /testbench/uut/spi_do
   
   if (0) {
   	add_divider "SPI controller"
   	add_wave_label "" "CS" /uut/ispi_cs
   	add_wave_label "" "DI" /uut/ispi_di
   	add_wave_label "" "DI req" /uut/ispi_di_req
   	add_wave_label "" "DO" /uut/ispi_do
   	add_wave_label "" "DO vld" /uut/ispi_do_vld
   }
   
   add_divider "APP"
   add_wave_label "-color #FFFF00" "reset" /testbench/uut/fpga_inst/reset
   add_wave_label "-color #FF8000" "clk" /testbench/uut/fpga_inst/clk
   add_wave_label "-radix hex" "X" /testbench/uut/x
   
   run 10 ms
} 


