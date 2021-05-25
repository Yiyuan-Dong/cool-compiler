#ifndef COOL_TREE_H
#define COOL_TREE_H
//////////////////////////////////////////////////////////
//
// file: cool-tree.h
//
// This file defines classes for each phylum and constructor
//
//////////////////////////////////////////////////////////


#include "tree.h"
#include "cool-tree.handcode.h"
#include "symtab.h"

#define TYPE_METHOD (0)
#define TYPE_ATTR   (1)

// PA4 declares

typedef struct {
    Symbol name;
    Symbol type_decl; 
} formal_node;

typedef struct {
    formal_node* formals;
    int formal_count;
    Symbol return_type;
    Symbol name;
    Feature method;
} method_node;

typedef struct {
    int visit_flag;
    Symbol name;
    Symbol parent;
    int parent_index;
    int depth;
    method_node* method_nodes;
    int method_count;
    Class_ class_;
} class_node;

int find_symbol(Symbol);
bool check_parent(int, int);
Symbol LCA(Symbol, Symbol);
Symbol fetch_class_name(int);
ostream& semant_error(Symbol, tree_node *);
typedef SymbolTable<Symbol, Entry> *SymTbl;
bool check_subtype(Symbol, Symbol, Class_);
bool is_SELF_TYPE(Symbol);
bool is_same_type(Symbol a, Symbol b);
method_node* find_method(Symbol , Symbol);

#define SEMANT_ERROR (semant_error(class_->get_filename(), this))

extern int semant_debug;

extern Symbol 
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

// define the class for phylum
// define simple phylum - Program
typedef class Program_class *Program;

class Program_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Program(); }
   virtual Program copy_Program() = 0;

#ifdef Program_EXTRAS
   Program_EXTRAS
#endif
};


// define simple phylum - Class_
typedef class Class__class *Class_;

class Class__class : public tree_node {
public:
   tree_node *copy()		 { return copy_Class_(); }
   virtual Class_ copy_Class_() = 0;

   // PA4
   virtual Symbol get_name() = 0;  
   virtual Symbol get_parent() = 0;
   virtual Features get_features() = 0;

#ifdef Class__EXTRAS
   Class__EXTRAS
#endif
};


// define simple phylum - Feature
typedef class Feature_class *Feature;

class Feature_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Feature(); }
   virtual Feature copy_Feature() = 0;

   // PA4
   virtual int get_type() = 0;
   virtual Formals get_formals() = 0;
   virtual Symbol get_name() = 0;
   virtual Symbol get_return_type() = 0;
   virtual Symbol get_type_decl() = 0;
   virtual bool type_check(Class_ class_, SymTbl obj_env) = 0;

#ifdef Feature_EXTRAS
   Feature_EXTRAS
#endif
};


// define simple phylum - Formal
typedef class Formal_class *Formal;

class Formal_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Formal(); }
   virtual Formal copy_Formal() = 0;

   // PA4
   virtual Symbol get_name() = 0;
   virtual Symbol get_type_decl() = 0;
   virtual void add_var(Class_ class_, SymTbl obj_env) = 0;

#ifdef Formal_EXTRAS
   Formal_EXTRAS
#endif
};


// define simple phylum - Expression
typedef class Expression_class *Expression;

class Expression_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Expression(); }
   virtual Expression copy_Expression() = 0;

   // PA4
   virtual bool is_no_expr(){ return false; }
   virtual bool type_check_and_set(Class_ class_, SymTbl obj_env) = 0;

#ifdef Expression_EXTRAS
   Expression_EXTRAS
#endif
};


// define simple phylum - Case
typedef class Case_class *Case;

class Case_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Case(); }
   virtual Case copy_Case() = 0;

   virtual Symbol get_name() = 0;
   virtual Symbol get_type_decl() = 0;
   virtual Expression get_expr() = 0;
   virtual bool type_check_and_set(Class_, SymTbl) = 0;

#ifdef Case_EXTRAS
   Case_EXTRAS
#endif
};


// define the class for phylum - LIST
// define list phlyum - Classes
typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;


// define list phlyum - Features
typedef list_node<Feature> Features_class;
typedef Features_class *Features;


// define list phlyum - Formals
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;


// define list phlyum - Expressions
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;


