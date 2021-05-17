Class A inherits Object {
    a : A;
    b : B <- b;
    a () : A{
        a
    };
};

Class B inherits A{
    b () : A{
        a
    };
};

Class C inherits B {
    c(i : Int) : A{
        b
    };
};

Class Main{
    a : A;
    b : B;
    main() : A{
        b.a()
    };
};