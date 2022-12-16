---
published: true
tag:
  - cpp
---
# Tóm tắt Exceptional C Plus Plus
## Sử dụng iterator và thư viện
### Item 1:
sử dụng iterator trong algorithm đúng :
không nên sử dụng phép toán số học lên iterator khi không chắc chắn pointer đó out of range 
ví dụ 
```
std::vector<int> vi;
std::cout << vi.begin() + 1;

for(auto it = vi.begin(); vi != vi.end(); vi++){
	if(*it %2){
    	vi.erase(it);
    }
}
```
ví dụ 1 bug access an element out of the vector's range
ví dụ 2 khi xóa trong lúc duyệt chương trình sẽ không đảm bảo *vi* còn point tới vi nữa hay không ( có thể sau khi xóa vùng nhớ của vi sẽ thay đỏi và ta không chắc chắn được -> vi++ là undefine behavior)

### Item 6: tránh temporary object 
```
string FindAddr( list<Employee> emps, string name ) (1)
{
	for( list<Employee>::iterator i = emps.begin();i != emps.end();i++)(2)
    {
      if( *i == name ) (3)
      {
          return i->addr;
      }
    }
    return "";
}
```
theo đoạn code trên có các điểm làm lãng phí tài nguyên.  
(1). nên dùng const & thay vì copy giá trị  
(2). việc recalculate emps.end() là không cần thiết.  
(3). \*i nghĩa là từ kiểu Employee cần implement việc cast về string hoặc đã overload == operator -> tạo ra temporary string thay vào đó ta có thể implement 1 method compare cho lớp Employee ví dụ i.compare(name).

Để tránh gặp phải những đoạn code như trên ta nên sử dụng các function đã được define trong std::algorithm như std::find thay vì viết for loop như ví dụ trên, thư viện đã tối ưu và loại bỏ những nhược điểm không đáng có.

## Exception safety
Mô tả các rule, kinh nghiệm viết code an toàn. tránh side effect và những lỗi tiềm ẩn với exception.
một (đoạn) chương trình được đảm bảo exception safety theo các tiêu chí sau:
+ Đảm bảo đoạn chương tình đó sử dụng, giải phóng tài nguyên đúng đắn ngay cả khi exception xảy ra vd (std::bad_alloc, std::runtime,..).
+ Đảm bảo tài nguyên được an toàn ví dụ khi đang copy 1 array 1-> array 2 và exception xảy ra thì cần đảm bảo array2 phải recover được trạng thái trước copy, chỉ được phép copy khi không có bất kì exception nào xảy ra trong quá trình copy  

```
vector<int> arr1, arr2;
copy(arr1.begin(), arr1.end(), arr2.begin()); 
// exception safety by library cho các kiểu dữ kiểu nguyên bản của c++ (int, char, long,...).
Với class khi ta sử dụng copy như trên đoạn code đó sẽ không đảm bảo exception safety.

```
+ Đảm bảo phía client sẽ không phải handle exceptions vì internal function được gọi đã handle ví dụ hàm đó chỉ trả về true or false thay vì exceptions.
### Item 13 ví dụ về exception guarantee
+ copy and swap idiom. nhằm tránh việc duplicate code với copy constructor, không cần quan tâm việc delete với point cũ trong this pointer( đã automatically calling the destructor cho tmp obj) và đồng thời đảm bảo exception safety vì dòng 2,3 nào can't throw. Chỉ duy nhất việc copy vào *other* nếu failed thì sẽ xảy ra exception và phía client cần handle điều đó.
```
    Derived & operator = (Derived other) {
        swap(this->p , other.p);
        return *this;
    }
```
khi so sánh với các làm thông thường bên dưới.
```
  Derived & operator = (const Derived &other) {
    if( this != &other )
    {
      T* v_new = NewCopy( other.v_,
      other.vsize_,
      other.vsize_ );
      delete[] v_; // this can't throw
      v_ = v_new; // take ownership
      vsize_ = other.vsize_;
      vused_ = other.vused_;
    }
    return *this; // safe, no copy involved
  }
```

### Item 15: try/catch
+ Việc throw lại 1 exception mới có thể dẫn đến side effect vì client source code có thể đã handle exception cũ do vậy cần chú ý, hạn chế throw khi không cần thiết.
+ Việc define các exception specification cho 1 class là không cần thiết đặc biệt đối với trường hợp class đó là generic vì ta không thể biết T ở đó là gì và những exception có thể có với khi thao tác với T
```
template <typename T>
class Vector {
 ...
}
```

### Summary
```
The advice "be aware, drive with care" certainly applies to writing exception-safe code for containers and
other objects. To do it successfully, you do have to meet a sometimes significant extra duty of care. But
don't get unduly frightened by exceptions. Apply the guidelines outlined above—that is, isolate your resource
management, use the "update a temporary and swap" idiom, and never write classes whose destructors can
allow exceptions to escape—and you'll be well on your way to safe and happy production code that is both
exception-safe and exception-neutral. The advantages can be both concrete and well worth the trouble for
your library and your library's users
```