// define list phlyum - Cases
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;


// define the class for constructors
// define constructor - program
class program_class : public Program_class {
protected:
   Classes classes;
public:
   program_class(Classes a1) {
      classes = a1;
   }
   Program copy_Program();
   void dump(ostream& stream, int n);

#ifdef Program_SHARED_EXTRAS
   Program_SHARED_EXTRAS
#endif
#ifdef program_EXTRAS
   program_EXTRAS
#endif
};


// define constructor - class_
class class__class : public Class__class {
protected:
   Symbol name;
   Symbol parent;
   Features features;
   Symbol filename;
public:
   class__class(Symbol a1, Symbol a2, Features a3, Symbol a4) {
      name = a1;
      parent = a2;
      features = a3;
      filename = a4;
   }
   Class_ copy_Class_();
   void dump(ostream& stream, int n);

   // PA4
   Symbol get_name() { return name; } 
   Symbol get_parent() { return parent; }
   Features get_features() { return features; }

#ifdef Class__SHARED_EXTRAS
   Class__SHARED_EXTRAS
#endif
#ifdef class__EXTRAS
   class__EXTRAS
#endif
};


// define constructor - method
class method_class : public Feature_class {
protected:
   Symbol name;
   Formals formals;
   Symbol return_type;
   Expression expr;
public:
   method_class(Symbol a1, Formals a2, Symbol a3, Expression a4) {
      name = a1;
      formals = a2;
      return_type = a3;
      expr = a4;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);

   // PA4
   int get_type(){ return TYPE_METHOD; }
   Formals get_formals(){ return formals; };
   Symbol get_name(){ return name; }
   Symbol get_return_type(){ return return_type; }
   Symbol get_type_decl(){ return NULL; }
   bool type_check(Class_ class_, SymTbl obj_env){
      obj_env->enterscope();

      for (int i = formals->first();
         formals->more(i); 
         i = formals->next(i)){
         formals->nth(i)->add_var(class_, obj_env);
      }

      expr->type_check_and_set(class_, obj_env);

      bool flag = check_subtype(expr->get_type(), return_type, class_);
      if (!flag){
         SEMANT_ERROR << "expression type: " << expr->get_type()->get_string() <<
            " is not subtype of return type: " << return_type << endl;
      }

      obj_env->exitscope();

      return flag;
   }

#ifdef Feature_SHARED_EXTRAS
   Feature_SHARED_EXTRAS
#endif
#ifdef method_EXTRAS
   method_EXTRAS
#endif
};


// define constructor - attr
class attr_class : public Feature_class {
protected:
   Symbol name;
   Symbol type_decl;
   Expression init;
public:
   attr_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      init = a3;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);
   
   // PA4
   int get_type(){ return TYPE_ATTR; }
   Formals get_formals(){ return NULL; }
   Symbol get_name(){ return name; }
   Symbol get_return_type(){ return NULL; }
   Symbol get_type_decl(){ return type_decl; }
   bool type_check(Class_ class_, SymTbl obj_env){
      // [self/SELF_TYPE] has been added in semant.cc

      if (init->is_no_expr()){
         return true;
      }

      // `type_decl` is checked when constructing `obj_env`

      init->type_check_and_set(class_, obj_env);
      
      Symbol init_type = init->get_type();

      bool check_ret = check_subtype(init_type, type_decl, class_);
      if (!check_ret){
         SEMANT_ERROR << "expression type: " << init->get_type()->get_string() <<
            " is not subtype of declared type: " << type_decl << endl;
      }
      return check_ret;  
   }

#ifdef Feature_SHARED_EXTRAS
   Feature_SHARED_EXTRAS
#endif
#ifdef attr_EXTRAS
   attr_EXTRAS
#endif
};


// define constructor - formal
class formal_class : public Formal_class {
protected:
   Symbol name;
   Symbol type_decl;
public:
   formal_class(Symbol a1, Symbol a2) {
      name = a1;
      type_decl = a2;
   }
   Formal copy_Formal();
   void dump(ostream& stream, int n);

