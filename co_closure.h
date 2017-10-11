/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef __CO_CLOSURE_H__
#define __CO_CLOSURE_H__
struct stCoClosure_t
{
public:
	virtual void exec() = 0;
};

//1.base
//-- 1.1 comac_argc

#define comac_get_args_cnt( ... ) comac_arg_n( __VA_ARGS__ )
#define comac_arg_n( _0,_1,_2,_3,_4,_5,_6,_7,N,...) N
#define comac_args_seqs() 7,6,5,4,3,2,1,0
#define comac_join_1( x,y ) x##y

//计算可变参数...中参数的个数.注意由于seq的范围为7-0，该宏可识别的参数个数的范围为0-7个。超过7个参数识别有问题
#define comac_argc( ... ) comac_get_args_cnt( 0,##__VA_ARGS__,comac_args_seqs() )
//x,y连接为一个字符串
#define comac_join( x,y) comac_join_1( x,y )

//-- 1.2 repeat
#define repeat_0( fun,a,... )
#define repeat_1( fun,a,... ) fun( 1,a,__VA_ARGS__ ) repeat_0( fun,__VA_ARGS__ )
#define repeat_2( fun,a,... ) fun( 2,a,__VA_ARGS__ ) repeat_1( fun,__VA_ARGS__ )
#define repeat_3( fun,a,... ) fun( 3,a,__VA_ARGS__ ) repeat_2( fun,__VA_ARGS__ )
#define repeat_4( fun,a,... ) fun( 4,a,__VA_ARGS__ ) repeat_3( fun,__VA_ARGS__ )
#define repeat_5( fun,a,... ) fun( 5,a,__VA_ARGS__ ) repeat_4( fun,__VA_ARGS__ )
#define repeat_6( fun,a,... ) fun( 6,a,__VA_ARGS__ ) repeat_5( fun,__VA_ARGS__ )

#define repeat( n,fun,... ) comac_join( repeat_,n )( fun,__VA_ARGS__)

//2.implement
#if __cplusplus <= 199711L
#define decl_typeof( i,a,... ) typedef typeof( a ) typeof_##a;
#else
#define decl_typeof( i,a,... ) typedef decltype( a ) typeof_##a;
#endif
#define impl_typeof( i,a,... ) typeof_##a & a;
#define impl_typeof_cpy( i,a,... ) typeof_##a a;
#define con_param_typeof( i,a,... ) typeof_##a & a##r,
#define param_init_typeof( i,a,... ) a(a##r),


//2.1 reference
/*	example
co_ref(ref1, ref2);
宏定义展开之后:
typedef typeof(ref2) typeof_ref2;
class type_ref1 {
    public: typeof_ref2 & ref2;
    int _member_cnt;
    type_ref1(typeof_ref2 & ref2r, ...): ref2(ref2r),_member_cnt(1) {}
} ref1(ref2);;
从上面的宏展开中可以看出，假设该宏定义参数有三个分别为#1,#2,#3。
该宏定义的作用是:定义一个类（类名为type_#1），该类含有两个引用成员,#2,#3，以及一个记录成员个数的变量_member_cnt。
上述几个参数在初始化时指定。
定义类type_#1之后，同时使用指定的参数实例化了该类的实例为#1。
*/

#define co_ref( name,... )\
repeat( comac_argc(__VA_ARGS__) ,decl_typeof,__VA_ARGS__ )\
class type_##name\
{\
public:\
	repeat( comac_argc(__VA_ARGS__) ,impl_typeof,__VA_ARGS__ )\
	int _member_cnt;\
	type_##name( \
		repeat( comac_argc(__VA_ARGS__),con_param_typeof,__VA_ARGS__ ) ... ): \
		repeat( comac_argc(__VA_ARGS__),param_init_typeof,__VA_ARGS__ ) _member_cnt(comac_argc(__VA_ARGS__)) \
	{}\
} name( __VA_ARGS__ ) ;


//2.2 function
/*example
co_func(func1,func2){
  	printf("i: %d\n",i);
    for(int j=0;j<2;j++) {
        usleep( 1000 );
        printf("i %d j %d\n",i,j);
    }
	}
  co_func_end;

//宏定义展开之后:
typedef typeof(func2) typeof_func2;
class func1: public stCoClosure_t {
    public: typeof_func2 func2;
    int _member_cnt;
    public: func1(typeof_func2 & func2r, ...): func2(func2r),
    _member_cnt(1) {}
    void exec() {
        printf("i: %d\n", i);
        for (int j = 0; j < 2; j++) {
            usleep(1000);
            printf("i %d j %d\n", i, j);
        }
    }
};
该宏定义的作用:如co_func(#1,#2)
声明一个名字为#1的类，这个类继承于stCoClosure_t，并在中间实现了继承自stCoClosure_t的函数exec。
成员及其类型与co_ref类似。
*/
#define co_func(name,...)\
repeat( comac_argc(__VA_ARGS__) ,decl_typeof,__VA_ARGS__ )\
class name:public stCoClosure_t\
{\
public:\
	repeat( comac_argc(__VA_ARGS__) ,impl_typeof_cpy,__VA_ARGS__ )\
	int _member_cnt;\
public:\
	name( repeat( comac_argc(__VA_ARGS__),con_param_typeof,__VA_ARGS__ ) ... ): \
		repeat( comac_argc(__VA_ARGS__),param_init_typeof,__VA_ARGS__ ) _member_cnt(comac_argc(__VA_ARGS__))\
	{}\
	void exec()

#define co_func_end }


#endif
