Class A inherits Object {
    a : A;
    b : B <- b;
    a (b : Int) : A{
        let b : String in a
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
    main() : Int{
        case 1 of 
            a : Int => 1;
            a : String => 2;
        esac
    };
};