#pragma once
#ifndef UniformTools_h__
#define UniformTools_h__

#include <boost/preprocessor.hpp>


#define __UNIFORM_SEQ_TUPLE(seq) BOOST_PP_CAT(__UNIFORM_SEQ_TUPLE_X seq, 0)
#define __UNIFORM_SEQ_TUPLE_X(x, y) ((x, y)) __UNIFORM_SEQ_TUPLE_Y
#define __UNIFORM_SEQ_TUPLE_Y(x, y) ((x, y)) __UNIFORM_SEQ_TUPLE_X
#define __UNIFORM_SEQ_TUPLE_X0
#define __UNIFORM_SEQ_TUPLE_Y0

#define __UNIFORM_DECL_UNIFORM(r, data, elem) \
    BOOST_PP_TUPLE_ELEM(2, 0, elem) __UNIFORM_GLUE(_, BOOST_PP_TUPLE_ELEM(2, 1, elem));

#define __UNIFORM_DECL_ADD_NAME(r, data, index, elem) \
    mUniforms.push_back({index, __UNIFORM_MAKE_STR(BOOST_PP_TUPLE_ELEM(2, 1, elem))});

#define __UNIFORM_DECL_SET(r, data, index, elem) \
    SetUniform(__UNIFORM_GLUE(_, BOOST_PP_TUPLE_ELEM(2, 1, elem)), mUniforms[index].location);

#define __UNIFORM_DECL_SETTER(r, data, elem) \
  void BOOST_PP_TUPLE_ELEM(2, 1, elem)(const BOOST_PP_TUPLE_ELEM(2, 0, elem) &value){ __UNIFORM_GLUE(_, BOOST_PP_TUPLE_ELEM(2, 1, elem)) = value; }

#define __UNIFORM_DECL_GETTER(r, data, elem) \
  const BOOST_PP_TUPLE_ELEM(2, 0, elem) &BOOST_PP_TUPLE_ELEM(2, 1, elem)() const { return __UNIFORM_GLUE(_, BOOST_PP_TUPLE_ELEM(2, 1, elem)); } 


#define __UNIFORM_MAKE_STR(x) __UNIFORM_MAKE_STR_1(x)
#define __UNIFORM_MAKE_STR_1(x) #x

#define __UNIFORM_GLUE(x, y) __UNIFORM_GLUE_1(x, y)
#define __UNIFORM_GLUE_1(x, y) x##y


//Uniform class 

#define UNIFORM(val) \
\
class BOOST_PP_SEQ_HEAD(val) : public UniformBase\
{\
public:\
  BOOST_PP_SEQ_HEAD(val)()\
  {\
    BOOST_PP_SEQ_FOR_EACH_I(__UNIFORM_DECL_ADD_NAME, ~, __UNIFORM_SEQ_TUPLE(BOOST_PP_SEQ_TAIL(val)))\
  }\
\
  void Bind()\
  {\
    BOOST_PP_SEQ_FOR_EACH_I(__UNIFORM_DECL_SET, ~, __UNIFORM_SEQ_TUPLE(BOOST_PP_SEQ_TAIL(val)))\
  }\
\
  BOOST_PP_SEQ_FOR_EACH(__UNIFORM_DECL_SETTER, ~, __UNIFORM_SEQ_TUPLE(BOOST_PP_SEQ_TAIL(val)))\
  BOOST_PP_SEQ_FOR_EACH(__UNIFORM_DECL_GETTER, ~, __UNIFORM_SEQ_TUPLE(BOOST_PP_SEQ_TAIL(val)))\
\
private:\
  BOOST_PP_SEQ_FOR_EACH(__UNIFORM_DECL_UNIFORM, ~, __UNIFORM_SEQ_TUPLE(BOOST_PP_SEQ_TAIL(val)))\
};


// example:
// class UniformTest : public UniformBase
// {
// public:
//   UniformTest()
//   {
//     mUniforms.push_back({ -1, "texture" });
//   }
// 
//   void Bind()
//   {
//     SetUniform(_texture, mUniforms[0].location);
//   }
// 
//   void texture(int value) { _texture = value; }
//   const int &texture() const { return _texture; }
// 
// private:
//   int _texture;
// };

#endif // UniformTools_h__
