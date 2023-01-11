cc :=	"g++"
build := "build/"
src := "src/"
tst := "test/"
gdb := "gdb"

alias tm := test-main
alias t := test
alias rm := run-main
alias r := run
alias d := debug
alias c := clean

# run dipole on test/FILE
run FILE:
	{{cc}} {{src}}*.cpp -o {{build}}dipole
	{{build}}dipole {{tst}}{{FILE}}

# run dipole
run-main: 
	{{cc}} {{src}}*.cpp -o {{build}}dipole
	{{build}}dipole {{tst}}main.dp

# test dipole on test/FILE
test FILE:
	{{cc}} -ggdb {{src}}*.cpp -o {{build}}test
	-{{build}}test {{tst}}{{FILE}}

# test dipole on test/main.dp
test-main:
	{{cc}} -ggdb {{src}}*.cpp -o {{build}}test
	-{{build}}test {{tst}}main.dp

# debug dipole
debug: test-main
	{{gdb}} {{build}}test

# clean build file
clean:
	rm {{build}}dipole
	rm {{build}}test
