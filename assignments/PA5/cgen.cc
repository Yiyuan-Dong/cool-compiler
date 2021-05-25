
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;

// Newly added
SymbolTable<Symbol, ObjEntry> *obj_table = NULL;
CgenClassTable *table_ptr = NULL;
int obj_index = 0;
int label_index = 0;
Symbol curr_filename = NULL;

int max(int x, int y){
  return x > y ? x : y;
}

//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
       arg,
       arg2,
       Bool,
       concat,
       cool_abort,
       copy,
       Int,
       in_int,
       in_string,
       IO,
       length,
       Main,
       main_meth,
       No_class,
       No_type,
       Object,
       out_int,
       out_string,
       prim_slot,
       self,
       SELF_TYPE,
       Str,
       str_field,
       substr,
       type_name,
       val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
  arg         = idtable.add_string("arg");
  arg2        = idtable.add_string("arg2");
  Bool        = idtable.add_string("Bool");
  concat      = idtable.add_string("concat");
  cool_abort  = idtable.add_string("abort");
  copy        = idtable.add_string("copy");
  Int         = idtable.add_string("Int");
  in_int      = idtable.add_string("in_int");
  in_string   = idtable.add_string("in_string");
  IO          = idtable.add_string("IO");
  length      = idtable.add_string("length");
  Main        = idtable.add_string("Main");
  main_meth   = idtable.add_string("main");
//   _no_class is a symbol that can't be the name of any 
//   user-defined class.
  No_class    = idtable.add_string("_no_class");
  No_type     = idtable.add_string("_no_type");
  Object      = idtable.add_string("Object");
  out_int     = idtable.add_string("out_int");
  out_string  = idtable.add_string("out_string");
  prim_slot   = idtable.add_string("_prim_slot");
  self        = idtable.add_string("self");
  SELF_TYPE   = idtable.add_string("SELF_TYPE");
  Str         = idtable.add_string("String");
  str_field   = idtable.add_string("_str_field");
  substr      = idtable.add_string("substr");
  type_name   = idtable.add_string("type_name");
  val         = idtable.add_string("_val");
}

