Class A inherits Object {
    a : A;
    b : B <- a;
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

Class C inherits IO {

};

Class Main{
    a : A;
    b : B;
    main() : A{
        b.a()
    };
};