std: std_1.cpp
	g++ --std=c++11 std_1.cpp -o std

run:
	make && ./std

clean:
	rm std