## Code complexity
### Item 19 có rất nhiều hiden code branch bị ẩn đi với exception
```
String EvaluateSalaryAndReturnName( Employee e )
{
   if( e.Title() == "CEO" || e.Salary() > 100000 )
   {
      cout << e.First() << " " << e.Last() << " is overpaid" << endl;
   }
   return e.First() + " " + e.Last();
}
```
#### Đánh giá:
Có tổng 23 branchs có thể xảy ra đối với hàm bên trên trong đó : 
+ 3 branchs là nonexception
+ 20 branchs là branch do exceptions có thể có khi create/update temporary object.
Hàm bên trên thỏa mãn basic exception guarantee (không gây leak) nhưng không thỏa mãn strong exception guarantee (side effects) do có exception có thể tồn tại giữa cout và return. Một khi exception xảy ra giữa -> return result sẽ lỗi tuy nhiên cout có thể vẫn in ra được một số ký tự -> *side effect*.
#### make better:
```
unique_ptr<String>
EvaluateSalaryAndReturnName( Employee e )
{
  unique_ptr<String> result = new String( e.First() + " " + e.Last() );
  if( e.Title() == "CEO" || e.Salary() > 100000 )
  {
     String message = (*result) + " is overpaid\n";
     cout << message;
  }
  return result;
}
```
Khi đó sẽ đảm bảo exception safety. không cấp phát thêm vùng nhớ ( không tạo thêm temporary string object mà chỉ return pointer -> strong exception safe  


## Class Design and Inheritance
Template 
```
class Complex {
   public:
    explicit Complex(double real, double imaginary = 0)
        : real_(real), imaginary_(imaginary) {}
    Complex& operator+=(const Complex& other) {
        real_ += other.real_;
        imaginary_ += other.imaginary_;
        return *this;
    }
    Complex& operator++() {
        ++real_;
        return *this;
    }
    const Complex operator++(int) {
        Complex temp(*this);
        ++*this;
        return temp;
    }
    ostream& Print(ostream& os) const {
        return os << "(" << real_ << "," << imaginary_ << ")";
    }

   private:
    double real_, imaginary_;
};
const Complex operator+(const Complex& lhs, const Complex& rhs) {
    Complex ret(lhs);
    ret += rhs;
    return ret;
}
ostream& operator<<(ostream& os, const Complex& c) { return c.Print(os); }
```

```
#include<iostream>
#include <complex>
    using namespace std;
class Base {
   public:
    virtual void f(int);
    virtual void f(double);
    virtual void g(int i = 100);
};
void Base::f(int) { cout << "Base::f(int)" << endl; }
void Base::f(double) { cout << "Base::f(double)" << endl; }
void Base::g(int i) { cout << i+90 << endl; }
class Derived : public Base {
   public:
    void f(complex<double>);
    void g(int i = 20) override;
};
void Derived::f(complex<double>) { cout << "Derived::f(complex)" << endl; }
void Derived::g(int i) { cout << "Derived::g() " << i << endl; }
int main() {
    Base b;
    Derived d;
    Base* pb = new Derived;
    b.f(1.0); // call base method
    d.f(1.0); // call derived method
    pb->f(1.0); call derived method due to base method is hiden ( use using to fix)
    b.g(); // call base;
    d.g();  // call derived method(i = 20);
    pb->g(); // call derived::g(i = 10) see note below
    delete pb; // bug virtual base to correct
    return 0;
}
```

### Item 24 compositon over inheritence
Tùy thuộc vào tính chất requirement mà ta sẽ lựa chon inherit hoặc composition.
+ Inherit khi cần 'IS A' relationship dấu hiều là có các virtual method ử lớp base, cần thay đổi các thuộc tính, customize ở protected access modifier  
+ composition khi 'HAS A' relationship dấu hiều ngược lại so với inherit, composition giúp hạn chế coupling giữa các class với nhau  
Đối với inherit khi 1 class ở lớp base thay đổi kéo theo derived class cũng thay đổi theo, đặc biệt độ phúc tạp sẽ lớn hơn khi 1 derived class kế thừa từ nhiều base class và tạo thành nhiều lớp trong cây kế thừa.

Đối với composition ta có thể sử dụng abstraction có các class cần 'inject' vào 1 class khác để sử dụng như 1 client thông thường qua các public method/ attribute.  

## Pimpl 
pos:
+ It's sometimes tempting to cut corners in the name of "reducing dependencies" or in the name of "efficiency,"

cons:  
+ make overhead heap allocation for the pimpl pointer.

### When will pimpl be used
+ put all nonvirtual, private members into XImpl.


### Name Lookup, Namespaces, and the interface Principal.
Dựa trên tham số truyền vào hàm compiler có thể loopkup hàm được gọi cùng namespace với parameter truyền vào đó.

Define the class in OOP:  
*A class describes a set of data, along with the functions that operate on that data*.
```
#include <algorithm>
#include <vector>
int main() {
    int ai[]= {1,2,3,1,2,9,0};
    // sort(vi, vi+7); // failed due to sort must be implicit std::sort

    std::vector<int> veci = {1,3,1,0,8,3};
    sort(veci.begin(), veci.end());

    return 0;
}
```

####  Note
+ Like overloads, default parameters are taken from the static type  

+ Never inherit publicly to reuse (code in the base class); inherit publicly in order to be reused (by existing code that uses base objects polymorphically).

## Memory management
```
Base* pb2 = new Derived[10];
delete[] pb2;
This is undefined behavior. The language requires that the static type of the pointer that is passed to operator delete[]() must be the same as its dynamic type. Scott Meyers' section, "Never Treat Arrays Polymorphically" 
```
