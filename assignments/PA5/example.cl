
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

Class Main inherits Object{
  attr_io : IO <- new IO;
  attr_int1 : Int;
  attr_int2 : Int <- 123;
  attr_string : String;
  attr_bool : Bool <- true;
  a : A <- new A;
  b : B <- new B;

  equal_test(x : Object, y : Object) : Object{
    {
      if x = y
      then attr_io.out_string("OK\n")
      else attr_io.out_string("??\n")
      fi;
    }
  };

  not_equal_test(x : Object, y : Object) : Object{
    {
      if not x = y
      then attr_io.out_string("OK\n")
      else attr_io.out_string("??\n")
      fi;
    }
  };

  print_int(x : Int) : Object{
    {
      (attr_io).out_int(x);
      (new IO).out_string("\n");
    }
  };

  basic_class_test() : Object{
    {
      -- String
      attr_io.out_string("\nBasic String\n");

      (new IO).out_string(attr_string);
      attr_string <- "OK\n";
      attr_io.out_string(attr_string);

      let temp_string : String <- "OKOK\n" in {
        attr_io.out_string(temp_string);
        equal_test(temp_string.length(), 5);
        attr_string <- temp_string;
        attr_io.out_string(attr_string);
        equal_test(attr_string, "OKOK\n");
      };

      -- Bool and cond
      attr_io.out_string("\nBasic Bool\n");

      if attr_bool
      then attr_io.out_string("OK\n")
      else attr_io.out_string("??\n")
      fi;

      attr_bool <- false;
      if attr_bool
      then attr_io.out_string("??\n")
      else attr_io.out_string("OK\n")
      fi;

      attr_bool <- not attr_bool;
      if attr_bool
      then attr_io.out_string("OK\n")
      else attr_io.out_string("??\n")
      fi;

      if not not not not attr_bool
      then attr_io.out_string("OK\n")
      else attr_io.out_string("??\n")
      fi; 

      -- Int
      attr_io.out_string("\nBasic Int\n");
      
      equal_test(attr_int1, 0);
      equal_test(attr_int2, 123);
      attr_int1 <- 456;
      equal_test(attr_int1, 456);
      attr_int2 <- new Int;
      equal_test(attr_int2, 0);

      -- arithmetic OP and let
      let int3 : Int, int4 : Int <- 100 in {
        equal_test(int3, 0);
        equal_test(int4, 100);

        -- 100 + 0 - 456 + 100 = -256
        int3 <- int4 + int3 - attr_int1 + int4;
        
        equal_test(int3, ~256);

        -- -256 * 2 - 100 = -612
        int3 <- int3 * 2 + ~int4;

        equal_test(int3, ~612);

        -- -612 / 10 = -61
        int3 <- int3 / 10;

        equal_test(int3, ~61);

        -- -(-61) = 61
        int3 <- ~int3;

        equal_test(int3, 61);
      };
    }
  };

  logic_test() : Object{
    {
      -- compare
      attr_io.out_string("\nCompare\n");

      attr_int1 <- 110;
      if attr_int1 < 100
      then attr_io.out_string("??\n")
      else attr_io.out_string("OK\n")
      fi;

      attr_int1 <- 99;
      if attr_int1 < 100
      then attr_io.out_string("OK\n")
      else attr_io.out_string("??\n")
      fi;

      attr_int1 <- 110;
      if attr_int1 = 100
      then attr_io.out_string("??\n")
      else attr_io.out_string("OK\n")
      fi;

      attr_int1 <- 100;
      if attr_int1 = 100
      then attr_io.out_string("OK\n")
      else attr_io.out_string("??\n")
      fi;

      attr_int1 <- 100;
      if attr_int1 <= 100
      then attr_io.out_string("OK\n")
      else attr_io.out_string("??\n")
      fi;

      attr_int1 <- 996;
      if attr_int1 <= 100
      then attr_io.out_string("??\n")
      else attr_io.out_string("OK\n")
      fi;

      attr_int1 <- 98;
      if attr_int1 <= 100
      then attr_io.out_string("OK\n")
      else attr_io.out_string("??\n")
      fi;

      attr_int1 <- 100;
      if attr_int1 + attr_int1 + 100 + attr_int1 = 400
      then attr_io.out_string("OK\n")
      else attr_io.out_string("??\n")
      fi;

      if attr_int1 = 100 + ~100 * 3 + 300
      then {attr_io.out_string("OK\n"); attr_io.out_string("OK\n"); attr_io.out_string("OK\n"); }
      else attr_io.out_string("??\n")
      fi;

      equal_test({1; 2; 3; 5; 8;}, 8);

      -- Loop
      attr_io.out_string("\nLoop\n");

      attr_int1 <- 100;
      let count : Int in {
        while attr_int1 <= 1000 loop
        {
          attr_io.out_int(count);
          attr_io.out_string(" ");
          attr_int1 <- attr_int1 + 100;
          count <- count + 1;
        }
        pool;

        equal_test(count, 10);

        count <- 0;
        attr_int1 <- 100;
        while attr_int1 = 100 loop
        {
          attr_io.out_int(count);
          attr_io.out_string(" ");
          attr_int1 <- attr_int1 + 100;
          count <- count + 1;
        }
        pool;

        equal_test(count, 1);

        count <- 0;
        attr_int1 <- 100;
        while attr_int1 + count < 100 loop
        {
          attr_int1 <- attr_int1 + 100;
          count <- count + 1;
        }
        pool;

        equal_test(count, 0);
      };

      -- Branch
      attr_io.out_string("\nBranch\n");

      case new Int of
        s : String => attr_io.out_string(s);
        i : Int => attr_io.out_string("OK\n");
        b : Bool => 1;
      esac;
      
      case "glgjssy" of
        s : String => attr_io.out_string("OK\n");
        b : Bool => 1;
        i : Int => attr_io.out_string("??\n");
      esac;
    }
  };

  class_and_dispatch_test() : Object{
    {
      attr_io.out_string("\nClass and dispatch\n");

      a.set(1, 2);
      equal_test(a.geta1(), 1);
      equal_test(b.geta1(), 0);

      b.set(4,5);
      b.setb(7,8);
      equal_test(b.geta1(), 4);
      equal_test(b.getb2(), 8);

      equal_test(a.play(), 1);
      equal_test(b.play(), 2);

      a <- b;
      equal_test(a.geta1(), 4);
      a.set(100, 200);
      equal_test(a.geta1(), 100);
      equal_test(b.geta1(), 100);
      not_equal_test(new A, new A);

      equal_test(b@A.play(), 1);
      equal_test(a@A.play(), 1);

      b.set(1, 2);
      a <- b.self_copy();
      equal_test(a.geta2(), 2);
      b.set(3, 4);
      not_equal_test(a.geta2(), 4);

      let temp_a : A in {
        temp_a <- a.self_copy();
        equal_test(temp_a.play(), 2);
      };

      let a : A in {
        equal_test(isvoid a, true);
        a <- new A;
        equal_test(isvoid a, false);
      };

      let d : D <- new D in {
        equal_test(d.getb2(), 0);
        equal_test(d.play(), 3);
        equal_test(d@C.play(), 2);
        equal_test(d@B.play(), 2);
        equal_test(d@A.play(), 1);
      };

      equal_test(
        case new C of 
          i : Int => 1;
          s : String => 2;
          d : D => 3;
          c : C => 4;
          b : B => 5;
          a : A => 6;
          o : Object => 100;
        esac
      , 4);

      equal_test(
        case new B of 
          i : Int => 1;
          s : String => 2;
          d : D => 3;
          c : C => 4;
          o : Object => 100;
        esac
      , 100);
    }
  };

  main() : Object{
    {
      basic_class_test();
      logic_test();
      class_and_dispatch_test();
    }
  };
};

class A {
  a1 : Int;
  a2 : Int;
  a_bool : Bool;

  set(x : Int, y : Int) : Object{
    {
      a1 <- x;
      a2 <- y;
    }
  };

  get() : SELF_TYPE{
    self
  };

  geta1() : Int{
    a1
  };

  geta2() : Int{
    a2
  };

  play() : Int{
    1 
  };

  self_copy() : SELF_TYPE{
    let temp : SELF_TYPE in {
      temp <- new SELF_TYPE;
      temp.set(a1, a2);
      temp;
    }
  };
};

class B inherits A{
  b1 : Int;
  b2 : Int;
  str : String;
  b_bool : Bool;

  play() : Int{
    2
  };

  setb(z : Int, w : Int) : Object{
    {
      b1 <- z;
      b2 <- w;
    }
  };

  getb1() : Int {
    b1
  };

  getb2() : Int {
    b2
  };

  self_copy() : SELF_TYPE{
    let temp : SELF_TYPE in {
      temp <- self@A.self_copy();
      temp.setb(b1, b2);
      temp;
    }
  };
};

class C inherits B{

};

class D inherits C {
  play() : Int{
    3
  };
};