   // PA4
   Symbol get_name(){ return name; }
   Symbol get_type_decl(){ return type_decl; }
   void add_var(Class_ class_, SymTbl obj_env){
      if (obj_env->probe(name)){
         SEMANT_ERROR << "arg " << name->get_string() << " is redefined" << endl;
         return;
      }

      if (is_same_type(type_decl, SELF_TYPE)){
         SEMANT_ERROR << "Method args should not use SELF_TYPE" << endl;
         obj_env->addid(name, Object);
      } else {
         if (find_symbol(type_decl) < 0){
            SEMANT_ERROR << "Unknown type: " << type_decl->get_string() << endl;
            obj_env->addid(name, Object);
         } else {
            obj_env->addid(name, type_decl);  
         }
      }
   }

#ifdef Formal_SHARED_EXTRAS
   Formal_SHARED_EXTRAS
#endif
#ifdef formal_EXTRAS
   formal_EXTRAS
#endif
};


// define constructor - branch
class branch_class : public Case_class {
protected:
   Symbol name;
   Symbol type_decl;
   Expression expr;
public:
   branch_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      expr = a3;
   }
   Case copy_Case();
   void dump(ostream& stream, int n);

   Symbol get_name(){
      return name;
   }

   Symbol get_type_decl(){
      return type_decl;
   }

   Expression get_expr(){
      return expr;
   }

   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      obj_env->enterscope();

      if (is_same_type(type_decl, SELF_TYPE)){
         SEMANT_ERROR << "Can not use SELF_TYPE in branch" << endl;
         obj_env->addid(name, Object);
      } else {
         if (!(find_symbol(type_decl) < 0)){
            SEMANT_ERROR << "Unknown declared type: " 
               << type_decl->get_string() << endl;
            obj_env->addid(name, Object);
         } else {
            obj_env->addid(name, type_decl);
         }
      }

      expr->type_check_and_set(class_, obj_env);
      obj_env->exitscope();

      return true;
   }

#ifdef Case_SHARED_EXTRAS
   Case_SHARED_EXTRAS
#endif
#ifdef branch_EXTRAS
   branch_EXTRAS
#endif
};


