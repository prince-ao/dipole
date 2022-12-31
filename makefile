run: dipole
	./build/dipole test/temp.dp

dipole: src/main.cpp src/lexer.cpp src/parser.cpp
	g++ -ggdb src/main.cpp src/lexer.cpp src/parser.cpp -o build/dipole

clean:
	rm build/dipole
