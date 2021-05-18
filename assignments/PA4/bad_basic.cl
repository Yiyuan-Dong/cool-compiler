Class A{};

Class B inherits A{};

Class C inherits B{};

Class D inherits B{};

Class E inherits A{};

Class Main{
    a : A;
    b : B;
    c : C;
    d : D;
    e : E;

    main(x : Int, y : Bool) : Int{
        {
            x <- if 1 then 1 else 2 fi;  -- error
            x <- if 1 = 2 + 3 then 1 else 2 fi;  -- OK
            x <- if not ~1 < 2 + 3 then 1 else 2 fi;  -- OK
            y <- if 1 = 2 + 3 then 1 else 2 fi;  -- error

            if ~a then 1 else 2 fi; -- error
            if a + b -c / d then tRue else 2 fi; -- error
            if 1 + a then 1 else 2 fi;  -- error

            a <- if not ~1 < 0 then b else e fi; -- OK
            b <- if not ~1 < 0 then b else e fi; -- error
            b <- if not ~1 < 0 then b else d fi; -- OK
            b <- if not ~1 < 0 then c else d fi; -- OK
            c <- if not ~1 < 0 then c else d fi; -- error

            a <- case 1 of 
                f : A => b;
                g : B => c;
                h : C => d;
            esac;  -- OK

            b <- case 1 of 
                f : A => b;
                g : B => c;
                h : C => d;
            esac;  -- OK

            b <- case 1 of 
                f : A => a;
                g : B => c;
                h : C => d;
            esac;  -- error

            e <- case 1 of 
                f : A => b;
                g : B => c;
                h : C => d;
            esac;  -- error

            x <- new Int;  -- OK
        }
    };
};