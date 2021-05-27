#include <assert.h>
#include <stdio.h>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"

enum Basicness     {Basic, NotBasic};
#define TRUE 1
#define FALSE 0
#define TYPE_ATTR 0
#define TYPE_ARGS 1
#define TYPE_LET  2
#define TYPE_CASE 3

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

typedef struct{
   int type;
   int index;  // for args, index = arg_sum - arg_index
               // (In order to calculate offset)
}ObjEntry;

class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
   List<CgenNode> *nds;
   ostream& str;
   int stringclasstag;
   int intclasstag;
   int boolclasstag;
   int class_count;


// The following methods emit code for
// constants and global declarations.

   void code_global_data();
   void code_global_text();
   void code_bools(int);
   void code_select_gc();
   void code_constants();
   void code_objtable();
   void code_nametable();
   void code_dispatch_table();
   void code_prototype_object();
   void code_init();
   void code_methods();

// The following creates an inheritance graph from
// a list of classes.  The graph is implemented as
// a tree of `CgenNode', and class names are placed
// in the base class symbol table.

   void install_basic_classes();
   void install_class(CgenNodeP nd);
   void install_classes(Classes cs);
   void build_inheritance_tree();
   void set_relations(CgenNodeP nd);
   void reverse_nds();

public:
   CgenClassTable(Classes, ostream& str);
   void code();
   CgenNodeP root();
};

typedef struct {
   Symbol class_name;
   Symbol func_name;
} DispatchEntry;

typedef struct{
   Symbol name;
   Symbol decl_type;
} AttrEntry;

class CgenNode : public class__class {
private: 
   CgenNodeP parentnd;                        // Parent of class
   List<CgenNode> *children;                  // Children of class
   Basicness basic_status;                    // `Basic' if class is basic
                                              // `NotBasic' otherwise 
   List<DispatchEntry> *dispatch_table;
   List<AttrEntry> *attrs;
   int attr_count;

public:
   CgenNode(Class_ c,
            Basicness bstatus,
            CgenClassTableP class_table);

   void add_child(CgenNodeP child);
   List<CgenNode> *get_children() { return children; }
   void set_parentnd(CgenNodeP p);
   CgenNodeP get_parentnd() { return parentnd; }
   int basic() { return (basic_status == Basic); }
   void add_dispatch_func(Symbol func_name, Symbol class_name);
   void gen_dispatch_tbl(CgenNodeP);
   void code_dispatch_table(ostream &s);
   void code_prototype_object(ostream &s, int class_index);
   void fetch_attrs(CgenNodeP current_class);
   void code_init(ostream &str);
   int get_attr_index(Symbol name);
   int get_method_index(Symbol name);
   void code_methods(ostream &str);
};

class BoolConst 
{
 private: 
  int val;
 public:
  BoolConst(int);
  void code_def(ostream&, int boolclasstag);
  void code_ref(ostream&) const;
};

bool equal_Symbol(Symbol x, Symbol y){
   return x->equal_string(y->get_string(), y->get_len());
}