static char *gc_init_names[] =
  { "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
static char *gc_collect_names[] =
  { "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };


//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************

void program_class::cgen(ostream &os) 
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  CgenClassTable *codegen_classtable = new CgenClassTable(classes,os);

  os << "\n# end of generated code\n";
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_load(char *dest_reg, int offset, char *source_reg, ostream& s)
{
  s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")" 
    << endl;
}

static void emit_store(char *source_reg, int offset, char *dest_reg, ostream& s)
{
  s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
      << endl;
}

static void emit_load_imm(char *dest_reg, int val, ostream& s)
{ s << LI << dest_reg << " " << val << endl; }

static void emit_load_address(char *dest_reg, char *address, ostream& s)
{ s << LA << dest_reg << " " << address << endl; }

static void emit_partial_load_address(char *dest_reg, ostream& s)
{ s << LA << dest_reg << " "; }

static void emit_load_bool(char *dest, const BoolConst& b, ostream& s)
{
  emit_partial_load_address(dest,s);
  b.code_ref(s);
  s << endl;
}

static void emit_load_string(char *dest, StringEntry *str, ostream& s)
{
  emit_partial_load_address(dest,s);
  str->code_ref(s);
  s << endl;
}

static void emit_load_int(char *dest, IntEntry *i, ostream& s)
{
  emit_partial_load_address(dest,s);
  i->code_ref(s);
  s << endl;
}

static void emit_move(char *dest_reg, char *source_reg, ostream& s)
{ s << MOVE << dest_reg << " " << source_reg << endl; }

static void emit_neg(char *dest, char *src1, ostream& s)
{ s << NEG << dest << " " << src1 << endl; }

static void emit_add(char *dest, char *src1, char *src2, ostream& s)
{ s << ADD << dest << " " << src1 << " " << src2 << endl; }

static void emit_addu(char *dest, char *src1, char *src2, ostream& s)
{ s << ADDU << dest << " " << src1 << " " << src2 << endl; }

static void emit_addiu(char *dest, char *src1, int imm, ostream& s)
{ s << ADDIU << dest << " " << src1 << " " << imm << endl; }

static void emit_div(char *dest, char *src1, char *src2, ostream& s)
{ s << DIV << dest << " " << src1 << " " << src2 << endl; }

static void emit_mul(char *dest, char *src1, char *src2, ostream& s)
{ s << MUL << dest << " " << src1 << " " << src2 << endl; }

static void emit_sub(char *dest, char *src1, char *src2, ostream& s)
{ s << SUB << dest << " " << src1 << " " << src2 << endl; }

static void emit_sll(char *dest, char *src1, int num, ostream& s)
{ s << SLL << dest << " " << src1 << " " << num << endl; }

static void emit_jalr(char *dest, ostream& s)
{ s << JALR << "\t" << dest << endl; }

static void emit_jal(char *address,ostream &s)
{ s << JAL << address << endl; }

static void emit_return(ostream& s)
{ s << RET << endl; }

static void emit_gc_assign(ostream& s)
{ s << JAL << "_GenGC_Assign" << endl; }

static void emit_disptable_ref(Symbol sym, ostream& s)
{  s << sym << DISPTAB_SUFFIX; }

static void emit_init_ref(Symbol sym, ostream& s)
{ s << sym << CLASSINIT_SUFFIX; }

static void emit_label_ref(int l, ostream &s)
{ s << "label" << l; }

static void emit_protobj_ref(Symbol sym, ostream& s)
{ s << sym << PROTOBJ_SUFFIX; }

static void emit_method_ref(Symbol classname, Symbol methodname, ostream& s)
{ s << classname << METHOD_SEP << methodname; }

static void emit_label_def(int l, ostream &s)
{
  emit_label_ref(l,s);
  s << ":" << endl;
}

static void emit_beqz(char *source, int label, ostream &s)
{
  s << BEQZ << source << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_beq(char *src1, char *src2, int label, ostream &s)
{
  s << BEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bne(char *src1, char *src2, int label, ostream &s)
{
  s << BNE << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bleq(char *src1, char *src2, int label, ostream &s)
{
  s << BLEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blt(char *src1, char *src2, int label, ostream &s)
{
  s << BLT << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blti(char *src1, int imm, int label, ostream &s)
{
  s << BLT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bgti(char *src1, int imm, int label, ostream &s)
{
  s << BGT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_branch(int l, ostream& s)
{
  s << BRANCH;
  emit_label_ref(l,s);
  s << endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
static void emit_push(char *reg, ostream& str)
{
  emit_store(reg,0,SP,str);
  emit_addiu(SP,SP,-4,str);
}

//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source into the register dest
//
static void emit_fetch_int(char *dest, char *source, ostream& s)
{ emit_load(dest, DEFAULT_OBJFIELDS, source, s); }

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
static void emit_store_int(char *source, char *dest, ostream& s)
{ emit_store(source, DEFAULT_OBJFIELDS, dest, s); }


static void emit_test_collector(ostream &s)
{
  emit_push(ACC, s);
  emit_move(ACC, SP, s); // stack end
  emit_move(A1, ZERO, s); // allocate nothing
  s << JAL << gc_collect_names[cgen_Memmgr] << endl;
  emit_addiu(SP,SP,4,s);
  emit_load(ACC,0,SP,s);
}

static void emit_gc_check(char *source, ostream &s)
{
  if (source != (char*)A1) emit_move(A1, source, s);
  s << JAL << "_gc_check" << endl;
}

static void get_args(char *dest, int index, int length, ostream &s){
  int offset = length - index + 2;
  emit_load(dest, offset, FP, s);
}

static void emit_object(Symbol name, ostream &s){
  auto obj_entry_ptr = obj_table->lookup(name);
  assert(obj_entry_ptr);

  switch (obj_entry_ptr->type)
  {
  case TYPE_ARGS:
    s << "-" << obj_entry_ptr->index * 4 + 8 << "($fp)";
    break;
  case TYPE_ATTR:
    s << obj_entry_ptr->index * 4 + 12 << "($s0)";
    break;
  case TYPE_CASE:
  case TYPE_LET:
    s << obj_entry_ptr->index * 4 + 4 << "($fp)"; 
    break;
  default:
    assert(true);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << STRCONST_PREFIX << index;  // Dyy: something like ``str_const1'' ?
}

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream& s, int stringclasstag)
{
  IntEntryP lensym = inttable.add_int(len);

  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s  << LABEL                                             // label
      << WORD << stringclasstag << endl                                 // tag
      << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len+4)/4) << endl // size
      << WORD;


 /***** Add dispatch information for class String ******/
      s << "String" << DISPTAB_SUFFIX;

      s << endl;                                              // dispatch table
      s << WORD;  lensym->code_ref(s);  s << endl;            // string length
  emit_string_constant(s,str);                                // ascii string
  s << ALIGN;                                                 // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s, int stringclasstag)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,stringclasstag);
}

//
// Ints
//
void IntEntry::code_ref(ostream &s)
{
  s << INTCONST_PREFIX << index;
}

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream &s, int intclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                // label
      << WORD << intclasstag << endl                      // class tag
      << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl  // object size
      << WORD; 

 /***** Add dispatch information for class Int ******/
      s << "Int" << DISPTAB_SUFFIX;

      s << endl;                                          // dispatch table
      s << WORD << str << endl;                           // integer value
}


//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream &s, int intclasstag)
{
  for (List<IntEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,intclasstag);
}


//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream& s) const
{
  s << BOOLCONST_PREFIX << val;
}
  
//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream& s, int boolclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                  // label
      << WORD << boolclasstag << endl                       // class tag
      << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl   // object size
      << WORD;

 /***** Add dispatch information for class Bool ******/
      s << "Bool" << DISPTAB_SUFFIX;

      s << endl;                                            // dispatch table
      s << WORD << val << endl;                             // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//***************************************************
//
//  Emit code to start the .data segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_data()
{
  Symbol main    = idtable.lookup_string(MAINNAME);
  Symbol string  = idtable.lookup_string(STRINGNAME);
  Symbol integer = idtable.lookup_string(INTNAME);
  Symbol boolc   = idtable.lookup_string(BOOLNAME);

  str << "\t.data\n" << ALIGN;
  //
  // The following global names must be defined first.
  //
  str << GLOBAL << CLASSNAMETAB << endl;
  str << GLOBAL; emit_protobj_ref(main,str);    str << endl;
  str << GLOBAL; emit_protobj_ref(integer,str); str << endl;
  str << GLOBAL; emit_protobj_ref(string,str);  str << endl;
  str << GLOBAL; falsebool.code_ref(str);  str << endl;
  str << GLOBAL; truebool.code_ref(str);   str << endl;
  str << GLOBAL << INTTAG << endl;
  str << GLOBAL << BOOLTAG << endl;
  str << GLOBAL << STRINGTAG << endl;

  //
  // We also need to know the tag of the Int, String, and Bool classes
  // during code generation.
  //
  str << INTTAG << LABEL
      << WORD << intclasstag << endl;
  str << BOOLTAG << LABEL 
      << WORD << boolclasstag << endl;
  str << STRINGTAG << LABEL 
      << WORD << stringclasstag << endl;    
}


//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text()
{
  str << GLOBAL << HEAP_START << endl
      << HEAP_START << LABEL 
      << WORD << 0 << endl
      << "\t.text" << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Main"), str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Int"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("String"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Bool"),str);
  str << endl << GLOBAL;
  emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
  str << endl;
}

void CgenClassTable::code_bools(int boolclasstag)
{
  falsebool.code_def(str,boolclasstag);
  truebool.code_def(str,boolclasstag);
}

void CgenClassTable::code_select_gc()
{
  //
  // Generate GC choice constants (pointers to GC functions)
  //
  str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
  str << "_MemMgr_INITIALIZER:" << endl;
  str << WORD << gc_init_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
  str << "_MemMgr_COLLECTOR:" << endl;
  str << WORD << gc_collect_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_TEST" << endl;
  str << "_MemMgr_TEST:" << endl;
  str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
}


//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************

void CgenClassTable::code_constants()
{
  //
  // Add constants that are required by the code generator.
  //
  stringtable.add_string("");
  inttable.add_string("0");

  stringtable.code_string_table(str,stringclasstag);  // 就是把string_table里的每个string都放进.data里
  inttable.code_string_table(str,intclasstag);
  code_bools(boolclasstag);
}


CgenClassTable::CgenClassTable(Classes classes, ostream& s) : nds(NULL) , str(s)
{
   stringclasstag = 4 /* Change to your String class tag here */;
   intclasstag =    2 /* Change to your Int class tag here */;
   boolclasstag =   3 /* Change to your Bool class tag here */;

   enterscope();
   if (cgen_debug) cout << "Building CgenClassTable" << endl;
   install_basic_classes();
   install_classes(classes);
   build_inheritance_tree();

   code();
   exitscope();
}

void CgenClassTable::install_basic_classes()
{

// The tree package uses these globals to annotate the classes built below.
  // curr_lineno  = 0;
  Symbol filename = stringtable.add_string("<basic class>");  // 他也在string_const里

//
// A few special class names are installed in the lookup table but not
// the class list.  Thus, these classes exist, but are not part of the
// inheritance hierarchy.
// No_class serves as the parent of Object and the other special classes.
// SELF_TYPE is the self class; it cannot be redefined or inherited.
// prim_slot is a class known to the code generator.
//

  // 他们都会在string_const里
  addid(No_class,
	new CgenNode(class_(No_class,No_class,nil_Features(),filename),
			    Basic,this));
  addid(SELF_TYPE,
	new CgenNode(class_(SELF_TYPE,No_class,nil_Features(),filename),
			    Basic,this));
  addid(prim_slot,
	new CgenNode(class_(prim_slot,No_class,nil_Features(),filename),
			    Basic,this));

// 
// The Object class has no parent class. Its methods are
//        cool_abort() : Object    aborts the program
//        type_name() : Str        returns a string representation of class name
//        copy() : SELF_TYPE       returns a copy of the object
//
// There is no need for method bodies in the basic classes---these
// are already built in to the runtime system.
//
  install_class(
   new CgenNode(
    class_(Object, 
	   No_class,
	   append_Features(
           append_Features(
           single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
           single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
           single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	   filename),
    Basic,this));

// 
// The IO class inherits from Object. Its methods are
//        out_string(Str) : SELF_TYPE          writes a string to the output
//        out_int(Int) : SELF_TYPE               "    an int    "  "     "
//        in_string() : Str                    reads a string from the input
//        in_int() : Int                         "   an int     "  "     "
//
   install_class(
    new CgenNode(
     class_(IO, 
            Object,
            append_Features(
            append_Features(
            append_Features(
            single_Features(method(out_string, single_Formals(formal(arg, Str)),
                        SELF_TYPE, no_expr())),
            single_Features(method(out_int, single_Formals(formal(arg, Int)),
                        SELF_TYPE, no_expr()))),
            single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
            single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	   filename),	    
    Basic,this));

//
// The Int class has no methods and only a single attribute, the
// "val" for the integer. 
//
   install_class(
    new CgenNode(
     class_(Int, 
	    Object,
            single_Features(attr(val, prim_slot, no_expr())),
	    filename),
     Basic,this));

//
// Bool also has only the "val" slot.
//
    install_class(
     new CgenNode(
      class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename),
      Basic,this));

//
// The class Str has a number of slots and operations:
//       val                                  ???
//       str_field                            the string itself
//       length() : Int                       length of the string
//       concat(arg: Str) : Str               string concatenation
//       substr(arg: Int, arg2: Int): Str     substring
//       
   install_class(
    new CgenNode(
      class_(Str, 
	     Object,
             append_Features(
             append_Features(
             append_Features(
             append_Features(
             single_Features(attr(val, Int, no_expr())),
            single_Features(attr(str_field, prim_slot, no_expr()))),
            single_Features(method(length, nil_Formals(), Int, no_expr()))),
            single_Features(method(concat, 
				   single_Formals(formal(arg, Str)),
				   Str, 
				   no_expr()))),
	    single_Features(method(substr, 
				   append_Formals(single_Formals(formal(arg, Int)), 
						  single_Formals(formal(arg2, Int))),
				   Str, 
				   no_expr()))),
	     filename),
        Basic,this));

}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_class(CgenNodeP nd)
{
  class_count++;

  Symbol name = nd->get_name();

  if (probe(name))
    {
      return;
    }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds = new List<CgenNode>(nd,nds);
  addid(name,nd);
}

void CgenClassTable::install_classes(Classes cs)
{
  for(int i = cs->first(); cs->more(i); i = cs->next(i))
    install_class(new CgenNode(cs->nth(i),NotBasic,this));
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree()
{
  for(List<CgenNode> *l = nds; l; l = l->tl())
      set_relations(l->hd());
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNodeP nd)
{
  CgenNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n)
{
  children = new List<CgenNode>(n,children);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}

void CgenNode::add_dispatch_func(Symbol func_name, Symbol class_name){
  for (List<DispatchEntry> *l = dispatch_table; l; l = l->tl()){
    if (equal_Symbol(l->hd()->func_name, func_name)){
      return;
    }
  }

  dispatch_table = new List<DispatchEntry>(new DispatchEntry{class_name, func_name}, dispatch_table);
}

void CgenNode::gen_dispatch_tbl(CgenNodeP curr_class){  
  assert(curr_class);

  Features features = curr_class->get_features();

  for (int i = features->first(); features->more(i); i = features->next(i)){
    Feature feature = features->nth(i);
    if (feature->is_method()){
      this->add_dispatch_func(feature->get_name(), curr_class->get_name());
    }
  }

  if (!equal_Symbol(curr_class->get_name(), No_class)){
    this->gen_dispatch_tbl(curr_class->get_parentnd());
  }
}

void CgenNode::code_dispatch_table(ostream &s){
  s << get_name() << DISPTAB_SUFFIX << LABEL ;
  for(List<DispatchEntry> *l = dispatch_table; l; l = l->tl()){
    s << WORD << l->hd()->class_name << "." << l->hd()->func_name << endl;
  }
}

void CgenNode::fetch_attrs(CgenNodeP curr_class){
  if (!equal_Symbol(curr_class->get_parentnd()->get_name(), No_class)){
    fetch_attrs(curr_class->get_parentnd());
  }

  Features features = curr_class->get_features();
  for (int i = features->first(); features->more(i); i = features->next(i)) {
    Feature feature = features->nth(i);
    
    if (feature->is_method()){
      continue;
    }
    attr_count++;
    attrs = new List<AttrEntry>(new AttrEntry{feature->get_name(), feature->get_type()}, attrs);
  }
}

void CgenNode::code_prototype_object(ostream &s, int index){

  fetch_attrs(this);

  // Reverse `attrs`
  List<AttrEntry> *tmpptr = NULL;
  for (; attrs; ){
    tmpptr = new List<AttrEntry>(attrs->hd(), tmpptr);
    List<AttrEntry> * old_attrs = attrs;
    attrs = attrs->tl();
    delete old_attrs;
  }
  attrs = tmpptr;

  s << WORD << -1 << endl;

  s << get_name() << PROTOBJ_SUFFIX << LABEL <<
    WORD << index << endl <<
    WORD << attr_count + 3 << endl <<
    WORD << get_name() << DISPTAB_SUFFIX << endl;

  for (List<AttrEntry> *ptr = attrs; ptr; ptr = ptr->tl()){
    Symbol type = ptr->hd()->decl_type;
    Symbol name = ptr->hd()->name;

    if (equal_Symbol(type, Str)){
      s << WORD;
      stringtable.lookup_string("")->code_ref(s);
      s << endl;

    } else {
      if (equal_Symbol(type, Int)){
        s << WORD;
        inttable.lookup_string("0")->code_ref(s);
        s << endl;

      } else {
        if (equal_Symbol(type, Bool)){
          s << WORD;
          falsebool.code_ref(s);
          s << endl;

        } else {
          s << WORD << 0 << endl;
        }
      }
    }
    
    
  }
}

void CgenClassTable::code_nametable(){
  str << CLASSNAMETAB << LABEL;
  for (List<CgenNode> *l = nds; l; l = l->tl()){
    Symbol class_name = l->hd()->get_name();
    StringEntry *entry = stringtable.lookup_string(class_name->get_string());
    str << WORD;
    entry->code_ref(str);
    str << endl;
  }
}

void CgenClassTable::code_objtable(){
  str << CLASSOBJTAB << LABEL;
  for (List<CgenNode> *l = nds; l; l = l->tl()){
    Symbol class_name = l->hd()->get_name();
    str << WORD << class_name->get_string() << PROTOBJ_SUFFIX << endl;
    str << WORD << class_name->get_string() << CLASSINIT_SUFFIX << endl;
  }
}

void CgenClassTable::code_dispatch_table(){
  for (List<CgenNode> *l = nds; l; l = l->tl()){
    l->hd()->gen_dispatch_tbl(l->hd());
    l->hd()->code_dispatch_table(str);
  }
}

void CgenClassTable::code_prototype_object(){
  int index = 0;

  for (List<CgenNode> *l = nds; l; l = l->tl()){
    l->hd()->code_prototype_object(str, index++);
  }
}

void CgenClassTable::reverse_nds(){
  List<CgenNode> *new_nds = NULL;
  for (; nds; nds = nds->tl()){
    new_nds = new List<CgenNode>(nds->hd(), new_nds);
  }

  nds = new_nds;
}


void code_start(ostream& s){
  emit_addiu(SP, SP, -12, s);
  emit_store(FP, 3, SP, s);
  emit_store(S0, 2, SP, s);
  emit_store(RA, 1, SP, s);
  emit_addiu(FP, SP, 4, s);
  emit_move(S0, ACC, s);
}

void code_end(ostream& s, int arg_num) {
  emit_move(ACC, S0, s);
  emit_load(FP, 3, SP, s);
  emit_load(S0, 2, SP, s);
  emit_load(RA, 1, SP, s);
  emit_addiu(SP, SP, 12 + arg_num * 4, s);
  emit_return(s); 
}

void CgenClassTable::code_init(){
  for (List<CgenNode> *l = nds; l; l = l->tl()){
    l->hd()->code_init(str);
  }
} 

int CgenNode::get_attr_index(Symbol name){
  int count = 0;
  for (auto l = attrs; l; l = l->tl()){
    if (equal_Symbol(l->hd()->name, name)){
      return count;
    }
    count++;
  }

  // Should never go here !!!
  assert(true);
  return -1;
}

int CgenNode::get_method_index(Symbol name){
    int count = 0;
  for (auto l = dispatch_table; l; l = l->tl()){
    if (equal_Symbol(l->hd()->func_name, name)){
      return count;
    }
    count++;
  }

  // Should never go here !!!
  assert(true);
  return -1;
}

void CgenNode::code_init(ostream &str){
  str << get_name() << CLASSINIT_SUFFIX << LABEL;

  code_start(str);
  if (!equal_Symbol(get_name(), Object)){
    str << JAL << get_parent()->get_string() << CLASSINIT_SUFFIX << endl;
  }

  Features features = get_features();
  for (int i = features->first(); 
    features->more(i); 
    i = features->next(i)){

    Feature feature = features->nth(i);

    if (feature->is_method()){
      continue;
    }

    Expression init = feature->get_init();
    assert(init);
    if (init->is_no_expr()){
      continue;
    }

    init->code(str);
    // We need to query index because we do not know how many 
    // attrs are from ancient 
    int index = get_attr_index(feature->get_name());
    emit_store(ACC, 3 + index, S0, str);
  }
  
  code_end(str, 0);
}

void CgenClassTable::code_methods(){
  for (auto *l = nds; l; l = l->tl()){
    l->hd()->code_methods(str);
  }
}

void CgenNode::code_methods(ostream &str){
  // basic class is defiend in running time system
  if (basic_status){
    return ;
  }

  curr_filename = filename;
  obj_table->enterscope();
  int index = 0;

  for (auto l = attrs; l; l = l->tl()){
    obj_table->addid(
      l->hd()->name, 
      new ObjEntry{TYPE_ATTR, index}
    );
    index++;
  }

  Features features = get_features();

  for (
    int i = features->first();
    features->more(i);
    i = features->next(i)
  ){
    Feature feature = features->nth(i);

    if (feature->is_method()){
      emit_method_ref(name, feature->get_name(), str);
      str << LABEL;

      feature->code(str);
    }
  }

  if (cgen_debug) obj_table->dump();

  obj_table->exitscope();
}

void method_class::code(ostream &s){
  code_start(s);

  obj_table->enterscope();
  // Take care!
  int index = formals->len();

  for (int i = formals->first(); 
      formals->next(i); 
      i = formals->next(i)){
    Symbol name = formals->nth(i)->get_name();
    obj_table->addid(name, new ObjEntry{TYPE_ARGS, index--});
  }

  // save space for temp vars
  // locate temp vars using 4 + x * 4($fp)
  int temp_count = expr->temp_count();
  emit_addiu(SP, SP, -4 * temp_count, s);

  expr->code(s);

  // pop stack up
  emit_addiu(SP, SP, 4 * temp_count, s);

  obj_table->exitscope();
  code_end(s, formals->len());
}


void CgenClassTable::code()
{
  table_ptr = this; 

  if (cgen_debug) cout << "coding global data" << endl;
  code_global_data();

  if (cgen_debug) cout << "choosing gc" << endl;
  code_select_gc();

  if (cgen_debug) cout << "coding constants" << endl;
  code_constants();

//                 Add your code to emit
//                   - prototype objects
//                   - class_nameTab
//                   - dispatch tables
//
  reverse_nds();

  if (cgen_debug) cout << "coding name table" << endl;
  code_nametable();

  if (cgen_debug) cout << "coding object table" << endl;
  code_objtable();

  if (cgen_debug) cout << "coding dispatch table" << endl;
  code_dispatch_table();

  if (cgen_debug) cout << "coding prototype ondjects" << endl;
  code_prototype_object();

  if (cgen_debug) cout << "coding global text" << endl;
  code_global_text();

//                 Add your code to emit
//                   - object initializer
//                   - the class methods
//                   - etc...
  if (cgen_debug) cout << "coding initializer" << endl;
  code_init();

  if (cgen_debug) cout << "coding methods" << endl;
  code_methods();
}


CgenNodeP CgenClassTable::root()
{
   return probe(Object);
}


///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct) :
   class__class((const class__class &) *nd),
   parentnd(NULL),
   children(NULL),
   basic_status(bstatus)
{ 
   stringtable.add_string(name->get_string());          // Add class name to string table
}


//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

void assign_class::code(ostream &s) {
  expr->code(s);
  s << SW << ACC << " ";
  emit_object(name, s);
  s << endl;
}

void handle_dispatch_abort(int lineno, Symbol filename, ostream& s){
  // check if is void
  emit_bne(ACC, ZERO, label_index, s);

  StringEntry *entry = stringtable.lookup_string(filename->get_string());
  emit_load_string(ACC, entry, s);
  emit_load_imm(T1, lineno, s);
  emit_jal("_dispatch_abort", s);

  emit_label_def(label_index, s);
  label_index++;
}

void static_dispatch_class::code(ostream &s) {
  // Push args
  for (
    int i = actual->first();
    actual->more(i);
    i = actual->next(i)
  )
  {
    Expression expr = actual->nth(i);
    expr->code(s);
    emit_push(ACC, s);
  }

  // generate e0 (expr)
  expr->code(s);
  handle_dispatch_abort(line_number, curr_filename, s);

  auto node_ptr = table_ptr->lookup(type_name);
  int offset = node_ptr->get_method_index(name);

  // The key of static dispatch is we should use 
  // the dispatch table in declared type's prototype obj
  s << LW << T1 << " " ;
  emit_protobj_ref(type_name, s);
  s << endl;

  emit_load(T1, 2, T1, s);
  emit_load(T1, offset, T1, s);
  emit_jalr(T1, s);
}

void dispatch_class::code(ostream &s) {
    // Push args
  for (
    int i = actual->first();
    actual->more(i);
    i = actual->next(i)
  )
  {
    Expression expr = actual->nth(i);
    expr->code(s);
    emit_push(ACC, s);
  }

  // generate e0 (expr)
  expr->code(s);
  handle_dispatch_abort(line_number, curr_filename, s);

  auto node_ptr = table_ptr->lookup(get_type());
  int offset = node_ptr->get_method_index(name);

  // In dynamic dispatch we should use the dispatch
  // table of that object 
  emit_load(T1, 2, ACC, s);
  emit_load(T1, offset, T1, s);
  emit_jalr(T1, s);
}

void cond_class::code(ostream &s) {
  pred->code(s);
  
  int then_label_index = label_index;
  int end_label_index = label_index + 1;
  label_index += 2;

  emit_load_bool(T1, truebool, s);
  emit_beq(T1, ACC, then_label_index, s);

  // else part
  else_exp->code(s);
  emit_branch(end_label_index, s);

  // then part
  emit_label_def(then_label_index, s);
  then_exp->code(s);

  // end (only a label)
  emit_label_def(end_label_index, s);
}

void loop_class::code(ostream &s) {
  int begin_label_index = label_index;
  int end_label_index = label_index + 1;
  label_index += 2;

  // while (**pred**)
  emit_label_def(begin_label_index, s);
  pred->code(s);
  emit_load_bool(T1, falsebool, s);
  emit_beq(T1, ACC, end_label_index, s);

  // {**body**}
  body->code(s);
  emit_branch(begin_label_index, s);

  emit_label_def(end_label_index, s);

  // return void
  emit_load_imm(ACC, 0, s);
}

void typcase_class::code(ostream &s) {
  expr->code(s);

  // check dispatch on void
  emit_bne(ACC, ZERO, label_index, s);

  StringEntry *entry = stringtable.lookup_string(curr_filename->get_string());
  emit_load_string(ACC, entry, s);
  emit_load_imm(T1, get_line_number(), s);
  emit_jal("_case_abort2", s);

  emit_label_def(label_index, s);
  label_index++;


  int end_label_index = label_index + cases->len();

  for (int i = cases->first(); 
      cases->more(i);
      i = cases->next(i)){
    Case branch = cases->nth(i);

    emit_label_def(label_index, s);

    // consider if I should jump to next
    s << LW << T1 << "0(" << branch->get_type_decl()
        << PROTOBJ_SUFFIX << ")";
    emit_load(T2, 0, ACC, s);
    emit_bne(T1, T2, label_index + 1, s);

    // add a new temp var
    obj_table->enterscope();
    obj_table->addid(
        branch->get_name(), 
        new ObjEntry{TYPE_CASE, obj_index}
    );

    // move ACC into this temp var
    emit_store(ACC, 1 + obj_index, FP, s);
    obj_index++;

    branch->get_expr()->code(s);

    obj_table->exitscope();
    obj_index--;
    emit_branch(end_label_index, s);
  }

  // If reachs here, means statement has no match
  // Object should already be put in ACC, so the
  // only thing we need to do is call `_case_abort`
  
  s << JAL << "_case_abort" << endl;

  // Normally should directy jump to here
  emit_label_def(end_label_index, s);
}

void block_class::code(ostream &s) {
  for (int i = body->first(); 
      body->more(i);
      i = body->next(i))
  {
    Expression one_expr = body->nth(i);

    one_expr->code(s);
  }
}

void let_class::code(ostream &s) {
  // should execute before new var is added into env
  if (!init->is_no_expr()){
    init->code(s);
  }

  obj_table->enterscope();
  obj_table->addid(identifier, new ObjEntry{TYPE_LET, obj_index});

  // set default value for this temp var
  if (init->is_no_expr()){
    if (equal_Symbol(type_decl, Str)){ // if is a String
      emit_load_string(T1, stringtable.lookup_string(""), s);
      emit_store(T1, 1 + obj_index, FP, s);
    } 
    else {  // If is a Int
      if (equal_Symbol(type, Int)){
        emit_load_int(T1, inttable.lookup_string("0"), s);
        emit_store(T1, 1 + obj_index, FP, s);
      } 
      else {  // If is a Bool
        if (equal_Symbol(type, Bool)){
          emit_load_bool(T1, falsebool, s);
          emit_store(T1, 1 + obj_index, FP, s);
        } 
        else {
          emit_load_imm(T1, 0, s);
          emit_store(T1, 1 + obj_index, FP, s);
        }
      }
    }
  }  
  // like Assign, init and move in
  else {
    emit_store(ACC, 1 + obj_index, FP, s);
  }

  obj_index++;

  body->code(s); // execute

  obj_table->exitscope();
  obj_index--;
}

void plus_class::code(ostream &s) {
  // store left expr at stack
  e1->code(s);
  emit_store(ACC, 1 + obj_index, FP, s);
  obj_index++;

  // copy right expr (will be use as result)
  e2->code(s);
  s << JAL << "Object.copy" << endl;
  obj_index--;

  // calculate arith result
  emit_load(T1, 1 + obj_index, FP, s);
  emit_load(T2, 3, T1, s);
  emit_load(T3, 3, ACC, s);
  emit_add(T1, T2, T3, s);

  // store arith result
  emit_store(T1, 3, ACC, s);
}

void sub_class::code(ostream &s) {
    // store left expr at stack
  e1->code(s);
  emit_store(ACC, 1 + obj_index, FP, s);
  obj_index++;

  // copy right expr (will be use as result)
  e2->code(s);
  s << JAL << "Object.copy" << endl;
  obj_index--;

  // calculate arith result
  emit_load(T1, 1 + obj_index, FP, s);
  emit_load(T2, 3, T1, s);
  emit_load(T3, 3, ACC, s);
  emit_sub(T1, T2, T3, s);

  // store arith result
  emit_store(T1, 3, ACC, s);
}

void mul_class::code(ostream &s) {
    // store left expr at stack
  e1->code(s);
  emit_store(ACC, 1 + obj_index, FP, s);
  obj_index++;

  // copy right expr (will be use as result)
  e2->code(s);
  s << JAL << "Object.copy" << endl;
  obj_index--;

  // calculate arith result
  emit_load(T1, 1 + obj_index, FP, s);
  emit_load(T2, 3, T1, s);
  emit_load(T3, 3, ACC, s);
  emit_mul(T1, T2, T3, s);

  // store arith result
  emit_store(T1, 3, ACC, s);
}

void divide_class::code(ostream &s) {
    // store left expr at stack
  e1->code(s);
  emit_store(ACC, 1 + obj_index, FP, s);
  obj_index++;

  // copy right expr (will be use as result)
  e2->code(s);
  s << JAL << "Object.copy" << endl;

  // calculate arith result
  emit_load(T1, 1 + obj_index - 1, FP, s);
  emit_load(T2, 3, T1, s);
  emit_load(T3, 3, ACC, s);
  emit_div(T1, T2, T3, s);
  obj_index--;

  // store arith result
  emit_store(T1, 3, ACC, s);
}

void neg_class::code(ostream &s) {
  e1->code(s);
  emit_load(T1, 3, ACC, s);
  emit_neg(T1, T1, s);
  emit_store(T1, 3, ACC, s);
}

void lt_class::code(ostream &s) {
  e1->code(s);
  emit_store(ACC, 1 + obj_index, FP, s);
  obj_index++;

  e2->code(s);

  emit_load(T1, 1 + obj_index - 1, FP, s);
  emit_load(T2, 3, T1, s);
  emit_load(T3, 3, ACC, s);
  obj_index--;

  emit_load_bool(ACC, truebool, s);
  emit_blt(T2, T3, label_index, s);
  emit_load_bool(ACC, falsebool, s);
  emit_label_def(label_index, s);
  label_index++;
}

void eq_class::code(ostream &s) {
  Symbol type = e1->get_type();

  if (
    equal_Symbol(type, Int) ||
    equal_Symbol(type, Str) ||
    equal_Symbol(type, Bool)
  )
  // For Int, Bool, String: should check value
  {
    e1->code(s);
    emit_store(ACC, 1 + obj_index, FP, s);
    obj_index++;

    e2->code(s);

    // call `equality_test`
    emit_store(T1, 1 + obj_index - 1, FP, s);
    emit_move(T2, ACC, s);
    obj_index--;
    emit_load_bool(ACC, truebool, s);
    emit_beq(T1, T2, label_index, s); // If have same ptr, must be equal
    emit_load_bool(A1, falsebool, s);
    emit_jal("equality_test", s);

    emit_label_def(label_index, s);
    label_index++;
  }
  // For others, just check if has same ptr
  else{
    e1->code(s);
    emit_store(ACC, 1 + obj_index, FP, s);
    obj_index++;

    e2->code(s);

    emit_store(T1, 1 + obj_index - 1, FP, s);
    emit_move(T2, ACC, s);
    emit_load_bool(ACC, truebool, s);
    emit_beq(T1, T2, label_index, s);
    emit_load_bool(ACC, falsebool, s);
    
    emit_label_def(label_index, s);
    label_index++;
  }

}

void leq_class::code(ostream &s) {
  e1->code(s);
  emit_store(ACC, 1 + obj_index, FP, s);
  obj_index++;

  e2->code(s);

  emit_load(T1, 1 + obj_index - 1, FP, s);
  emit_load(T2, 3, T1, s);
  emit_load(T3, 3, ACC, s);
  obj_index--;

  emit_load_bool(ACC, truebool, s);
  emit_bleq(T2, T3, label_index, s);
  emit_load_bool(ACC, falsebool, s);
  emit_label_def(label_index, s);
  label_index++;
}

void comp_class::code(ostream &s) {
  e1->code(s);
  emit_load(T1, 3, ACC, s);
  emit_load_bool(ACC, truebool, s);
  emit_beqz(T1, label_index, s);
  emit_load_bool(ACC, falsebool, s);
  emit_label_def(label_index, s);
  label_index++;
}

void int_const_class::code(ostream& s)  
{
  //
  // Need to be sure we have an IntEntry *, not an arbitrary Symbol
  //
  emit_load_int(ACC,inttable.lookup_string(token->get_string()),s);
}

void string_const_class::code(ostream& s)
{
  emit_load_string(ACC,stringtable.lookup_string(token->get_string()),s);
}

void bool_const_class::code(ostream& s)
{
  emit_load_bool(ACC, BoolConst(val), s);
}

void new__class::code(ostream &s) {
  // new SELF_TYPE
  if (equal_Symbol(type_name, SELF_TYPE)){
    // We should:
    // 1. find self_protObj in objTab
    // 2. Object.copy
    // 3. call self.init

    emit_load_address(T1, CLASSOBJTAB, s);
    emit_load(T2, 0, S0, s);
    emit_sll(T2, T2, 3, s);
    emit_addu(T1, T1, T2, s);
    emit_move(S1, T1, s); // S1 can kept through func call

    emit_load(ACC, 0, S1, s);  // put SELF_protObj into ACC
    emit_jal("Object.copy", s);

    emit_load(T1, 1, S1, s);
    emit_jalr(T1, s);
  }
  else{
    s << LA << ACC << " " << type_name << PROTOBJ_SUFFIX << endl;
    s << JAL << "Object.copy" << endl;
    s << JAL << type_name << CLASSINIT_SUFFIX << endl;
  }

}

void isvoid_class::code(ostream &s) {
  e1->code(s);
  emit_move(T1, ACC, s);
  emit_load_bool(ACC, truebool, s);
  emit_beqz(T1, label_index++, s);
  emit_load_bool(ACC, falsebool, s);
  emit_label_def(label_index, s);
}

void no_expr_class::code(ostream &s) {
  return ;
}

void object_class::code(ostream &s) {
  if (equal_Symbol(name, self)){
    emit_move(ACC, S0, s);
  }
  else{
    s << LW << ACC << " "; 
    emit_object(name, s);
    s << endl;
  }
}

/************************
 *      temp count
 ************************/

int assign_class::temp_count() {
  return expr->temp_count();
}

// we still use stack push and pop for dispatch.
// That means, the result of each args should not be counted
// in the temp vars
 
int static_dispatch_class::temp_count() {
  int maximum = expr->temp_count();
  for (int i = actual->first(); actual->more(i); i = actual->next(i)){
    maximum  = max(maximum, actual->nth(i)->temp_count());
  }
  return maximum;
}

int dispatch_class::temp_count() {
  int maximum = expr->temp_count();
  for (int i = actual->first(); actual->more(i); i = actual->next(i)){
    maximum  = max(maximum, actual->nth(i)->temp_count());
  }
  return maximum;
}

int cond_class::temp_count() {
  return max(pred->temp_count(), 
    max(then_exp->temp_count(), else_exp->temp_count())
  );
}

int loop_class::temp_count() {
  return max(pred->temp_count(), body->temp_count());
}

int typcase_class::temp_count() {
  int maximum = 0;
  for (int i = cases->first(); cases->more(i); i = cases->next(i)){
    maximum = max(maximum, cases->nth(i)->get_expr()->temp_count());
  }

  // Each branch will have a new temp
  return maximum + 1;
}

int block_class::temp_count() {
  int maximum = 0;
  for (int i = body->first(); body->more(i); i = body->next(i)){
    maximum = max(maximum, body->nth(i)->temp_count());
  }

  return maximum;
}

int let_class::temp_count() {
  int init_count = init->temp_count();
  int body_count = body->temp_count();

  // Only body has a new var
  return max(init_count, body_count + 1);
}

int plus_class::temp_count() {
  int left_count = e1->temp_count();
  int right_count = e2->temp_count();

  // We need to store left when calculating right
  return max(left_count, right_count + 1);
}

int sub_class::temp_count() {
  int left_count = e1->temp_count();
  int right_count = e2->temp_count();

  // We need to store left when calculating right
  return max(left_count, right_count + 1);
}

int mul_class::temp_count() {
  int left_count = e1->temp_count();
  int right_count = e2->temp_count();

  // We need to store left when calculating right
  return max(left_count, right_count + 1);
}

int divide_class::temp_count() {
  int left_count = e1->temp_count();
  int right_count = e2->temp_count();

  // We need to store left when calculating right
  return max(left_count, right_count + 1);
}

int neg_class::temp_count() {
  return e1->temp_count();
}

int lt_class::temp_count() {
  int left_count = e1->temp_count();
  int right_count = e2->temp_count();

  // We need to store left when calculating right
  return max(left_count, right_count + 1);
}

int eq_class::temp_count() {
  int left_count = e1->temp_count();
  int right_count = e2->temp_count();

  // We need to store left when calculating right
  return max(left_count, right_count + 1);
}

int leq_class::temp_count() {
  int left_count = e1->temp_count();
  int right_count = e2->temp_count();

  // We need to store left when calculating right
  return max(left_count, right_count + 1);
}

int comp_class::temp_count() {
  return e1->temp_count();
}

int int_const_class::temp_count() {
  return 0;
}

int string_const_class::temp_count() {
  return 0;
}

int bool_const_class::temp_count() {
  return 0;
}

int new__class::temp_count() {
  return 0;
}

int isvoid_class::temp_count() {
  return e1->temp_count();
}

int no_expr_class::temp_count() {
  return 0;
}

int object_class::temp_count() {
  return 0;
}


