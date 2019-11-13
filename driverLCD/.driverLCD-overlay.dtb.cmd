cmd_/home/stud/Projekt3/PRJ3/driverLCD/driverLCD-overlay.dtb := mkdir -p /home/stud/Projekt3/PRJ3/driverLCD/ ; arm-poky-linux-gnueabi-gcc -E -Wp,-MD,/home/stud/Projekt3/PRJ3/driverLCD/.driverLCD-overlay.dtb.d.pre.tmp -nostdinc -I./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o /home/stud/Projekt3/PRJ3/driverLCD/.driverLCD-overlay.dtb.dts.tmp /home/stud/Projekt3/PRJ3/driverLCD/driverLCD-overlay.dts ; ./scripts/dtc/dtc -O dtb -o /home/stud/Projekt3/PRJ3/driverLCD/driverLCD-overlay.dtb -b 0 -i/home/stud/Projekt3/PRJ3/driverLCD/ -i./scripts/dtc/include-prefixes -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-gpios_property -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-graph_port -Wno-unique_unit_address -Wno-pci_device_reg  -d /home/stud/Projekt3/PRJ3/driverLCD/.driverLCD-overlay.dtb.d.dtc.tmp /home/stud/Projekt3/PRJ3/driverLCD/.driverLCD-overlay.dtb.dts.tmp ; cat /home/stud/Projekt3/PRJ3/driverLCD/.driverLCD-overlay.dtb.d.pre.tmp /home/stud/Projekt3/PRJ3/driverLCD/.driverLCD-overlay.dtb.d.dtc.tmp > /home/stud/Projekt3/PRJ3/driverLCD/.driverLCD-overlay.dtb.d

source_/home/stud/Projekt3/PRJ3/driverLCD/driverLCD-overlay.dtb := /home/stud/Projekt3/PRJ3/driverLCD/driverLCD-overlay.dts

deps_/home/stud/Projekt3/PRJ3/driverLCD/driverLCD-overlay.dtb := \

/home/stud/Projekt3/PRJ3/driverLCD/driverLCD-overlay.dtb: $(deps_/home/stud/Projekt3/PRJ3/driverLCD/driverLCD-overlay.dtb)

$(deps_/home/stud/Projekt3/PRJ3/driverLCD/driverLCD-overlay.dtb):
