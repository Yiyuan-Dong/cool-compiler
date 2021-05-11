class Main {
    a : Int;
    main() :Object {
        {
	    (new IO).out_string("\n");
            let b : Int <- 0 in if 3 + 4 - b  + 5 = 5 + 4 + 3 
		then (new IO).out_string("1") 
		else (new IO).out_string("-1") fi ; 
            let b : Int, c: Int <- 2 in if b + c * 3 < 12 / 2 + 1 
		then (new IO).out_string("2") 
		else (new IO).out_string("-2") fi ; 
	    (new IO).out_string("\n");
        }
    };
};