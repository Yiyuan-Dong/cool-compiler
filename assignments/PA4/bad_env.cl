Class A{

};

Class Main{
    a : A <- new A;
    b : A <- a;
    c : Int <- d;
    d : Int <- e;  -- error
    f : Int <- self; -- error

    main() : Object{
        self
    };

    -- error
    a(a : Int) : Int{
        1
    };

    b(x : Int, y : Int) : Int{
        {
            x + y;
            let z : Int <- z in 1;  -- error
            z; -- error
            let z : SELF_TYPE <- self, z : Int <- 1, z : Bool <- tRue in self.a(1);  -- OK
        }
    };

    -- error
    b2() : Int{
        y
    };

    -- error SELF_TYPE can not use as args
    c(x : SELF_TYPE) : Int {
        1
    };

    -- OK
    d() : SELF_TYPE {
        self
    };

    e(x : Int, x : Bool, a : Int) : Int { -- error for x, not for a
        case self of
            a : Int => 1; -- ok
            a2 : Int => a2 + 1; -- error two Ints
            b2 : Strint => a2; -- error
            c2 : IO => a; -- ok
        esac
    };
};