// define constructor - assign
class assign_class : public Expression_class {
protected:
   Symbol name;
   Expression expr;
public:
   assign_class(Symbol a1, Expression a2) {
      name = a1;
      expr = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      expr->type_check_and_set(class_, obj_env);

      if (is_same_type(name, self)){
         SEMANT_ERROR << "Can not assign to self" << endl;
      }

      Symbol type = obj_env->lookup(name);
      if (!type){
         SEMANT_ERROR << "Unknown object: " << name->get_string() << endl;
         set_type(Object);
         return false;
      } else {
         if (!check_subtype(expr->get_type(), type, class_)){
            SEMANT_ERROR << "expression type: " << expr->get_type()->get_string() <<
               " is not subtype of declared type: " << type->get_string() << endl;
            set_type(Object);
            return false;
         } else {
            set_type(type);
            return true;
         }
      }
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef assign_EXTRAS
   assign_EXTRAS
#endif
};


// define constructor - static_dispatch
class static_dispatch_class : public Expression_class {
protected:
   Expression expr;
   Symbol type_name;
   Symbol name;
   Expressions actual;
public:
   static_dispatch_class(Expression a1, Symbol a2, Symbol a3, Expressions a4) {
      expr = a1;
      type_name = a2;
      name = a3;
      actual = a4;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      // initialize subexpressions
      expr->type_check_and_set(class_, obj_env);
      for (int expr_index = actual->first();
         actual->more(expr_index);
         expr_index = actual->next(expr_index)){
         actual->nth(expr_index)->type_check_and_set(class_, obj_env);
      }

      if (is_same_type(type_name, SELF_TYPE)){
         SEMANT_ERROR << "static dispatch should not use SELF_TYPE" << endl;
         set_type(Object);
         return false;
      }

      if (find_symbol(type_name) < 0){
         SEMANT_ERROR << "Unknown dispatch type: " << type_name->get_string() << endl;
         set_type(Object);
         return false; 
      }

      if (!check_subtype(expr->get_type(), type_name, class_)){
         SEMANT_ERROR << "expression type: " << expr->get_type()->get_string() <<
            " is not subtype of static dispatch type: " << type_name->get_string() << endl;
         set_type(Object);
         return false;
      }

      // find that method
      method_node* method_ptr = find_method(name, type_name);

      // check entire method
      if (method_ptr == NULL){
         SEMANT_ERROR << "Unknwon method: " << name->get_string() << endl; 
         set_type(Object);
         return false;
      }

      if (method_ptr->formal_count != actual->len()){
         SEMANT_ERROR << "In method " << name->get_string() <<
            " for type " << class_->get_name() <<
            " expected " << method_ptr->formal_count <<
            " args, but get " << actual->len() << " args" << endl;
         set_type(Object);
         return false;
      }

      // check each arg
      bool flag = true;
      for (int expr_index = actual->first();
         actual->more(expr_index);
         expr_index = actual->next(expr_index)){
         if (!check_subtype(
               actual->nth(expr_index)->get_type(),
               method_ptr->formals[expr_index].type_decl, 
               class_))
         {
            flag = false;
            SEMANT_ERROR << "Method " << name->get_string() <<
               " 's [" << expr_index << "] arg" << 
               " the current type " << actual->nth(expr_index)->get_type()->get_string() <<
               " is not subtype of " << method_ptr->formals[expr_index].type_decl->get_string() << endl;
            set_type(Object);
         }
      }

      if (flag){
         if (is_same_type(method_ptr->return_type, SELF_TYPE)){
            set_type(expr->get_type());
         } else {
            set_type(method_ptr->return_type);
         }
         return true;
      }      

      return false;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef static_dispatch_EXTRAS
   static_dispatch_EXTRAS
#endif
};


// define constructor - dispatch
class dispatch_class : public Expression_class {
protected:
   Expression expr;
   Symbol name;
   Expressions actual;
public:
   dispatch_class(Expression a1, Symbol a2, Expressions a3) {
      expr = a1;
      name = a2;
      actual = a3;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      // initialize subexpressions
      expr->type_check_and_set(class_, obj_env);
      for (int expr_index = actual->first();
         actual->more(expr_index);
         expr_index = actual->next(expr_index)){
         actual->nth(expr_index)->type_check_and_set(class_, obj_env);
      }

      // find that method
      method_node* method_ptr;
      if (is_same_type(expr->get_type(), SELF_TYPE)){
         method_ptr = find_method(name, class_->get_name());
      } else {
         method_ptr = find_method(name, expr->get_type());
      }

      // check entire method
      if (method_ptr == NULL){
         SEMANT_ERROR << "Unknwon method: " << name->get_string() << endl; 
         set_type(Object);
         return false;
      }

      if (method_ptr->formal_count != actual->len()){
         SEMANT_ERROR << "In method " << name->get_string() <<
            " for type " << class_->get_name() <<
            " expected " << method_ptr->formal_count <<
            " args, but get " << actual->len() << " args" << endl;
         set_type(Object);
         return false;
      }

      // check each arg
      bool flag = true;
      for (int expr_index = actual->first();
         actual->more(expr_index);
         expr_index = actual->next(expr_index)){
         if (!check_subtype(
               actual->nth(expr_index)->get_type(),
               method_ptr->formals[expr_index].type_decl, 
               class_)
         ){
            flag = false;
            SEMANT_ERROR << "Method " << name->get_string() <<
               " 's [" << expr_index << "] arg" << 
               " the current type " << actual->nth(expr_index)->get_type()->get_string() <<
               " is not subtype of " << method_ptr->formals[expr_index].type_decl << endl;
            set_type(Object);
         }
      }

      if (flag){
         if (is_same_type(method_ptr->return_type, SELF_TYPE)){
            set_type(expr->get_type());
         } else {
            set_type(method_ptr->return_type);
         }
         return true;
      }      

      return false;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef dispatch_EXTRAS
   dispatch_EXTRAS
#endif
};


// define constructor - cond
class cond_class : public Expression_class {
protected:
   Expression pred;
   Expression then_exp;
   Expression else_exp;
public:
   cond_class(Expression a1, Expression a2, Expression a3) {
      pred = a1;
      then_exp = a2;
      else_exp = a3;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      pred->type_check_and_set(class_, obj_env);
      then_exp->type_check_and_set(class_, obj_env);
      else_exp->type_check_and_set(class_, obj_env);

      if (!is_same_type(pred->get_type(), Bool)){
         SEMANT_ERROR << "Expect Bool for `If` pred" << endl;
      }
      set_type(LCA(then_exp->get_type(), else_exp->get_type()));

      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef cond_EXTRAS
   cond_EXTRAS
#endif
};


// define constructor - loop
class loop_class : public Expression_class {
protected:
   Expression pred;
   Expression body;
public:
   loop_class(Expression a1, Expression a2) {
      pred = a1;
      body = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      pred->type_check_and_set(class_, obj_env);
      body->type_check_and_set(class_, obj_env);

      if (!is_same_type(pred->get_type(), Bool)){
         SEMANT_ERROR << "Expect Bool for `Loop` pred" << endl;
         set_type(Object);
         return false;
      } else {
         set_type(Object);
         return true;
      }
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef loop_EXTRAS
   loop_EXTRAS
#endif
};


// define constructor - typcase
class typcase_class : public Expression_class {
protected:
   Expression expr;
   Cases cases;
public:
   typcase_class(Expression a1, Cases a2) {
      expr = a1;
      cases = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      expr->type_check_and_set(class_, obj_env);

      for (int i = cases->first(); 
         cases->more(i);
         i = cases->next(i)){
         cases->nth(i)->type_check_and_set(class_, obj_env);

         for (int j = cases->first(); 
            j < i;
            j = cases->next(j)){
            if (is_same_type(cases->nth(j)->get_type_decl(), cases->nth(i)->get_type_decl())){
               SEMANT_ERROR << cases->nth(i)->get_type_decl()->get_string() <<  
                  " is dulicatedly appeared" << endl;
            }
         }
      }

      Symbol final_type;
      if (cases->len() == 1){
         final_type = cases->nth(cases->first())->get_expr()->get_type();
      } else {
         int i = cases->first();
         Symbol first_type = cases->nth(i)->get_expr()->get_type();
         i = cases->next(i);
         Symbol second_type = cases->nth(i)->get_expr()->get_type();
         i = cases->next(i);

         final_type = LCA(first_type, second_type);

         for (; cases->more(i); i = cases->next(i)){
            Symbol this_type = cases->nth(i)->get_expr()->get_type();
            final_type = LCA(final_type, this_type);
         }
      }

      set_type(final_type);

      return true;
   }
#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef typcase_EXTRAS
   typcase_EXTRAS
#endif
};


// define constructor - block
class block_class : public Expression_class {
protected:
   Expressions body;
public:
   block_class(Expressions a1) {
      body = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      for (int i = body->first();
         body->more(i);
         i = body->next(i)){

         body->nth(i)->type_check_and_set(class_, obj_env);
      }

      int length = body->len();
      set_type(body->nth(length - 1)->get_type());
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef block_EXTRAS
   block_EXTRAS
#endif
};


// define constructor - let
class let_class : public Expression_class {
protected:
   Symbol identifier;
   Symbol type_decl;
   Expression init;
   Expression body;
public:
   let_class(Symbol a1, Symbol a2, Expression a3, Expression a4) {
      identifier = a1;
      type_decl = a2;
      init = a3;
      body = a4;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      if (!init->is_no_expr()){
         init->type_check_and_set(class_, obj_env);
      }
      
      obj_env->enterscope();

      // Update obj_env
      // Can use `SELF_TYPE` in let, so only consider unknown
      if (find_symbol(type_decl) < 0 && 
         !is_same_type(type_decl, SELF_TYPE)){
         SEMANT_ERROR << "Unknown declared type: " 
            << type_decl->get_string() << endl;
         obj_env->addid(identifier, Object);
      } 
      // Check if `init` return a subtype of `type_decl`
      else {
         if (!init->is_no_expr() && 
            !check_subtype(init->get_type(), type_decl, class_)){
            SEMANT_ERROR << "expression type: " << init->get_type()->get_string() <<
               " is not subtype of declared type: " << type_decl->get_string() << endl;
            obj_env->addid(identifier, Object);
         } else {
            obj_env->addid(identifier, type_decl);
         }
      }

      body->type_check_and_set(class_, obj_env);
      obj_env->exitscope();

      set_type(body->get_type());
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef let_EXTRAS
   let_EXTRAS
#endif
};


// define constructor - plus
class plus_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   plus_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      e1->type_check_and_set(class_, obj_env);
      e2->type_check_and_set(class_, obj_env);

