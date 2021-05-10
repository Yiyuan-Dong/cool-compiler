(*
 *  CS164 Fall 94
 *
 *  Programming Assignment 1
 *    Implementation of a simple stack machine.
 *
 *  Skeleton file
 * (* ** *) *)

class List{

	val : String;
	next : List;
	
	isNil() : Bool 
		{ {abort(); true; } };
	
	init(in_val : String, in_next : List) : List {
		{
			val <- in_val;
			next <- in_next;
		}
	};
	
	get_val() : String{
		val
	};
	
	get_next() : List{
		next
	};
	
	set_value(input : String) : String{
		{
			val <-  input;
			input;
		}
	};
	
};


class Nil inherits List{
	
	isNil() : Bool { true };
	
};


class Node inherits List{
	
	isNil() : Bool { false };
	
};
	
	
class Stack{

	top : List;
	
	set_top(input_top : List) : Stack {
		{
			top <- input_top;
			self;
		}
	};
	
	add(val : String) : Stack {
		{
			(let tmp : List <- (new Node) in {
				tmp.init(val ,top);
				top <- tmp;
			});
			self;
		}
	};
	
	display() : Object {
		{
			(let tmp : List <- top in 
				while (not tmp.isNil()) loop{
					(new IO).out_string(tmp.get_val().concat("\n"));
					tmp <- tmp.get_next();
				}
				pool
			);
			top;
		}
	};
	
	eval() : Object {
		if (top.isNil()) then 1 else
			if (top.get_val() = "+") then 
				let a : Int <- 0,
					b : Int <- 0,
					s : Int <- 0
				in {
					
					a <- (new A2I).a2i(top.get_next().get_val());
					b <- (new A2I).a2i(top.get_next().get_next().get_val());
					s <- a + b;
					top <- top.get_next().get_next().get_next();
					add((new A2I).i2a(s));
					1;
					
				}
			else
				if (top.get_val() = "s") then 
					let a : String,
						b : String
					in {
						a <- top.get_next().get_val();
						b <- top.get_next().get_next().get_val();
						(top.get_next()).set_value(b);
						(top.get_next().get_next()).set_value(a);
						top <- top.get_next();
						1;
					}
				else 1 
				fi 
			fi 
		fi
		
	};
	
};


class Main inherits IO {

	main() : Object {
		let flag : Bool <- false,
		    stack : Stack <- new Stack,
			nil_node : List <- new Nil 
		in {
			stack.set_top(nil_node);
			while (not flag) loop 
				let input : String in {
					(new IO).out_string(">");
					input <- (new IO).in_string();
					(if input = "x" then
						flag <- true
					else
						if input = "d" then 
							stack.display()
						else
							if input = "e" then 
								stack.eval()
							else 
								stack.add(input)
							fi
						fi
					fi);
				}
			pool;
			1;
		}
	};

};
