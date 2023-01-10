cc :=	"g++"
build := "build/"
src := "src/"
tst := "test/"
gdb := "gdb"

alias t := test
alias r := run
alias d := debug

# run dipole
run: 
	{{cc}} {{src}}*.cpp -o {{build}}dipole
	{{build}}dipole {{tst}}temp.dp

# test dipole
test:
	{{cc}} -ggdb {{src}}*.cpp -o {{build}}test
	-{{build}}test {{tst}}temp.dp

# debug dipole
debug: test
	{{gdb}} {{build}}test

# clean build file
clean:
	rm {{build}}dipole
	rm {{build}}test
