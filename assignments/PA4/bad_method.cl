Class A{
    a : A;
    a1() : Object{
        a
    };
    a2(x : Int) : A{
        a
    };
};

Class B inherits A{
    -- OK
    a1() : Object{
        (new IO).out_int(1)
    };

    -- Ok
    a3() : A{
        self.a2(1)
    };

    -- error
    a4() : B{
        self@A.a2(2)
    };

    -- error (Cascading error)
    a5() : A{
        self@A.a2()
    };
};

Class Main inherits A{
    -- error 
    a1() : A{
        a
    };

    -- error 
    a2(x : Bool) : A{
        a
    };

    -- error 
    a2(x : Bool) : A {
        5
    };

    main() : Object{
        {
            a1(1); -- error 
            a2(a); -- error 
            self.a1(); -- OK
            let b : B <- new B in {
                b.a1(); -- OK
                b.a2(1); -- OK
                b.a2("1"); -- error 
                b@A.a2(3 + 4 * 5 - 6 / 7); -- OK
            };
        }
    };
    
};