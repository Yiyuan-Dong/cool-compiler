Class Main inherits Object{
  attr_io : IO <- new IO;
  error_io : IO;
  main() : Object{
    {
      attr_io.out_string("This is ok, but next...\n");
      error_io.out_string("?");
      attr_io.out_string("123\n");
    }
  };
};