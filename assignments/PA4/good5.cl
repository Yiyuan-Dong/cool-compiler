Class A inherits Object {
    a : A;
    b : B <- b;
    a () : A{
        self
    };
};

Class B inherits A{
    b () : A{
        a
    };
    a () : A{
        a
    };
};

Class C inherits B {
    a(i : Int) : A{
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