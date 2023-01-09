run: dipole
	./build/dipole test/temp.dp

dipole: src/main.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp
	g++ src/main.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp -o  build/dipole

test: src/main.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp
	g++ -ggdb src/main.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp -o build/test

clean:
	rm build/dipole
