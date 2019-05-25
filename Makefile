# Makefile solo para evitar tener que escribir ciertos commands que no se 
# se puedan agregar a CMake

# Corre el entorno de pruebas de Box2D
testbed:
	cd libs/Box2D-master/ && ../premake5 gmake && make -C Build/ && cd Testbed && ../Build/bin/x86_64/Debug/Testbed
	
valgrind-client:
	cd build && valgrind --leak-check=full --show-leak-kinds=all --suppressions=../libs/valgrind-supp/minimal.supp ./client
	
valgrind-server:
	cd build && valgrind --leak-check=full --show-leak-kinds=all --suppressions=../libs/valgrind-supp/minimal.supp ./server

.PHONY: testbed
