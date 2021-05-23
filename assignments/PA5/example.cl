
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)
class A {
  a : Int;
  b : Int <- 1234567;
  c : Main;
  d : Main <- new Main;

  c(a : Int, b : Int) : Object{
    1
  };

  d(a1: Int, a2 :Int, a3:Int, a4:Int, a5:Int) : Object{
    {
      a1;
      a2;
      a3;
      a4;
      a5;
      996;
    }
  };

  e() : Object{
    self.d(3,4,5,6,7)
  };

  f() : Object{
    let a : Int, b : Int, c : Int in 
      {
        a;
        b;
        c;
        a + b + c + 1;
        1 < 2;
        isvoid 123456;
        new SELF_TYPE;
      }
  };
};

class B inherits A{

};

class Main {
  main():Int { 1 };
};