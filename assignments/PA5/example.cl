
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

  print_int(x : Int) : Object{
    {
      (attr_io).out_int(x);
      (new IO).out_string("\n");
    }
  };

  basic_class_test() : Object{
    {
      attr_io.out_string("\nBasic Int\n");

      -- Int
      attr_io.out_string("123\n");
      print_int(attr_int1);
      print_int(attr_int2);
      attr_int1 <- 456;
      print_int(attr_int1);
      attr_int2 <- new Int;
      print_int(attr_int2);

      -- arithmetic OP and let
      let int3 : Int, int4 : Int <- 100 in {
        -- 100 + 0 - 456 + 100 = -256
        print_int(int3);
        print_int(int4);
        9999;
        int3 <- int4 + int3;
        
        print_int(int3);

        -- -256 * 2 - 100 = -612
        int3 <- int3 * 2 + ~int4;

        print_int(int3);

        -- -612 / 10 = -61
        int3 <- int3 / 10;

        print_int(int3);

        -- -(-61) = 61
        int3 <- ~int3;
        print_int(int3);
      };

      attr_io.out_string("\nBasic String\n");

      -- String
      (new IO).out_string(attr_string);
      attr_string <- "Pass\n";
      attr_io.out_string(attr_string);

      let temp_string : String <- "Pass\n" in {
        attr_io.out_string(temp_string);
        print_int(temp_string.length());
        attr_string <- temp_string;
        attr_io.out_string(attr_string);
      };

      attr_io.out_string("\nBasic Bool\n");

      -- Bool
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
    }
  };

  logic_test() : Object{
    {
      attr_io.out_string("\nCompare\n");

      -- compare
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
      attr_io.out_string("\nBranch\n\n");

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
      attr_io.out_string("\nClass and dispatch\n\n");

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

  copy() : SELF_TYPE{
    let temp : SELF_TYPE in {
      temp <- new SELF_TYPE;
      temp.set(2, 3);
      temp;
    }
  };
};

class B inherits A{
  b1 : Int;
  b2 : Int;

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
};

class C{

};