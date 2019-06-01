std: std.cpp
	g++ -g --std=c++11 std.cpp -o std

run:
	make && ./std

clean:
	rm std