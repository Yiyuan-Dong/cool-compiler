
(*
 *  execute "coolc bad.cl" to see the error messages that the coolc parser
 *  generates
 *
 *  execute "myparser bad.cl" to see the error messages that your parser
 *  generates
 *)

(* no error *)
class A {
};

(* error:  b is not a type identifier *)
Class b inherits A {
};

(* error:  a is not a type identifier *)
Class C inherits a {
};

(* error:  keyword inherits is misspelled *)
Class D inherts A {
};

(* error:  closing brace is missing *)
Class E inherits A {
;

(* New error for test *)

Class G {
    main(): Object {  {
        1;
        2;
        let a:Int, b:int, c:Int in {
             1;
	     2 234 sdfsdffadsfasf
	     A;
	};
	1;
    }  };
};

Class F{
    a : Int <- 1;
    test(): Int { {
        1;
        {2; A; B;};     (* Error *)
        2;
	{2 3 4 5}
    } };
};