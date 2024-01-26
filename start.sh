if [[ "$OSTYPE" == "msys" ]]; then
	g++ source/main.cpp -std=c++20 -DWINDOWS -o go; go
else
	g++ source/main.cpp -std=c++20 -o go; ./go
fi