      if (!is_same_type(e1->get_type(), Int) ||
            !is_same_type(e2->get_type(), Int)){
         set_type(Object);
         SEMANT_ERROR << "Expect two Ints for `+`" << endl;
         return false;
      }

      set_type(Int);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef plus_EXTRAS
   plus_EXTRAS
#endif
};


// define constructor - sub
class sub_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   sub_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      e1->type_check_and_set(class_, obj_env);
      e2->type_check_and_set(class_, obj_env);

      if (!is_same_type(e1->get_type(), Int) ||
            !is_same_type(e2->get_type(), Int)){
         set_type(Object);
         SEMANT_ERROR << "Expect two Ints for `-`" << endl;
         return false;
      }

      set_type(Int);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef sub_EXTRAS
   sub_EXTRAS
#endif
};


// define constructor - mul
class mul_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   mul_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      e1->type_check_and_set(class_, obj_env);
      e2->type_check_and_set(class_, obj_env);

      if (!is_same_type(e1->get_type(), Int) ||
            !is_same_type(e2->get_type(), Int)){
         set_type(Object);
         SEMANT_ERROR << "Expect two Ints for `*`" << endl;
         return false;
      }

      set_type(Int);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef mul_EXTRAS
   mul_EXTRAS
#endif
};


// define constructor - divide
class divide_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   divide_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      e1->type_check_and_set(class_, obj_env);
      e2->type_check_and_set(class_, obj_env);

