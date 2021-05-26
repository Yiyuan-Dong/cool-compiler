
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

Class Main inherits Object{
  attr_io : IO <- new IO;
  attr_int1 : Int;
  attr_int2 : Int <- 123;

  print_int(x : Int) : Object{
    {
      (attr_io).out_int(x);
      (new IO).out_string("\n");
    }
  };

  basic_class_test() : Object{
    {
      -- Int
      attr_io.out_string("123\n");
      print_int(attr_int1);
      print_int(attr_int2);
      attr_int1 <- 456;
      print_int(attr_int1);
      attr_int2 <- new Int;
      print_int(attr_int2);

      -- arithmetic OP
      print_int(996);
      let int3 : Int, int4 : Int <- 100 in {
        int3 <- int4 + int3 ;
        print_int(996);
      };
    }
  };

  main() : Object{
    {
      basic_class_test();
    }
  };
};