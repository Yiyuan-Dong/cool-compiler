
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
      attr_string <- "Test Pass\n";
      attr_io.out_string(attr_string);

      let temp_string : String <- "Pass Again\n" in {
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
    }
  };

  main() : Object{
    {
      basic_class_test();
    }
  };
};