      if (!is_same_type(e1->get_type(), Int) ||
            !is_same_type(e2->get_type(), Int)){
         set_type(Object);
         SEMANT_ERROR << "Expect two Ints for `/`" << endl;
         return false;
      }

      set_type(Int);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef divide_EXTRAS
   divide_EXTRAS
#endif
};


// define constructor - neg
class neg_class : public Expression_class {
protected:
   Expression e1;
public:
   neg_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      e1->type_check_and_set(class_, obj_env);

      if (!is_same_type(e1->get_type(), Int)){
         set_type(Object);
         SEMANT_ERROR << "Expect type Int for `~`" << endl;
         return false;
      }

      set_type(Int);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef neg_EXTRAS
   neg_EXTRAS
#endif
};


// define constructor - lt
class lt_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   lt_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      e1->type_check_and_set(class_, obj_env);
      e2->type_check_and_set(class_, obj_env);

      if (!is_same_type(e1->get_type(), Int) || 
            !is_same_type(e2->get_type(), Int)){
         set_type(Object);
         SEMANT_ERROR << "Expect Int for `<`" << endl;
         return false;
      }

      set_type(Bool);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef lt_EXTRAS
   lt_EXTRAS
#endif
};


// define constructor - eq
class eq_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   eq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      e1->type_check_and_set(class_, obj_env);
      e2->type_check_and_set(class_, obj_env);

      if (is_same_type(e1->get_type(), Int) ||
            is_same_type(e1->get_type(), Str) || 
            is_same_type(e1->get_type(), Bool) || 
            is_same_type(e2->get_type(), Int) || 
            is_same_type(e2->get_type(), Bool) ||
            is_same_type(e2->get_type(), Str)){
         if (!is_same_type(e1->get_type(), e2->get_type())){
            set_type(Object);
            SEMANT_ERROR << "Expect same type for `=` if one type is Int, Bool or Str" << endl;
            return false;
         }
      }

      set_type(Bool);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef eq_EXTRAS
   eq_EXTRAS
#endif
};


// define constructor - leq
class leq_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   leq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      e1->type_check_and_set(class_, obj_env);
      e2->type_check_and_set(class_, obj_env);

      if (!is_same_type(e1->get_type(), Int) || 
            !is_same_type(e2->get_type(), Int)){
         set_type(Object);
         SEMANT_ERROR << "Expect Int for `<=`" << endl;
         return false;
      }

      set_type(Bool);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef leq_EXTRAS
   leq_EXTRAS
#endif
};


// define constructor - comp
class comp_class : public Expression_class {
protected:
   Expression e1;
public:
   comp_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      e1->type_check_and_set(class_, obj_env);

      if (is_same_type(e1->get_type(), Bool)){
         set_type(Bool);
         return true;
      } else {
         set_type(Object);
         return false;
      }
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef comp_EXTRAS
   comp_EXTRAS
#endif
};


