#============================================================================
# Run: 
#    xtclsh test_ise.tcl  - creates XILINX ISE project file
#    ise test_project.ise - opens the project
#============================================================================
source "../../../../../base/xilinxise.tcl"

project_new "test_project"
project_set_props
puts "Adding source files"
xfile add "../../../../../fpga/units/clkgen/clkgen_config.vhd"
xfile add "test_config.vhd"
xfile add "../../../../../fpga/units/clkgen/clkgen.vhd"
xfile add "../../../../../fpga/units/math/math_pack.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc_entity.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc_autoincr.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_reg.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_ctrl.vhd"
xfile add "../../../../../fpga/chips/fpga_xc3s50.vhd"
xfile add "../../../../../fpga/chips/architecture_pc/arch_pc_ifc.vhd"
xfile add "../../../../../fpga/chips/architecture_pc/tlv_pc_ifc.vhd"
xfile add "../../../../../fpga/ctrls/vga/vga_config.vhd"
xfile add "../../../../../fpga/ctrls/vga/vga_ctrl.vhd"
xfile add "../../../../../fpga/ctrls/keyboard/keyboard_ctrl.vhd"
xfile add "../../../../../fpga/ctrls/keyboard/keyboard_ctrl_high.vhd"
xfile add "../../fpga/top.vhd"
xfile add "../../fpga/symbol_rom.vhd"
xfile add "../../fpga/game_pack.vhd"
xfile add "../../fpga/cell.vhd"
xfile add "../../fpga/engen.vhd"
puts "Adding simulation files"
xfile add "../../fpga/sim/tb.vhd" -view Simulation
puts "Libraries"
project_set_isimscript "test_xsim.tcl"
project_set_top "fpga"
project_close
