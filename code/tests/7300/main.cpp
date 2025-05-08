#include<cstdio>
#include<utility>
#include<cstdlib>
#include<type_traits>
#include<array>
#include<tuple>
/**
 * 写得死烂，又长又慢。
 * Author:qL
 * todo：
 * Better modInt
 * frac
 * More Poly
 * fix bug of radix_sort
 * new IO
 * turn std::enable_if into static_assert 
*/
namespace QL{
	/**
	 * 图方便用的
	*/
	namespace{
		using ll=long long;
		using ull=unsigned long long;
		using uint=unsigned int;
		using db=double;
		using ld=long double;
#if _GLIBCXX_USE_INT128
		using lll=__int128;
		using ulll=unsigned __int128;
#else
		using lll=long long;
		using ulll=unsigned long long;
#endif
#if _GLIBCXX_NUMERIC&&(__cplusplus>=201703L)
		template<typename _Tp>
		constexpr _Tp Inf=std::numeric_limits<_Tp>::max()/2;
#else
		constexpr int Inf=0x3f3f3f3f;
		constexpr long long llInf=0x3f3f3f3f3f3f3f3f;
		constexpr double dbInf=1e17;
		constexpr long double ldInf=1e22;
#endif
#ifndef _GLIBCXX_CMATH
	#define sqrt __builtin_sqrt
	#define sqrtf __builtin_sqrtf
	#define sqrtl __builtin_sqrtl
	#define ceil __builtin_ceil
	#define ceilf __builtin_ceilf
	#define ceill __builtin_ceill
	#define floor __builtin_floor
	#define floorf __builtin_floorf
	#define floorl __builtin_floorl
	#define log2 __builtin_log2
	#define log __builtin_log
	#define cos __builtin_cos
	#define sin __builtin_sin
	#define tan __builtin_tan
	#define acos __builtin_acos
#endif
#ifndef _GLIBCXX_CSTRING
	#define memset __builtin_memset
	#define memcpy __builtin_memcpy
	#define strlen __builtin_strlen
	#define strcmp __builtin_strcmp
#endif
#ifndef _GLIBCXX_CSTDIO
	#define putchar __builtin_putchar
	#define fputc __builtin_fputc
#endif
#ifndef likely
	#define likely(x) __builtin_expect(!!(x),1)
#endif
#ifndef unlikely
	#define unlikely(x) __builtin_expect(!!(x),0)
#endif
	}
	/**
	 * 不想多加头文件了……
	*/
	namespace Error{
		constexpr void make_re(int _seed_=114514){
			std::exit(_seed_);
		}
#ifndef _GLIBCXX_CASSERT
		constexpr bool assert(bool x,const char *reason="Assert: RE"){
			if(unlikely(!x)){
				fputs(reason,stderr);
				fputs(reason,stdout);
				make_re();
			}
			return false;
		}
		constexpr bool assert(bool x,char *reason){
			return assert(x,const_cast<const char *>(reason));
		}
#endif
	}
}
namespace QL{
	/**
	 * 这坨代码最让人难以理解的地方：没逝乱靠元编程库
	*/
	namespace Traits_Tools{
		template<typename _Tp>
		class has_member_swap{
		private:
			template<typename T>
			static auto Check(void)->decltype(std::declval<T>().swap(),std::true_type());
			template<typename T>
			static std::false_type Check(...);
		public:
			enum{value=std::is_same<decltype(Check<_Tp>(nullptr)),std::true_type>::value};
		};
		#define Operator_Check_Helper(name,opt) \
				template<typename _Tp1,typename _Tp2> \
				class has_operator_##name{ \
				private: \
					template<typename T1,typename T2> \
					static auto Check(void)->decltype(std::declval<T1,T2>().operator opt (),std::true_type()); \
					template<typename T1,typename T2> \
					static std::false_type Check(...); \
				public: \
					enum{value=std::is_same<decltype(Check<_Tp1,_Tp2>(nullptr)),std::true_type>::value}; \
				};
		Operator_Check_Helper(plus,+)
		Operator_Check_Helper(subtract,-)
		Operator_Check_Helper(multiply,*)
		Operator_Check_Helper(divide,/)
		Operator_Check_Helper(mod,%)
		Operator_Check_Helper(and,&)
		Operator_Check_Helper(or,|)
		Operator_Check_Helper(xor,^)
		#undef Operator_Check_Helper
		template<typename _Tp,
		typename std::enable_if<!std::is_same<_Tp,bool>::value&&!std::is_same<_Tp,char>::value&&std::is_integral<_Tp>::value>::type* =nullptr>
		struct to_upper_type;
		#define To_Upper_Helper(type,upper) \
		template<> \
		struct to_upper_type< type >{ \
			using unsigned_type=upper; \
		}; \
		template<> \
		struct to_upper_type< u##type >{ \
			using unsigned_type=u##upper; \
		};
		To_Upper_Helper(int,ll)
		To_Upper_Helper(ll,lll)
		#undef To_Upper_Helper
	}
}
namespace QL{
	namespace rel_ops{
		namespace Calc_Self{
			#define Calc_Self_Helper(opt) \
			template<typename _Tp1,typename _Tp2> \
			constexpr _Tp1 &operator opt##=(_Tp1 &lhs,const _Tp2 &rhs){ \
				return lhs=(lhs opt rhs); \
			}
			Calc_Self_Helper(+)
			Calc_Self_Helper(-)
			Calc_Self_Helper(*)
			Calc_Self_Helper(/)
			Calc_Self_Helper(%)
			Calc_Self_Helper(&)
			Calc_Self_Helper(|)
			Calc_Self_Helper(^)
			#undef Calc_Self_Helper
		}
		namespace Compare{
			template<typename _Tp1,typename _Tp2>
			constexpr bool operator!=(const _Tp1 &lhs,const _Tp2 &rhs){
				return !(lhs==rhs);
			}
			template<typename _Tp1,typename _Tp2>
			constexpr bool operator<=(const _Tp1 &lhs,const _Tp2 &rhs){
				return (lhs==rhs)||(lhs<rhs);
			}
			template<typename _Tp1,typename _Tp2>
			constexpr bool operator>(const _Tp1 &lhs,const _Tp2 &rhs){
				return !((lhs==rhs)||(lhs<rhs));
			}
			template<typename _Tp1,typename _Tp2>
			constexpr bool operator>=(const _Tp1 &lhs,const _Tp2 &rhs){
				return !(lhs<rhs);
			}
		}
	}
}
namespace QL{
	namespace Base_Tools{
		template<typename _Tp>
		static constexpr std::size_t integer_length=sizeof(_Tp)*10/sizeof(int);
		bool is_space(char ch){
			return ch==' ';
		}
		bool is_eoln(char ch){
#if (linux||__linux__||__linux)
			return ch=='\n'||ch=='\r';
#else
			return ch=='\n';
#endif
		}
		bool is_blank(char ch){
			return is_space(ch)||is_eoln(ch);
		}
		bool is_digit(char ch){
			switch(ch){
				case '0' ... '9': return true;
				default: return false;
			}
		}
		bool is_eof(char ch){
			return ch==EOF;
		}
	}
	namespace IO{
		using Base_Tools::integer_length;
		using Base_Tools::is_digit;
		using Base_Tools::is_space;
		using Base_Tools::is_eoln;
		using Base_Tools::is_blank;
		using Base_Tools::is_eof;
		/**
		 * fread+fwrite,-DLOACL for debug
		 * support:integer,floating,string,...
		 * todo:other
 		*/
		class IOstream{
		protected:
			using LIST=std::initializer_list<int>;
#ifndef LOCAL
			std::FILE *IN;
			std::FILE *OUT;
			static constexpr int SIZE=1<<15;
			char buf[SIZE]{},*p1{buf},*p2{buf},obuf[SIZE]{},*p3{obuf};
		public:
			char pull(){return p1==p2&&(p2=(p1=buf)+fread(buf,1,SIZE,IN),p1==p2)?(Ch=EOF):*p1++;}
			void push(char ch){((p3-obuf)==SIZE&&(flush(false),0)),*p3++=ch;}
			template<std::size_t L>
			std::FILE *set_in(const char (&name)[L]){
				static char in[L+5]={};
				std::sprintf(in,"%s.in",name);
				return std::fopen(in,"r");
			}
			template<std::size_t L>
			std::FILE *set_out(const char (&name)[L]){
				static char out[L+5];
				std::sprintf(out,"%s.out",name);
				return std::fopen(out,"w");
			}
#else
		protected:
		public:
			char pull(){return std::getchar();}
			void push(char ch){putchar(ch);}
			void err(char ch){fputc(ch,stderr);}
			template<std::size_t L>
			void set_in(const char(&name)[L]){
				static char in[L+5]={};
				std::sprintf(in,"%s.in",name);
				std::freopen(in,"r",stdin);
			}
			template<std::size_t L>
			void set_out(const char(&name)[L]){
				static char out[L+5];
				std::sprintf(out,"%s.out",name);
				std::freopen(out,"w",stdout);
			}
#endif
		public:
#ifndef LOCAL
			IOstream():IN{stdin},OUT{stdout},buf{},p1{buf},p2{buf},obuf{},p3{obuf},Ch{'\n'},outchar{&IO::IOstream::push}{}
			template<std::size_t L>
			IOstream(const char(&name)[L]):IN{set_in(name)},OUT{set_out(name)},buf{},p1{buf},p2{buf},obuf{},p3{obuf},Ch{'\n'},outchar{&IO::IOstream::push}{}
			template<std::size_t L>
			IOstream(const char(&name)[L],bool in,bool out):IN{in?set_in(name):stdin},OUT{out?set_out(name):stdout},buf{},p1{buf},p2{buf},obuf{},p3{obuf},Ch{'\n'},outchar{&IO::IOstream::push}{}
			template<std::size_t L>
			IOstream(char(&name)[L]):IN{set_in(name)},OUT{set_out(name)},buf{},p1{buf},p2{buf},obuf{},p3{obuf},Ch{'\n'},outchar{&IO::IOstream::push}{}
			template<std::size_t L>
			IOstream(char(&name)[L],bool in,bool out):IN{in?set_in(name):stdin},OUT{out?set_out(name):stdout},buf{},p1{buf},p2{buf},obuf{},p3{obuf},Ch{'\n'},outchar{&IO::IOstream::push}{}
			~IOstream(){flush();}
			void flush(bool _flush_=false){
				if(likely(p3!=obuf))
					fwrite(obuf,1,p3-obuf,OUT),p3=obuf;
				if(_flush_) std::fflush(stdout);
			}
#else
			IOstream(){}
			template<std::size_t L>
			IOstream(const char(&name)[L]):Ch{'\n'}{reset_stdin(name),reset_stdout(name);}
			template<std::size_t L>
			IOstream(const char(&name)[L],bool in,bool out):Ch{'\n'}{in&&(reset_stdin(name),0),out&&(reset_stdout(name),0);}
			template<std::size_t L>
			IOstream(char(&name)[L]):Ch{'\n'}{reset_stdin(name),reset_stdout(name);}
			template<std::size_t L>
			IOstream(char(&name)[L],bool in,bool out):Ch{'\n'}{in&&(reset_stdin(name),0),out&&(reset_stdout(name),0);}
			void flush(){std::fflush(stdout);}
#endif
			template<typename T>
			T read(){
				T x;
				read(x);
				return x;
			}
protected:
			char Ch='\n';
public:
			bool eof(void)const{
				return Ch==EOF;
			}
			template<typename T>
			void read(T &&x,typename std::enable_if<std::is_integral<T>::value,void>::type* =nullptr,typename std::enable_if<std::is_signed<T>::value,void>::type* =nullptr){
				x=0;bool flag=0;
				for(;!is_digit(Ch)&&!is_eof(Ch);Ch=pull()) if(Ch=='-') flag=1;
				if(is_eof(Ch)) return;
				if(flag) for(;is_digit(Ch);Ch=pull()) x=x*10-(Ch&15);
				else for(;is_digit(Ch);Ch=pull()) x=x*10+(Ch&15);
			}
			template<typename T>
			void read(T &&x,typename std::enable_if<std::is_integral<T>::value,void>::type* =nullptr,typename std::enable_if<std::is_unsigned<T>::value,void>::type* =nullptr){
				x=0;
				for(;!is_digit(Ch)&&!is_eof(Ch);Ch=pull());
				if(is_eof(Ch)) return;
				for(;is_digit(Ch);Ch=pull()) x=x*10+(Ch&15);
			}
			void read(char *str){
				for(;is_blank(Ch);Ch=pull());
				if(is_eof(Ch)) return (void)(*str='\0');
				for(;!is_blank(Ch)&&!is_eof(Ch);Ch=pull()) *str++=Ch;
				*str='\0';
			}
			void read(char &c){
				c=Ch;
				for(;is_blank(c)&&!is_eof(c);c=pull());
				if(is_eof(c)) return;
				Ch=pull();
			}
			void read(bool &x){
				for(;Ch!='0'&&Ch!='1'&&!is_eof(Ch);Ch=pull());
				if(is_eof(Ch)) return void(x=false);
				x=Ch=='1';
				Ch=pull();
			}
			template<typename T>
			void read(T &&x,typename std::enable_if<std::is_floating_point<T>::value,void*>::type* =nullptr){
				static char str[114];
				read(str);
				x=std::atof(str);
			}
			template<typename T>
			void read(T &x){read(std::move(x));}
		protected:
			void(IOstream::*outchar)(char)=&IO::IOstream::push;
			template<typename T>
			void out(T x,typename std::enable_if<std::is_integral<T>::value,void>::type* =nullptr,typename std::enable_if<std::is_signed<T>::value,void>::type* =nullptr){
				static char sta[integer_length<T>];
				int top=0;
				if(x<0){
					(this->*outchar)('-');
					do sta[top++]=((-x)%10)|48,x/=10;
					while(x);
				}
				else{
					do sta[top++]=(x%10)|48,x/=10;
					while(x);
				}
				while(top) (this->*outchar)(sta[--top]);
			}
			template<typename T>
			void out(T x,typename std::enable_if<std::is_integral<T>::value,void>::type* =nullptr,typename std::enable_if<std::is_unsigned<T>::value,void>::type* =nullptr){
				static char sta[integer_length<T>];
				int top=0;
				do sta[top++]=(x%10)|48,x/=10;
				while(x);
				while(top) (this->*outchar)(sta[--top]);
			}
			void out(bool x){(this->*outchar)(x?'1':'0');}
			void out(char x){(this->*outchar)(x);}
			void out(char *str){
				out(reinterpret_cast<const char *>(str));
			}
			void out(const char *str){
				while(*str!='\0') (this->*outchar)(*str++);
			}
			/**
			 * ssprintf is awful...
			 */
			void out(float x){
				static char str[114];
				std::sprintf(str,"%f",x);
				out(str);
			}
			void out(double x){
				static char str[114];
				std::sprintf(str,"%f",x);
				out(str);
			}
			void out(long double x){
				static char str[114];
				std::sprintf(str,"%Lf",x);
				out(str);
			}
			void out(std::pair<int,float> x){
				static char str[114],opt[10];
				std::sprintf(opt,"%%.%df",x.first);
				std::sprintf(str,opt,x.second);
				out(str);
			}
			void out(std::pair<int,double> x){
				static char str[114],opt[10];
				std::sprintf(opt,"%%.%df",x.first);
				std::sprintf(str,opt,x.second);
				out(str);
			}
			void out(std::pair<int,long double> x){
				static char str[114],opt[10];
				std::sprintf(opt,"%%.%dLf",x.first);
				std::sprintf(str,opt,x.second);
				out(str);
			}
			void set_std_out(){outchar=&IO::IOstream::push;}
#ifdef LOCAL
			void set_err_out(){outchar=&IO::IOstream::err;}
#endif
		public:
			template<typename...Args>
			void read(Args &&...args){(void)LIST{(read(args),0)...};}
			template<typename...Args>
			void write(Args...args){set_std_out(),(void)LIST{(out(args),0)...};}
			template<typename...Args>
			void writeln(Args...args){write(args...),push('\n');}
#ifdef LOCAL
			template<typename...Args>
			void error(Args...args){set_err_out(),(void)LIST{(out(args),0)...};}
			template<typename...Args>
			void errorln(Args...args){error(args...),err('\n');}
#endif
		};
		IOstream lq;
	}
}
namespace QL{
	namespace Base_Tools{
		template<typename _Tp1,typename _Tp2>
		constexpr auto min(_Tp1 x,_Tp2 y){
			return x<y?x:y;
		}
		template<typename _Tp,typename ...Args>
		constexpr auto min(_Tp x,Args ...args){
			return min(x,min(args...));
		}
		template<typename _Tp1,typename _Tp2>
		constexpr auto max(_Tp1 x,_Tp2 y){
			return y<x?x:y;
		}
		template<typename _Tp,typename ...Args>
		constexpr auto max(_Tp x,Args ...args){
			return max(x,max(args...));
		}
		template<typename _Tp1,typename _Tp2>
		constexpr bool chkmin(_Tp1 &x,_Tp2 y){
			return y<x?(x=y,true):false;
		}
		template<typename _Tp1,typename _Tp2,typename...Args>
		constexpr bool chkmin(_Tp1 &x,_Tp2 y,Args ...args){
			return chkmin(x,y)|chkmin(x,args...);
		}
		template<typename _Tp1,typename _Tp2>
		constexpr bool chkmax(_Tp1 &x,_Tp2 y){
			return x<y?(x=y,true):false;
		}
		template<typename _Tp1,typename _Tp2,typename...Args>
		constexpr bool chkmax(_Tp1 &x,_Tp2 y,Args ...args){
			return chkmax(x,y)|chkmax(x,args...);
		}
		template<typename _Tp,
		typename std::enable_if<!Traits_Tools::has_member_swap<_Tp>::value&&!std::is_integral<_Tp>::value>::type* =nullptr>
		constexpr void swap(_Tp &x,_Tp &y){
			_Tp tmp;
			tmp=x,x=y,y=tmp;
		}
		template<typename _Tp,typename std::enable_if<std::is_integral<_Tp>::value>::type* =nullptr>
		constexpr void swap(_Tp &x,_Tp &y){
			x!=y&&(x^=y^=x^=y);
		}
		template<typename _Tp>
		constexpr void swap(_Tp *&x,_Tp *&y){
			_Tp *tmp;
			tmp=x,x=y,y=tmp;
		}
		template<typename _Tp,typename std::enable_if<Traits_Tools::has_member_swap<_Tp>::value>::type* =nullptr>
		constexpr void swap(_Tp &x,_Tp &y){
			x.swap(y);
		}
		template<typename _Tp>
		constexpr _Tp abs(const _Tp &x){
			return x<0?-x:x;
		}
	}
}
namespace MAIN{
	using namespace QL;
	using IO::lq;
	template<typename _Compare>
	class LinkCutTree{
		static _Compare _Comp;
	protected:
		struct Node{
			Node *ch[2],*fa;
			bool rev;
			int val;
			Node *ptr;
			Node():ch{},fa{},rev{},ptr{this}{}
		};
		Node *tr;
		int node;
		bool is_root(Node *p){return p->fa==nullptr||(p->fa->ch[0]!=p&&p->fa->ch[1]!=p);}
		bool chk_child(Node *p){return p==p->fa->ch[1];}
		void pull(Node *p){
			p->ptr=p;
			if(p->ch[0]&&_Comp(p->ch[0]->ptr->val,p->ptr->val)) p->ptr=p->ch[0]->ptr;
			if(p->ch[1]&&_Comp(p->ch[1]->ptr->val,p->ptr->val)) p->ptr=p->ch[1]->ptr;
		}
		void reverse(Node*p){
			Base_Tools::swap(p->ch[0],p->ch[1]);
			p->rev=!p->rev;
		}
		void push(Node *p){
			if(p->rev){
				if(p->ch[0]) reverse(p->ch[0]);
				if(p->ch[1]) reverse(p->ch[1]);
				p->rev=false;
			}
		}
		void pushall(Node *p){
			if(likely(!is_root(p))) pushall(p->fa);
			push(p);
		}
		void rotate(Node *p){
			Node*q=p->fa;
			bool chk=chk_child(p);
			q->ch[chk]=p->ch[!chk];
			if(q->ch[chk]) q->ch[chk]->fa=q;
			p->fa=q->fa;
			if(!is_root(q)) p->fa->ch[chk_child(q)]=p;
			p->ch[!chk]=q,q->fa=p;
			pull(q);
		}
		void splay(Node *p){
			pushall(p);
			for(;!is_root(p);rotate(p))
				if(!is_root(p->fa)) rotate(chk_child(p)==chk_child(p->fa)?p->fa:p);
			pull(p);
		}
		void access(Node *p){
			for(Node *t=p,*q=nullptr;t;q=t,t=t->fa)
				splay(t),t->ch[1]=q,pull(t);
			splay(p);
		}
		void make_root(Node *p){
			access(p),reverse(p);
		}
		Node *find_root(Node *p){
			access(p),push(p);
			while(p->ch[0]) p=p->ch[0],push(p);
			splay(p);
			return p;
		}
		void split(Node *p,Node *q){
			make_root(p),access(q);
		}
		bool unicom(Node *p,Node *q){
			make_root(p);
			return find_root(q)==p;
		}
		void fix(Node *p,int val){
			splay(p),p->val=val,pull(p);
		}
		void link(Node *p,Node *q){
			if(!unicom(p,q)) p->fa=q;
		}
		void cut(Node *p,Node *q){
			if(!unicom(p,q)||q->fa!=p||q->ch[0]) return;
			q->fa=p->ch[1]=nullptr;
		}
		Node *find(Node *p,Node *q){
			split(p,q);
			return q->ptr;
		}
		void check_node_valid(int x){Error::assert(0<=x&&x<=node,"Node is invalid in Link-Cut Tree");}
		template<typename ...Args>
		void check_node_valid(int x,Args ...args){check_node_valid(x),check_node_valid(args...);}
	public:
		void fix(int x,int val){check_node_valid(x),fix(&tr[x],val);}
		bool unicom(int x,int y){return check_node_valid(x,y),unicom(&tr[x],&tr[y]);}
		void link(int x,int y){check_node_valid(x,y),link(&tr[x],&tr[y]);}
		void cut(int x,int y){check_node_valid(x,y),cut(&tr[x],&tr[y]);}
		int find(int x,int y){return check_node_valid(x,y),find(&tr[x],&tr[y])-tr;}
		LinkCutTree():tr{},node{}{}
		LinkCutTree(int n):tr{new Node[n+1]{}},node{n}{}
		void clear(){this->~LinkCutTree();}
		void resize(int n){clear(),tr=new Node[(node=n)+1]{};}
		~LinkCutTree(){delete[] tr;tr=nullptr,node=0;}
	};
	template<typename _Compare>
	_Compare LinkCutTree<_Compare>::_Comp;
	template<typename _Tp,template<typename> class _Plus>
	class BIT{
	protected:
		static _Plus<_Tp> plus;
		_Tp *c;
		int len;
	public:
		BIT():c{},len{}{}
		BIT(int _len):c{new _Tp[_len+2]{}},len{_len+1}{}
		~BIT(){delete[] c;c=nullptr,len=0;}
		int length()const{return len-2;}
		void clear(){this->~BIT();}
		void resize(int _len){clear(),c=new _Tp[_len+2]{},len=_len+1;}
		void upd(int x,_Tp val){
			// lq.errorln(x,' ',val);
			Error::assert(0<=x&&x<len,"In BIT(class), upd is invalid.");
			for(++x;x<=len;x+=x&-x) c[x]=plus(c[x],val);
		}
		_Tp qry(int x){
			_Tp ret{};
			Error::assert(0<=x&&x<len,"In BIT(class), qry is invalid.");
			for(++x;x;x-=x&-x) ret=plus(ret,c[x]);
			return ret;
		}
	};
	template<typename _Tp,template<typename> class _Plus>
	_Plus<_Tp> BIT<_Tp,_Plus>::plus;
	struct ckmi{bool operator()(int x,int y){return x<y;}};
	struct ckma{bool operator()(int x,int y){return x>y;}};
	template<typename _Tp>
	struct plus{_Tp operator()(_Tp x,_Tp y){return x+y;}};
	constexpr int N=5e4+5,M=2e5+5;
	std::array<std::tuple<int,int,int>,M> edge;
	template<typename _Compare,typename _RevCompare>
	class Generate_Tree{
	private:
		_Compare _Comp;
		LinkCutTree<_RevCompare> lct;
		BIT<int,plus> bit;
		int node;
	public:
		void resize(int n,int m,int val){
			node=n,lct.resize(n+m),bit.resize(m);
			for(int i=1;i<=n;++i) lct.fix(i,val);
		}
		void add_edge(int x,int y,int w,int id){
			// if(std::is_same<ckma,_Compare>::value) lq.errorln("ma");
			// else lq.errorln("mi");
			if(lct.unicom(x,y)){
				int it=lct.find(x,y)-node;
				// lq.errorln(x,' ',y,' '," with ",' ',std::get<0>(edge[it]),' ',std::get<1>(edge[it]));
				// Error::assert(0<it&&it<id,"Valid It");
				if(_Comp(w,std::get<2>(edge[it]))){
					lct.cut(std::get<0>(edge[it]),it+node);
					lct.cut(std::get<1>(edge[it]),it+node);
					lct.fix(id+node,w),lct.link(x,id+node),lct.link(y,id+node);
					bit.upd(std::get<2>(edge[it]),-1),bit.upd(w,1);
				}
			}
			else lct.fix(id+node,w),lct.link(x,id+node),lct.link(y,id+node),bit.upd(w,1);
			// lq.errorln("End");
		}
		int bound(int x){return bit.qry(x);}
	};
	Generate_Tree<ckmi,ckma> tr_mi;
	Generate_Tree<ckma,ckmi> tr_ma;
	int n,m;
	template<int N>
	struct Dsu{
	private:
		std::array<int,N> pre;
		int count;
	public:
		void resize(int n){
			count=n;
			for(int i=1;i<=n;++i) pre[i]=i;
		}
		int get(int x){return pre[x]==x?x:pre[x]=get(pre[x]);}
		void merge(int x,int y){if(unlikely(get(x)!=get(y))) pre[get(y)]=get(x),--count;}
		bool all_unicom()const{return count==1;}
	};
	Dsu<N> dsu;
	std::array<bool,M> appeared;
	void solve(){
		lq.read(n,m);
		tr_mi.resize(n,m,-Inf),tr_ma.resize(n,m,+Inf);
		dsu.resize(n);
		std::fill_n(appeared.begin()+1,m,false);
		for(int id=0,opt,x,y,k,w;m--;){
			lq.read(opt);
			if(opt==1){
				lq.read(x,y,w);
				edge[++id]=std::make_tuple(x,y,w);
				appeared[w]=true;
				tr_mi.add_edge(x,y,w,id);
				tr_ma.add_edge(x,y,w,id);
				dsu.merge(x,y);
			}
			else{
				lq.read(k,w);
				int down=tr_ma.bound(w-1)+1,up=tr_mi.bound(w);
				// lq.writeln(down,' ',up,' ',k,' ',w);
				lq.writeln(dsu.all_unicom()&&appeared[w]&&(down<=k&&k<=up)?"YES":"NO");
				// lq.errorln(":::::::: ",k,' ',w);
			}
		}
	}
	void _main_(void){
		for(int Test=lq.read<int>();Test--;) solve();
		return;
	}
}
signed main(){
	return MAIN::_main_(),0;
}