// define constructor - int_const
class int_const_class : public Expression_class {
protected:
   Symbol token;
public:
   int_const_class(Symbol a1) {
      token = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      set_type(Int);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef int_const_EXTRAS
   int_const_EXTRAS
#endif
};


// define constructor - bool_const
class bool_const_class : public Expression_class {
protected:
   Boolean val;
public:
   bool_const_class(Boolean a1) {
      val = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      set_type(Bool);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef bool_const_EXTRAS
   bool_const_EXTRAS
#endif
};


// define constructor - string_const
class string_const_class : public Expression_class {
protected:
   Symbol token;
public:
   string_const_class(Symbol a1) {
      token = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      set_type(Str);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef string_const_EXTRAS
   string_const_EXTRAS
#endif
};


// define constructor - new_
class new__class : public Expression_class {
protected:
   Symbol type_name;
public:
   new__class(Symbol a1) {
      type_name = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      if (is_SELF_TYPE(type_name)){
         set_type(SELF_TYPE);
         return true;
      }
      if (find_symbol(type_name) < 0){
         SEMANT_ERROR << "Unknwon Type: " << type_name->get_string() << endl;
         set_type(Object);
         return false;
      } else {
         set_type(type_name);
         return true;
      }
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef new__EXTRAS
   new__EXTRAS
#endif
};


// define constructor - isvoid
class isvoid_class : public Expression_class {
protected:
   Expression e1;
public:
   isvoid_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      e1->type_check_and_set(class_, obj_env);
      set_type(Bool);
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef isvoid_EXTRAS
   isvoid_EXTRAS
#endif
};


// define constructor - no_expr
class no_expr_class : public Expression_class {
protected:
public:
   no_expr_class() {
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   bool is_no_expr() { return true; }
   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      return true;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef no_expr_EXTRAS
   no_expr_EXTRAS
#endif
};


// define constructor - object
class object_class : public Expression_class {
protected:
   Symbol name;
public:
   object_class(Symbol a1) {
      name = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   bool type_check_and_set(Class_ class_, SymTbl obj_env){
      Symbol type = obj_env->lookup(name);
      if (!type){
         SEMANT_ERROR << "Unknown object: " << name->get_string() << endl;
         set_type(Object);
         return false;
      } else {
         set_type(type);
         return true;
      }
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef object_EXTRAS
   object_EXTRAS
#endif
};


// define the prototypes of the interface
Classes nil_Classes();
Classes single_Classes(Class_);
Classes append_Classes(Classes, Classes);
Features nil_Features();
Features single_Features(Feature);
Features append_Features(Features, Features);
Formals nil_Formals();
Formals single_Formals(Formal);
Formals append_Formals(Formals, Formals);
Expressions nil_Expressions();
Expressions single_Expressions(Expression);
Expressions append_Expressions(Expressions, Expressions);
Cases nil_Cases();
Cases single_Cases(Case);
Cases append_Cases(Cases, Cases);
Program program(Classes);
Class_ class_(Symbol, Symbol, Features, Symbol);
Feature method(Symbol, Formals, Symbol, Expression);
Feature attr(Symbol, Symbol, Expression);
Formal formal(Symbol, Symbol);
Case branch(Symbol, Symbol, Expression);
Expression assign(Symbol, Expression);
Expression static_dispatch(Expression, Symbol, Symbol, Expressions);
Expression dispatch(Expression, Symbol, Expressions);
Expression cond(Expression, Expression, Expression);
Expression loop(Expression, Expression);
Expression typcase(Expression, Cases);
Expression block(Expressions);
Expression let(Symbol, Symbol, Expression, Expression);
Expression plus(Expression, Expression);
Expression sub(Expression, Expression);
Expression mul(Expression, Expression);
Expression divide(Expression, Expression);
Expression neg(Expression);
Expression lt(Expression, Expression);
Expression eq(Expression, Expression);
Expression leq(Expression, Expression);
Expression comp(Expression);
Expression int_const(Symbol);
Expression bool_const(Boolean);
Expression string_const(Symbol);
Expression new_(Symbol);
Expression isvoid(Expression);
Expression no_expr();
Expression object(Symbol);


#endif
