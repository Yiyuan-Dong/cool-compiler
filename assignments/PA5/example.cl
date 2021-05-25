
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
        3 - 4 + ~5 - 6 + 7;
        1 < 2;
        isvoid 123456;
        new SELF_TYPE;
        c <- 123;
        "12345" = "12345";
        "12345" = (new IO).in_string();
        let temp:Bool in {
          temp <- new Bool;
          temp <- not temp;
        };
      }
  };

  g() : Object{ {
    d <- new Main;
    c <- d;
    let a : A in a;
  } };
};

class B inherits A{

};

class Main {
  main():Int { 1 };
};