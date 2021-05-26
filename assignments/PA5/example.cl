
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

Class Main inherits Object{
  attr_io : IO <- new IO;
  attr_int1 : Int;
  attr_int2 : Int <- 123;
  attr_string : String;
  attr_bool : Bool <- true;

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
        int3 <- int4 + int3 - attr_int1 + int4;  
        
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

        if count = 10
        then attr_io.out_string("OK\n")
        else attr_io.out_string("??\n")
        fi;

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

        if count = 1
        then attr_io.out_string("OK\n")
        else attr_io.out_string("??\n")
        fi;

        count <- 0;
        attr_int1 <- 100;
        while attr_int1 + count < 100 loop
        {
          attr_int1 <- attr_int1 + 100;
          count <- count + 1;
        }
        pool;

        if count = 0
        then attr_io.out_string("OK\n")
        else attr_io.out_string("??\n")
        fi;
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

  main() : Object{
    {
      basic_class_test();
      logic_test();
    }
  };
};