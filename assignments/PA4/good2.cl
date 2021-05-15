Class A inherits B{

};

Class B inherits C{

};

Class C inherits E{
};

Class D inherits E{

};

Class E inherits Int { 
};

class Main {
    a : Int <- c;
    b : Int <- 1;
    c : Int <- b;
    main() : Object{ {
        (new IO).out_int(a);
        (new IO).out_string("\n");
        (new IO).out_int(b);
        (new IO).out_string("\n");
        (new IO).out_int(c);
        (new IO).out_string("\n");
    } };
};