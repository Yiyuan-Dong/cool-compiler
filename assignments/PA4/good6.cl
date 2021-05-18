Class A{
    p() : IO{
        (new IO).out_string("123")
    };
    f() : IO{
        self.p()
    };
};

Class B inherits A{
    c : Int;
    p() : IO{
        (new IO).out_string("456")
    };
};

Class Main {
    b : B <- new B;
    main(): Object{
        case b of 
            i : Int => 1;
            c : SELF_TYPE => 2;
        esac
    };
};
